#include "ui_lvgl.h"

#include "cmsis_os.h"
#include "grbl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include "touch_calibration.h"
#include "ui_jog.h"

typedef struct {
  int8_t x_dir;
  int8_t y_dir;
  const char *label;
} ui_jog_vector_t;

typedef struct {
  const char *label;
  const char *primary_line;
  const char *secondary_line;
  const char *queued_hint;
} ui_preset_command_t;

static lv_obj_t *ui_state_card;
static lv_obj_t *ui_state_label;
static lv_obj_t *ui_x_value_label;
static lv_obj_t *ui_y_value_label;
static lv_obj_t *ui_z_value_label;
static lv_obj_t *ui_hint_label;
static lv_obj_t *ui_mode_label;
static lv_obj_t *ui_jog_buttons[4];
static lv_obj_t *ui_preset_buttons[3];
static lv_obj_t *ui_cal_button;

static uint8_t ui_last_hint_status = UI_JOG_STATUS_NONE;
static uint8_t ui_last_hint_pending = 0xFFU;
static uint8_t ui_last_jog_enabled = 0xFFU;
static uint8_t ui_last_preset_enabled = 0xFFU;

static const int16_t ui_fixed_step_x100 = 100;
static const uint16_t ui_fixed_feed_mm_min = 1500U;
static const uint16_t ui_boot_screen_hold_ms = 450U;
// 这个顺序要和按钮绑定顺序一致，回调里会按user_data取方向。
static const ui_jog_vector_t ui_jog_vectors[4] = {
    {0, 1, "Y+"},
    {-1, 0, "X-"},
    {1, 0, "X+"},
    {0, -1, "Y-"},
};
static const ui_preset_command_t ui_preset_commands[3] = {
    {"Reset", "G90G21G0Z5", "G90G21G0X0Y0", "Queued return to X0 Y0 Z5."},
    {"Up", "G90G21G0Z5", NULL, "Queued pen up to Z5."},
    {"Down", "G90G21G0Z-5", NULL, "Queued pen down to Z-5."},
};

static const char *ui_grbl_state_text(uint8_t state);
static const char *ui_jog_hint_text(uint8_t state, uint8_t status, uint8_t pending);
static lv_color_t ui_grbl_state_color(uint8_t state);
static void ui_read_machine_position(float *mpos);
static void ui_format_mm(char *buffer, uint32_t buffer_size, float value);
static void ui_set_hint_text(const char *text);
static void ui_set_panel_style(lv_obj_t *panel, lv_color_t bg_color, lv_color_t border_color);
static void ui_set_action_button_style(lv_obj_t *button, lv_color_t base_color);
static lv_obj_t *ui_create_axis_value(lv_obj_t *parent, const char *axis_text, lv_coord_t y, lv_color_t accent_color);
static void ui_update_mode_label(void);
static void ui_update_jog_button_state(uint8_t enabled);
static void ui_update_preset_button_state(uint8_t enabled);
static void ui_jog_button_event_cb(lv_event_t *event);
static void ui_preset_button_event_cb(lv_event_t *event);
static void ui_cal_button_event_cb(lv_event_t *event);
static void ui_status_timer_cb(lv_timer_t *timer);
static void ui_show_boot_screen(void);
static void ui_init_screen(void);

void ui_lvgl_task_init(void)
{
  // 先初始化显示再开触摸输入，避免上电瞬间误触。
  lv_port_disp_init();
  ui_show_boot_screen();
  (void)lv_timer_handler();
  osDelay(10);
  lv_port_indev_init();
  // 开机页多停一会，让底层外设和触摸采样更稳。
  osDelay(ui_boot_screen_hold_ms);
  ui_init_screen();
}

uint32_t ui_lvgl_task_step(void)
{
  uint32_t delay_ms;

  // 先跑一轮触摸校准，再让LVGL处理事件和重绘。
  touch_calibration_process();
  delay_ms = lv_timer_handler();
  // 返回值最少给1ms，避免osDelay(0)不让出CPU。
  return (delay_ms > 0U) ? delay_ms : 1U;
}

static const char *ui_grbl_state_text(uint8_t state)
{
  if (state == STATE_IDLE) { return "Idle"; }
  if (state & STATE_ALARM) { return "Alarm"; }
  if (state & STATE_CHECK_MODE) { return "Check"; }
  if (state & STATE_HOMING) { return "Homing"; }
  if (state & STATE_CYCLE) { return "Run"; }
  if (state & STATE_HOLD) { return "Hold"; }
  if (state & STATE_JOG) { return "Jog"; }
  if (state & STATE_SAFETY_DOOR) { return "Door"; }
  if (state & STATE_SLEEP) { return "Sleep"; }
  return "Unknown";
}

static const char *ui_jog_hint_text(uint8_t state, uint8_t status, uint8_t pending)
{
  if (!((state == STATE_IDLE) || (state & STATE_JOG))) {
    if (state & STATE_ALARM) { return "Alarm active. Unlock before jogging."; }
    if (state & STATE_HOMING) { return "Homing in progress."; }
    if (state & STATE_CYCLE) { return "Program running. Jog is locked."; }
    if (state & STATE_HOLD) { return "Cycle hold active."; }
    if (state & STATE_SAFETY_DOOR) { return "Safety door is open."; }
    if (state & STATE_SLEEP) { return "Machine is sleeping."; }
    return "Jog is unavailable in this state.";
  }

  if (pending > 0U) { return "Touch queued. Hold a key to repeat."; }

  switch (status) {
    case UI_JOG_STATUS_NONE:
    case STATUS_OK:
      return "Tap or hold a direction key to jog.";
    case STATUS_IDLE_ERROR:
      return "Jog rejected because the machine is busy.";
    case STATUS_TRAVEL_EXCEEDED:
      return "Jog blocked by travel limits.";
    case STATUS_INVALID_JOG_COMMAND:
      return "Jog command was rejected.";
    case STATUS_SYSTEM_GC_LOCK:
      return "Grbl lock is active.";
    case STATUS_CHECK_DOOR:
      return "Close the safety door first.";
    default:
      return "Jog request failed. Check Grbl state.";
  }
}

static lv_color_t ui_grbl_state_color(uint8_t state)
{
  if (state == STATE_IDLE) { return lv_color_hex(0x2A9D8F); }
  if (state & STATE_ALARM) { return lv_color_hex(0xD62839); }
  if (state & STATE_HOLD) { return lv_color_hex(0xF4A261); }
  if (state & STATE_HOMING) { return lv_color_hex(0xA855F7); }
  if (state & STATE_CYCLE) { return lv_color_hex(0x277DA1); }
  if (state & STATE_JOG) { return lv_color_hex(0x3A86FF); }
  if (state & STATE_SAFETY_DOOR) { return lv_color_hex(0xFB8500); }
  if (state & STATE_SLEEP) { return lv_color_hex(0x6B7280); }
  return lv_color_hex(0x415A77);
}

static void ui_read_machine_position(float *mpos)
{
  int32_t steps_snapshot[N_AXIS];
  uint32_t primask;
  uint8_t idx;

  // 位置可能在中断里更新，先关中断拷贝快照再做换算。
  primask = __get_PRIMASK();
  __disable_irq();
  for (idx = 0; idx < N_AXIS; idx++) {
    steps_snapshot[idx] = sys_position[idx];
  }
  if (primask == 0U) {
    __enable_irq();
  }

  system_convert_array_steps_to_mpos(mpos, steps_snapshot);
}

static void ui_format_mm(char *buffer, uint32_t buffer_size, float value)
{
  long scaled;
  unsigned long abs_scaled;
  unsigned long whole;
  unsigned long frac;

  scaled = (long)(value * 100.0f);
  if (scaled < 0) {
    abs_scaled = (unsigned long)(-scaled);
    whole = abs_scaled / 100UL;
    frac = abs_scaled % 100UL;
    lv_snprintf(buffer, buffer_size, "-%lu.%02lu", whole, frac);
    return;
  }

  abs_scaled = (unsigned long)scaled;
  whole = abs_scaled / 100UL;
  frac = abs_scaled % 100UL;
  lv_snprintf(buffer, buffer_size, "%lu.%02lu", whole, frac);
}

static void ui_set_hint_text(const char *text)
{
  if (ui_hint_label == NULL) {
    return;
  }

  lv_label_set_text(ui_hint_label, text);
}

static void ui_set_panel_style(lv_obj_t *panel, lv_color_t bg_color, lv_color_t border_color)
{
  lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(panel, 20, 0);
  lv_obj_set_style_bg_color(panel, bg_color, 0);
  lv_obj_set_style_bg_opa(panel, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(panel, 1, 0);
  lv_obj_set_style_border_color(panel, border_color, 0);
  lv_obj_set_style_border_opa(panel, LV_OPA_70, 0);
  lv_obj_set_style_shadow_width(panel, 14, 0);
  lv_obj_set_style_shadow_color(panel, border_color, 0);
  lv_obj_set_style_shadow_opa(panel, LV_OPA_20, 0);
  lv_obj_set_style_pad_all(panel, 0, 0);
}

static void ui_set_action_button_style(lv_obj_t *button, lv_color_t base_color)
{
  lv_obj_set_style_radius(button, 16, 0);
  lv_obj_set_style_bg_color(button, base_color, 0);
  lv_obj_set_style_bg_opa(button, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(button, 0, 0);
  lv_obj_set_style_shadow_width(button, 10, 0);
  lv_obj_set_style_shadow_color(button, base_color, 0);
  lv_obj_set_style_shadow_opa(button, LV_OPA_20, 0);
  lv_obj_set_style_text_color(button, lv_color_hex(0xF8FAFC), 0);
  lv_obj_set_style_text_font(button, &lv_font_montserrat_16, 0);
  lv_obj_set_style_bg_color(button, lv_color_hex(0x4B5563), LV_STATE_DISABLED);
  lv_obj_set_style_shadow_width(button, 0, LV_STATE_DISABLED);
  lv_obj_set_style_text_color(button, lv_color_hex(0xCBD5E1), LV_STATE_DISABLED);
}

static lv_obj_t *ui_create_axis_value(lv_obj_t *parent, const char *axis_text, lv_coord_t y, lv_color_t accent_color)
{
  lv_obj_t *caption;
  lv_obj_t *value;

  caption = lv_label_create(parent);
  lv_label_set_text(caption, axis_text);
  lv_obj_set_style_text_color(caption, accent_color, 0);
  lv_obj_set_style_text_font(caption, &lv_font_montserrat_12, 0);
  lv_obj_set_pos(caption, 14, y);

  value = lv_label_create(parent);
  lv_label_set_text(value, "0.00");
  lv_obj_set_style_text_color(value, lv_color_hex(0xF8FAFC), 0);
  lv_obj_set_style_text_font(value, &lv_font_montserrat_16, 0);
  lv_obj_set_pos(value, 38, y - 4);

  return value;
}

static void ui_update_mode_label(void)
{
  char step_text[12];

  if (ui_mode_label == NULL) {
    return;
  }

  ui_format_mm(
      step_text,
      sizeof(step_text),
      (float)ui_fixed_step_x100 / 100.0f);
  lv_label_set_text_fmt(
      ui_mode_label,
      "%s\nF%u",
      step_text,
      (unsigned int)ui_fixed_feed_mm_min);
}

static void ui_update_jog_button_state(uint8_t enabled)
{
  uint8_t idx;

  for (idx = 0; idx < 4U; idx++) {
    if (ui_jog_buttons[idx] == NULL) {
      continue;
    }

    if (enabled != 0U) {
      lv_obj_clear_state(ui_jog_buttons[idx], LV_STATE_DISABLED);
    } else {
      lv_obj_add_state(ui_jog_buttons[idx], LV_STATE_DISABLED);
    }
  }
}

static void ui_update_preset_button_state(uint8_t enabled)
{
  uint8_t idx;

  for (idx = 0; idx < 3U; idx++) {
    if (ui_preset_buttons[idx] == NULL) {
      continue;
    }

    if (enabled != 0U) {
      lv_obj_clear_state(ui_preset_buttons[idx], LV_STATE_DISABLED);
    } else {
      lv_obj_add_state(ui_preset_buttons[idx], LV_STATE_DISABLED);
    }
  }

  if (ui_cal_button != NULL) {
    if (enabled != 0U) {
      lv_obj_clear_state(ui_cal_button, LV_STATE_DISABLED);
    } else {
      lv_obj_add_state(ui_cal_button, LV_STATE_DISABLED);
    }
  }
}

static void ui_jog_button_event_cb(lv_event_t *event)
{
  const ui_jog_vector_t *vector;
  lv_event_code_t code;
  int16_t step_x100;
  uint16_t feed_mm_min;
  uint8_t enqueue_status;
  char step_text[12];
  char hint_text[48];

  code = lv_event_get_code(event);
  if ((code != LV_EVENT_CLICKED) && (code != LV_EVENT_LONG_PRESSED_REPEAT)) {
    return;
  }

  vector = (const ui_jog_vector_t *)lv_event_get_user_data(event);
  if (vector == NULL) {
    return;
  }

  // 只有Idle/Jog状态允许点动，其它状态只提示不入队。
  if (!((sys.state == STATE_IDLE) || (sys.state & STATE_JOG))) {
    ui_set_hint_text(ui_jog_hint_text(sys.state, STATUS_IDLE_ERROR, 0U));
    return;
  }

  step_x100 = ui_fixed_step_x100;
  feed_mm_min = ui_fixed_feed_mm_min;
  enqueue_status = ui_jog_enqueue(
      (int16_t)(vector->x_dir * step_x100),
      (int16_t)(vector->y_dir * step_x100),
      feed_mm_min);

  switch (enqueue_status) {
    case UI_JOG_ENQUEUE_OK:
      ui_format_mm(step_text, sizeof(step_text), (float)step_x100 / 100.0f);
      lv_snprintf(
          hint_text,
          sizeof(hint_text),
          "Queued %s by %s mm.",
          vector->label,
          step_text);
      ui_set_hint_text(hint_text);
      break;
    case UI_JOG_ENQUEUE_FULL:
      ui_set_hint_text("Jog queue is full. Wait for motion to catch up.");
      break;
    default:
      ui_set_hint_text("Jog request is invalid.");
      break;
  }
}

static void ui_preset_button_event_cb(lv_event_t *event)
{
  const ui_preset_command_t *command;
  uint8_t enqueue_status;
  uint8_t secondary_status;

  if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
    return;
  }

  command = (const ui_preset_command_t *)lv_event_get_user_data(event);
  if (command == NULL) {
    return;
  }

  if (sys.state != STATE_IDLE) {
    ui_set_hint_text("Preset move requires Idle state.");
    return;
  }

  enqueue_status = ui_jog_enqueue_line(command->primary_line);
  if (enqueue_status != UI_JOG_ENQUEUE_OK) {
    if (enqueue_status == UI_JOG_ENQUEUE_FULL) {
      ui_set_hint_text("Queue is full. Try again after motion finishes.");
    } else {
      ui_set_hint_text("Preset command is invalid.");
    }
    return;
  }

  if (command->secondary_line != NULL) {
    secondary_status = ui_jog_enqueue_line(command->secondary_line);
    if (secondary_status != UI_JOG_ENQUEUE_OK) {
      if (secondary_status == UI_JOG_ENQUEUE_FULL) {
        ui_set_hint_text("Queued Z5 only. Queue filled before XY reset.");
      } else {
        ui_set_hint_text("Queued first move only. Second command is invalid.");
      }
      return;
    }
  }

  ui_set_hint_text(command->queued_hint);
}

static void ui_cal_button_event_cb(lv_event_t *event)
{
  if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
    return;
  }

  if (touch_calibration_is_active() != 0U) {
    ui_set_hint_text(touch_calibration_get_last_message());
    ui_last_hint_status = UI_JOG_STATUS_NONE;
    ui_last_hint_pending = 0xFFU;
    return;
  }

  if (sys.state != STATE_IDLE) {
    ui_set_hint_text("Calibration requires Idle state.");
    return;
  }

  touch_calibration_start();
  ui_set_hint_text(touch_calibration_get_last_message());
}

static void ui_status_timer_cb(lv_timer_t *timer)
{
  float mpos[N_AXIS];
  char x_text[16];
  char y_text[16];
  char z_text[16];
  uint8_t pending;
  uint8_t status;
  uint8_t jog_enabled;
  uint8_t preset_enabled;
  uint8_t previous_enabled;
  uint8_t previous_preset_enabled;

  LV_UNUSED(timer);

  if ((ui_state_label == NULL) || (ui_x_value_label == NULL) || (ui_y_value_label == NULL) || (ui_z_value_label == NULL)) {
    return;
  }

  // 周期刷新UI：坐标、机器状态、队列长度、按钮可用性。
  ui_read_machine_position(mpos);
  ui_format_mm(x_text, sizeof(x_text), mpos[X_AXIS]);
  ui_format_mm(y_text, sizeof(y_text), mpos[Y_AXIS]);
  ui_format_mm(z_text, sizeof(z_text), mpos[Z_AXIS]);

  pending = ui_jog_get_pending_count();
  status = ui_jog_get_last_status();
  jog_enabled = ((sys.state == STATE_IDLE) || (sys.state & STATE_JOG)) ? 1U : 0U;
  preset_enabled = ((sys.state == STATE_IDLE) && (touch_calibration_is_active() == 0U)) ? 1U : 0U;
  previous_enabled = ui_last_jog_enabled;
  previous_preset_enabled = ui_last_preset_enabled;

  lv_obj_set_style_bg_color(ui_state_card, ui_grbl_state_color(sys.state), 0);
  lv_label_set_text_fmt(
      ui_state_label,
      "%s  Q%u",
      ui_grbl_state_text(sys.state),
      (unsigned int)pending);
  lv_label_set_text(ui_x_value_label, x_text);
  lv_label_set_text(ui_y_value_label, y_text);
  lv_label_set_text(ui_z_value_label, z_text);

  if (jog_enabled != previous_enabled) {
    ui_update_jog_button_state(jog_enabled);
    ui_last_jog_enabled = jog_enabled;
  }

  if (preset_enabled != previous_preset_enabled) {
    ui_update_preset_button_state(preset_enabled);
    ui_last_preset_enabled = preset_enabled;
  }

  // 校准时提示文字由校准模块接管，避免被普通提示覆盖。
  if (touch_calibration_is_active() != 0U) {
    ui_set_hint_text(touch_calibration_get_last_message());
    return;
  }

  if ((status != ui_last_hint_status) || (pending != ui_last_hint_pending) || (jog_enabled != previous_enabled) || (preset_enabled != previous_preset_enabled)) {
    ui_set_hint_text(ui_jog_hint_text(sys.state, status, pending));
    ui_last_hint_status = status;
    ui_last_hint_pending = pending;
  }
}

static void ui_show_boot_screen(void)
{
  lv_obj_t *screen = lv_scr_act();
  lv_obj_t *title;
  lv_obj_t *subtitle;

  lv_obj_clean(screen);
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x04121B), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x15324B), 0);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, 0);

  title = lv_label_create(screen);
  lv_label_set_text(title, "CoreScribe");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0xE2E8F0), 0);
  lv_obj_align(title, LV_ALIGN_CENTER, 0, -18);

  subtitle = lv_label_create(screen);
  lv_label_set_text(subtitle, "Initializing...");
  lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(subtitle, lv_color_hex(0x93C5FD), 0);
  lv_obj_align(subtitle, LV_ALIGN_CENTER, 0, 18);
}

static void ui_init_screen(void)
{
  lv_obj_t *screen = lv_scr_act();
  lv_obj_t *info_panel;
  lv_obj_t *jog_panel;
  lv_obj_t *title;
  lv_obj_t *button;
  lv_obj_t *button_label;
  lv_obj_t *mode_tile;
  uint8_t idx;
  static const lv_coord_t preset_x[3] = {12, 66, 120};

  lv_obj_clean(screen);
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x06141D), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x15324B), 0);
  lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, 0);

  info_panel = lv_obj_create(screen);
  lv_obj_set_size(info_panel, 124, 228);
  lv_obj_align(info_panel, LV_ALIGN_LEFT_MID, 8, 0);
  ui_set_panel_style(info_panel, lv_color_hex(0x10202F), lv_color_hex(0x35506B));

  title = lv_label_create(info_panel);
  lv_label_set_text(title, "CoreScribe");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0xE0FBFC), 0);
  lv_obj_set_pos(title, 14, 10);

  ui_state_card = lv_obj_create(info_panel);
  lv_obj_set_size(ui_state_card, 96, 26);
  lv_obj_set_pos(ui_state_card, 14, 36);
  lv_obj_clear_flag(ui_state_card, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(ui_state_card, 13, 0);
  lv_obj_set_style_border_width(ui_state_card, 0, 0);
  lv_obj_set_style_shadow_width(ui_state_card, 0, 0);
  lv_obj_set_style_pad_all(ui_state_card, 0, 0);

  ui_state_label = lv_label_create(ui_state_card);
  lv_obj_set_style_text_color(ui_state_label, lv_color_hex(0xF8FAFC), 0);
  lv_obj_set_style_text_font(ui_state_label, &lv_font_montserrat_12, 0);
  lv_obj_center(ui_state_label);

  ui_x_value_label = ui_create_axis_value(info_panel, "X", 72, lv_color_hex(0x4CC9F0));
  ui_y_value_label = ui_create_axis_value(info_panel, "Y", 102, lv_color_hex(0x80ED99));
  ui_z_value_label = ui_create_axis_value(info_panel, "Z", 132, lv_color_hex(0xF4A261));

  jog_panel = lv_obj_create(screen);
  lv_obj_set_size(jog_panel, 180, 228);
  lv_obj_align(jog_panel, LV_ALIGN_RIGHT_MID, -8, 0);
  ui_set_panel_style(jog_panel, lv_color_hex(0x132433), lv_color_hex(0x46657F));

  title = lv_label_create(jog_panel);
  lv_label_set_text(title, "Touch Jog");
  lv_obj_set_style_text_font(title, &lv_font_montserrat_16, 0);
  lv_obj_set_style_text_color(title, lv_color_hex(0xE0FBFC), 0);
  lv_obj_set_pos(title, 14, 10);

  button = lv_btn_create(jog_panel);
  lv_obj_set_size(button, 42, 22);
  lv_obj_set_pos(button, 124, 8);
  lv_obj_add_event_cb(button, ui_cal_button_event_cb, LV_EVENT_CLICKED, NULL);
  ui_set_action_button_style(button, lv_color_hex(0x6D597A));
  ui_cal_button = button;

  button_label = lv_label_create(button);
  lv_obj_set_style_text_font(button_label, &lv_font_montserrat_12, 0);
  lv_label_set_text(button_label, "Cal");
  lv_obj_center(button_label);

  ui_hint_label = lv_label_create(jog_panel);
  lv_obj_set_width(ui_hint_label, 152);
  lv_label_set_long_mode(ui_hint_label, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(ui_hint_label, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(ui_hint_label, lv_color_hex(0xC6D4E1), 0);
  lv_obj_set_style_text_align(ui_hint_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_pos(ui_hint_label, 14, 72);

  for (idx = 0; idx < 3U; idx++) {
    button = lv_btn_create(jog_panel);
    lv_obj_set_size(button, 48, 24);
    lv_obj_set_pos(button, preset_x[idx], 38);
    lv_obj_add_event_cb(button, ui_preset_button_event_cb, LV_EVENT_CLICKED, (void *)&ui_preset_commands[idx]);
    ui_set_action_button_style(button, lv_color_hex(0x3B556E));
    ui_preset_buttons[idx] = button;

    button_label = lv_label_create(button);
    lv_obj_set_style_text_font(button_label, &lv_font_montserrat_12, 0);
    lv_label_set_text(button_label, ui_preset_commands[idx].label);
    lv_obj_center(button_label);
  }

  button = lv_btn_create(jog_panel);
  lv_obj_set_size(button, 48, 30);
  lv_obj_set_pos(button, 66, 102);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_CLICKED, (void *)&ui_jog_vectors[0]);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_LONG_PRESSED_REPEAT, (void *)&ui_jog_vectors[0]);
  ui_set_action_button_style(button, lv_color_hex(0x2A9D8F));
  ui_jog_buttons[0] = button;

  button_label = lv_label_create(button);
  lv_label_set_text(button_label, "Y+");
  lv_obj_center(button_label);

  button = lv_btn_create(jog_panel);
  lv_obj_set_size(button, 48, 36);
  lv_obj_set_pos(button, 12, 138);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_CLICKED, (void *)&ui_jog_vectors[1]);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_LONG_PRESSED_REPEAT, (void *)&ui_jog_vectors[1]);
  ui_set_action_button_style(button, lv_color_hex(0x457B9D));
  ui_jog_buttons[1] = button;

  button_label = lv_label_create(button);
  lv_label_set_text(button_label, "X-");
  lv_obj_center(button_label);

  mode_tile = lv_obj_create(jog_panel);
  lv_obj_set_size(mode_tile, 48, 36);
  lv_obj_set_pos(mode_tile, 66, 138);
  lv_obj_clear_flag(mode_tile, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_radius(mode_tile, 16, 0);
  lv_obj_set_style_bg_color(mode_tile, lv_color_hex(0x203548), 0);
  lv_obj_set_style_bg_opa(mode_tile, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(mode_tile, 1, 0);
  lv_obj_set_style_border_color(mode_tile, lv_color_hex(0x46657F), 0);
  lv_obj_set_style_pad_all(mode_tile, 0, 0);
  lv_obj_set_style_shadow_width(mode_tile, 0, 0);

  ui_mode_label = lv_label_create(mode_tile);
  lv_obj_set_style_text_font(ui_mode_label, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_color(ui_mode_label, lv_color_hex(0xF8FAFC), 0);
  lv_obj_center(ui_mode_label);

  button = lv_btn_create(jog_panel);
  lv_obj_set_size(button, 48, 36);
  lv_obj_set_pos(button, 120, 138);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_CLICKED, (void *)&ui_jog_vectors[2]);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_LONG_PRESSED_REPEAT, (void *)&ui_jog_vectors[2]);
  ui_set_action_button_style(button, lv_color_hex(0xE76F51));
  ui_jog_buttons[2] = button;

  button_label = lv_label_create(button);
  lv_label_set_text(button_label, "X+");
  lv_obj_center(button_label);

  button = lv_btn_create(jog_panel);
  lv_obj_set_size(button, 48, 30);
  lv_obj_set_pos(button, 66, 180);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_CLICKED, (void *)&ui_jog_vectors[3]);
  lv_obj_add_event_cb(button, ui_jog_button_event_cb, LV_EVENT_LONG_PRESSED_REPEAT, (void *)&ui_jog_vectors[3]);
  ui_set_action_button_style(button, lv_color_hex(0x8E5CF7));
  ui_jog_buttons[3] = button;

  button_label = lv_label_create(button);
  lv_label_set_text(button_label, "Y-");
  lv_obj_center(button_label);

  // 每100ms刷新一次状态面板，响应和负载比较平衡。
  lv_timer_create(ui_status_timer_cb, 100, NULL);
  ui_update_mode_label();
  // 清掉上次状态缓存，确保首次刷新把界面完整同步一遍。
  ui_last_hint_status = UI_JOG_STATUS_NONE;
  ui_last_hint_pending = 0xFFU;
  ui_last_jog_enabled = 0xFFU;
  ui_last_preset_enabled = 0xFFU;
  ui_status_timer_cb(NULL);
}
