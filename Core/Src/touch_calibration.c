#include "touch_calibration.h"

#include "AT24C64.h"
#include "XPT2046_touch.h"
#include "lvgl.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#define TCAL_POINT_COUNT            5U
#define TCAL_SAMPLE_COUNT           8U
#define TCAL_MAX_POINT_RANGE        140
#define TCAL_MIN_RAW_VALUE          80
#define TCAL_MAX_RAW_VALUE          4000
#define TCAL_RMSE_LIMIT_PX          12.0f
#define TCAL_MAX_ERR_LIMIT_PX       24.0f
#define TCAL_ROUGH_RMSE_LIMIT_PX    24.0f
#define TCAL_ROUGH_MAX_ERR_LIMIT_PX 48.0f
#define TCAL_SOLVER_EPS             0.00001f
#define TCAL_EEPROM_ADDR            0x1F00U
#define TCAL_EEPROM_VERSION         1U
#define TCAL_SUCCESS_AUTO_CLOSE_MS  1200U
#define TCAL_FAILED_RETRY_HOLD_MS   1000U

typedef enum {
  TCAL_STATE_IDLE = 0,
  TCAL_STATE_WAIT_PRESS,
  TCAL_STATE_SAMPLING,
  TCAL_STATE_WAIT_RELEASE,
  TCAL_STATE_COMPUTE,
  TCAL_STATE_FAILED,
  TCAL_STATE_SUCCESS,
} tcal_state_t;

typedef struct __attribute__((packed)) {
  uint8_t magic[4];
  uint8_t version;
  uint8_t scan_mode;
  float coeff[6];
  uint16_t crc16;
} tcal_record_t;

extern uint8_t LCD_SCAN_MODE;
extern strType_XPT2046_TouchPara strXPT2046_TouchPara[];

static lv_obj_t *tcal_overlay;
static lv_obj_t *tcal_title_label;
static lv_obj_t *tcal_hint_label;
static lv_obj_t *tcal_target;
static lv_obj_t *tcal_retry_btn;
static lv_obj_t *tcal_retry_label;
static lv_obj_t *tcal_exit_btn;
static lv_obj_t *tcal_exit_label;

static tcal_state_t tcal_state;
static uint8_t tcal_active;
static uint8_t tcal_point_index;
static uint8_t tcal_sample_count;
static uint8_t tcal_sample_valid;
static uint32_t tcal_success_tick;
static uint8_t tcal_failed_hold_active;
static uint32_t tcal_failed_hold_tick;
static float tcal_last_rmse_px;
static float tcal_last_max_err_px;
static int32_t tcal_sum_x;
static int32_t tcal_sum_y;
static int16_t tcal_min_x;
static int16_t tcal_max_x;
static int16_t tcal_min_y;
static int16_t tcal_max_y;
static int16_t tcal_avg_x;
static int16_t tcal_avg_y;
static lv_coord_t tcal_target_px[TCAL_POINT_COUNT];
static lv_coord_t tcal_target_py[TCAL_POINT_COUNT];
static float tcal_raw_points[TCAL_POINT_COUNT][2];
static char tcal_last_message[96];

static float tcal_absf(float value)
{
  return (value < 0.0f) ? -value : value;
}

static uint16_t tcal_crc16_ccitt(const uint8_t *data, uint16_t len)
{
  uint16_t crc = 0xFFFFU;
  uint16_t i;
  uint8_t bit;

  for (i = 0U; i < len; i++) {
    crc ^= (uint16_t)data[i] << 8;
    for (bit = 0U; bit < 8U; bit++) {
      if ((crc & 0x8000U) != 0U) {
        crc = (uint16_t)((crc << 1) ^ 0x1021U);
      } else {
        crc = (uint16_t)(crc << 1);
      }
    }
  }

  return crc;
}

static void tcal_set_message(const char *text)
{
  if (text == NULL) {
    return;
  }

  (void)snprintf(tcal_last_message, sizeof(tcal_last_message), "%s", text);
  if (tcal_hint_label != NULL) {
    lv_label_set_text(tcal_hint_label, tcal_last_message);
  }
}

static lv_coord_t tcal_get_hor_res(void)
{
  lv_disp_t *disp = lv_disp_get_default();
  if (disp == NULL) {
    return 320;
  }
  return lv_disp_get_hor_res(disp);
}

static lv_coord_t tcal_get_ver_res(void)
{
  lv_disp_t *disp = lv_disp_get_default();
  if (disp == NULL) {
    return 240;
  }
  return lv_disp_get_ver_res(disp);
}

static void tcal_apply_coeff(uint8_t scan_mode, const float coeff[6])
{
  if (scan_mode > 7U) {
    return;
  }

  strXPT2046_TouchPara[scan_mode].dX_X = coeff[0];
  strXPT2046_TouchPara[scan_mode].dX_Y = coeff[1];
  strXPT2046_TouchPara[scan_mode].dX = coeff[2];
  strXPT2046_TouchPara[scan_mode].dY_X = coeff[3];
  strXPT2046_TouchPara[scan_mode].dY_Y = coeff[4];
  strXPT2046_TouchPara[scan_mode].dY = coeff[5];
}

static void tcal_export_coeff(uint8_t scan_mode, float coeff[6])
{
  if ((scan_mode > 7U) || (coeff == NULL)) {
    return;
  }

  coeff[0] = strXPT2046_TouchPara[scan_mode].dX_X;
  coeff[1] = strXPT2046_TouchPara[scan_mode].dX_Y;
  coeff[2] = strXPT2046_TouchPara[scan_mode].dX;
  coeff[3] = strXPT2046_TouchPara[scan_mode].dY_X;
  coeff[4] = strXPT2046_TouchPara[scan_mode].dY_Y;
  coeff[5] = strXPT2046_TouchPara[scan_mode].dY;
}

static uint8_t tcal_store_record(uint8_t scan_mode)
{
  tcal_record_t record;
  float coeff[6];
  uint16_t crc;

  if (scan_mode > 7U) {
    return 1U;
  }

  record.magic[0] = 'T';
  record.magic[1] = 'C';
  record.magic[2] = 'A';
  record.magic[3] = 'L';
  record.version = TCAL_EEPROM_VERSION;
  record.scan_mode = scan_mode;
  tcal_export_coeff(scan_mode, coeff);
  memcpy(record.coeff, coeff, sizeof(coeff));
  crc = tcal_crc16_ccitt((const uint8_t *)&record, (uint16_t)(sizeof(record) - sizeof(record.crc16)));
  record.crc16 = crc;

  return AT24C64_WriteBytes(TCAL_EEPROM_ADDR, (uint8_t *)&record, (uint16_t)sizeof(record));
}

static uint8_t tcal_load_record(void)
{
  tcal_record_t record;
  float coeff[6];
  uint16_t crc;
  uint8_t status;

  status = AT24C64_ReadBytes(TCAL_EEPROM_ADDR, (uint8_t *)&record, (uint16_t)sizeof(record));
  if (status != 0U) {
    return 0U;
  }

  if ((record.magic[0] != 'T') ||
      (record.magic[1] != 'C') ||
      (record.magic[2] != 'A') ||
      (record.magic[3] != 'L')) {
    return 0U;
  }

  if ((record.version != TCAL_EEPROM_VERSION) || (record.scan_mode > 7U)) {
    return 0U;
  }

  crc = tcal_crc16_ccitt((const uint8_t *)&record, (uint16_t)(sizeof(record) - sizeof(record.crc16)));
  if (crc != record.crc16) {
    return 0U;
  }

  memcpy(coeff, record.coeff, sizeof(coeff));
  tcal_apply_coeff(record.scan_mode, coeff);
  return 1U;
}

static void tcal_prepare_targets(void)
{
  lv_coord_t hor = tcal_get_hor_res();
  lv_coord_t ver = tcal_get_ver_res();
  lv_coord_t margin_x = (lv_coord_t)(hor / 8);
  lv_coord_t margin_y = (lv_coord_t)(ver / 5);

  if (margin_x < 28) { margin_x = 28; }
  if (margin_y < 42) { margin_y = 42; }

  tcal_target_px[0] = margin_x;
  tcal_target_py[0] = margin_y;
  tcal_target_px[1] = (lv_coord_t)(hor - margin_x - 1);
  tcal_target_py[1] = margin_y;
  tcal_target_px[2] = (lv_coord_t)(hor - margin_x - 1);
  tcal_target_py[2] = (lv_coord_t)(ver - margin_y - 1);
  tcal_target_px[3] = margin_x;
  tcal_target_py[3] = (lv_coord_t)(ver - margin_y - 1);
  tcal_target_px[4] = (lv_coord_t)(hor / 2);
  tcal_target_py[4] = (lv_coord_t)(ver / 2);
}

static void tcal_position_target(uint8_t idx)
{
  if ((tcal_target == NULL) || (idx >= TCAL_POINT_COUNT)) {
    return;
  }

  lv_obj_set_pos(
      tcal_target,
      (lv_coord_t)(tcal_target_px[idx] - (lv_coord_t)(lv_obj_get_width(tcal_target) / 2)),
      (lv_coord_t)(tcal_target_py[idx] - (lv_coord_t)(lv_obj_get_height(tcal_target) / 2)));
}

static void tcal_reset_sample_buffer(void)
{
  tcal_sample_count = 0U;
  tcal_sample_valid = 0U;
  tcal_sum_x = 0;
  tcal_sum_y = 0;
  tcal_min_x = 32767;
  tcal_max_x = -32768;
  tcal_min_y = 32767;
  tcal_max_y = -32768;
  tcal_avg_x = 0;
  tcal_avg_y = 0;
}

static void tcal_push_sample(int16_t raw_x, int16_t raw_y)
{
  tcal_sum_x += raw_x;
  tcal_sum_y += raw_y;
  if (raw_x < tcal_min_x) { tcal_min_x = raw_x; }
  if (raw_x > tcal_max_x) { tcal_max_x = raw_x; }
  if (raw_y < tcal_min_y) { tcal_min_y = raw_y; }
  if (raw_y > tcal_max_y) { tcal_max_y = raw_y; }
  tcal_sample_count++;
}

static uint8_t tcal_validate_sample(void)
{
  int16_t range_x;
  int16_t range_y;

  if (tcal_sample_count < TCAL_SAMPLE_COUNT) {
    return 0U;
  }

  tcal_avg_x = (int16_t)(tcal_sum_x / (int32_t)tcal_sample_count);
  tcal_avg_y = (int16_t)(tcal_sum_y / (int32_t)tcal_sample_count);
  range_x = (int16_t)(tcal_max_x - tcal_min_x);
  range_y = (int16_t)(tcal_max_y - tcal_min_y);

  if ((range_x > TCAL_MAX_POINT_RANGE) || (range_y > TCAL_MAX_POINT_RANGE)) {
    return 0U;
  }

  if ((tcal_avg_x < TCAL_MIN_RAW_VALUE) || (tcal_avg_x > TCAL_MAX_RAW_VALUE) ||
      (tcal_avg_y < TCAL_MIN_RAW_VALUE) || (tcal_avg_y > TCAL_MAX_RAW_VALUE)) {
    return 0U;
  }

  return 1U;
}

static uint8_t tcal_solve_3x3(const float A_in[3][3], const float b_in[3], float out[3])
{
  float A[3][3];
  float b[3];
  float pivot;
  float factor;
  float temp;
  uint8_t row;
  uint8_t col;
  uint8_t pivot_row;
  uint8_t j;

  for (row = 0U; row < 3U; row++) {
    for (col = 0U; col < 3U; col++) {
      A[row][col] = A_in[row][col];
    }
    b[row] = b_in[row];
  }

  for (col = 0U; col < 3U; col++) {
    pivot_row = col;
    for (row = (uint8_t)(col + 1U); row < 3U; row++) {
      if (tcal_absf(A[row][col]) > tcal_absf(A[pivot_row][col])) {
        pivot_row = row;
      }
    }

    if (tcal_absf(A[pivot_row][col]) < TCAL_SOLVER_EPS) {
      return 0U;
    }

    if (pivot_row != col) {
      for (j = col; j < 3U; j++) {
        temp = A[col][j];
        A[col][j] = A[pivot_row][j];
        A[pivot_row][j] = temp;
      }
      temp = b[col];
      b[col] = b[pivot_row];
      b[pivot_row] = temp;
    }

    pivot = A[col][col];
    for (row = (uint8_t)(col + 1U); row < 3U; row++) {
      factor = A[row][col] / pivot;
      A[row][col] = 0.0f;
      for (j = (uint8_t)(col + 1U); j < 3U; j++) {
        A[row][j] -= factor * A[col][j];
      }
      b[row] -= factor * b[col];
    }
  }

  for (row = 3U; row > 0U; row--) {
    uint8_t r = (uint8_t)(row - 1U);
    float sum = b[r];
    for (col = (uint8_t)(r + 1U); col < 3U; col++) {
      sum -= A[r][col] * out[col];
    }
    out[r] = sum / A[r][r];
  }

  return 1U;
}

static uint8_t tcal_compute_coeff(float coeff[6])
{
  float m[3][3] = {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
  float bx[3] = {0.0f, 0.0f, 0.0f};
  float by[3] = {0.0f, 0.0f, 0.0f};
  float px[3] = {0.0f, 0.0f, 0.0f};
  float py[3] = {0.0f, 0.0f, 0.0f};
  uint8_t i;

  for (i = 0U; i < TCAL_POINT_COUNT; i++) {
    float rx = tcal_raw_points[i][0];
    float ry = tcal_raw_points[i][1];
    float tx = (float)tcal_target_px[i];
    float ty = (float)tcal_target_py[i];

    m[0][0] += rx * rx;
    m[0][1] += rx * ry;
    m[0][2] += rx;
    m[1][0] += rx * ry;
    m[1][1] += ry * ry;
    m[1][2] += ry;
    m[2][0] += rx;
    m[2][1] += ry;
    m[2][2] += 1.0f;

    bx[0] += rx * tx;
    bx[1] += ry * tx;
    bx[2] += tx;

    by[0] += rx * ty;
    by[1] += ry * ty;
    by[2] += ty;
  }

  if (tcal_solve_3x3(m, bx, px) == 0U) {
    return 0U;
  }
  if (tcal_solve_3x3(m, by, py) == 0U) {
    return 0U;
  }

  coeff[0] = px[0];
  coeff[1] = px[1];
  coeff[2] = px[2];
  coeff[3] = py[0];
  coeff[4] = py[1];
  coeff[5] = py[2];
  return 1U;
}

static uint8_t tcal_validate_fit(const float coeff[6])
{
  float sum_err2 = 0.0f;
  float max_err2 = 0.0f;
  uint8_t i;

  for (i = 0U; i < TCAL_POINT_COUNT; i++) {
    float rx = tcal_raw_points[i][0];
    float ry = tcal_raw_points[i][1];
    float tx = (float)tcal_target_px[i];
    float ty = (float)tcal_target_py[i];
    float px = coeff[0] * rx + coeff[1] * ry + coeff[2];
    float py = coeff[3] * rx + coeff[4] * ry + coeff[5];
    float dx = px - tx;
    float dy = py - ty;
    float err2 = dx * dx + dy * dy;

    sum_err2 += err2;
    if (err2 > max_err2) {
      max_err2 = err2;
    }
  }

  tcal_last_rmse_px = sqrtf(sum_err2 / (float)TCAL_POINT_COUNT);
  tcal_last_max_err_px = sqrtf(max_err2);

  if (tcal_last_rmse_px > TCAL_RMSE_LIMIT_PX) {
    return 0U;
  }
  if (tcal_last_max_err_px > TCAL_MAX_ERR_LIMIT_PX) {
    return 0U;
  }
  return 1U;
}

static void tcal_retry_event_cb(lv_event_t *event);
static void tcal_exit_event_cb(lv_event_t *event);
static void tcal_overlay_event_cb(lv_event_t *event);

static void tcal_create_overlay(void)
{
  lv_obj_t *h_line;
  lv_obj_t *v_line;
  lv_coord_t hor;
  lv_coord_t ver;

  if (tcal_overlay != NULL) {
    return;
  }

  hor = tcal_get_hor_res();
  ver = tcal_get_ver_res();

  tcal_overlay = lv_obj_create(lv_layer_top());
  lv_obj_set_size(tcal_overlay, hor, ver);
  lv_obj_set_pos(tcal_overlay, 0, 0);
  lv_obj_set_style_bg_color(tcal_overlay, lv_color_hex(0x020617), 0);
  lv_obj_set_style_bg_opa(tcal_overlay, LV_OPA_80, 0);
  lv_obj_set_style_border_width(tcal_overlay, 0, 0);
  lv_obj_set_style_radius(tcal_overlay, 0, 0);
  lv_obj_set_style_pad_all(tcal_overlay, 0, 0);
  lv_obj_clear_flag(tcal_overlay, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(tcal_overlay, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(tcal_overlay, tcal_overlay_event_cb, LV_EVENT_CLICKED, NULL);

  tcal_title_label = lv_label_create(tcal_overlay);
  lv_label_set_text(tcal_title_label, "Touch Calibration");
  lv_obj_set_style_text_color(tcal_title_label, lv_color_hex(0xE2E8F0), 0);
  lv_obj_set_style_text_font(tcal_title_label, &lv_font_montserrat_16, 0);
  lv_obj_align(tcal_title_label, LV_ALIGN_TOP_MID, 0, 10);

  tcal_hint_label = lv_label_create(tcal_overlay);
  lv_obj_set_width(tcal_hint_label, (lv_coord_t)(hor - 24));
  lv_obj_set_style_text_color(tcal_hint_label, lv_color_hex(0xCBD5E1), 0);
  lv_obj_set_style_text_font(tcal_hint_label, &lv_font_montserrat_12, 0);
  lv_obj_set_style_text_align(tcal_hint_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_long_mode(tcal_hint_label, LV_LABEL_LONG_WRAP);
  lv_obj_align(tcal_hint_label, LV_ALIGN_TOP_MID, 0, 36);

  tcal_target = lv_obj_create(tcal_overlay);
  lv_obj_set_size(tcal_target, 28, 28);
  lv_obj_set_style_radius(tcal_target, 14, 0);
  lv_obj_set_style_bg_opa(tcal_target, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(tcal_target, 2, 0);
  lv_obj_set_style_border_color(tcal_target, lv_color_hex(0xF59E0B), 0);
  lv_obj_set_style_pad_all(tcal_target, 0, 0);
  lv_obj_set_style_shadow_width(tcal_target, 0, 0);
  lv_obj_clear_flag(tcal_target, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(tcal_target, LV_OBJ_FLAG_CLICKABLE);

  h_line = lv_obj_create(tcal_target);
  lv_obj_set_size(h_line, 22, 2);
  lv_obj_center(h_line);
  lv_obj_set_style_bg_color(h_line, lv_color_hex(0xF59E0B), 0);
  lv_obj_set_style_bg_opa(h_line, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(h_line, 0, 0);
  lv_obj_set_style_radius(h_line, 0, 0);
  lv_obj_set_style_shadow_width(h_line, 0, 0);
  lv_obj_clear_flag(h_line, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(h_line, LV_OBJ_FLAG_CLICKABLE);

  v_line = lv_obj_create(tcal_target);
  lv_obj_set_size(v_line, 2, 22);
  lv_obj_center(v_line);
  lv_obj_set_style_bg_color(v_line, lv_color_hex(0xF59E0B), 0);
  lv_obj_set_style_bg_opa(v_line, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(v_line, 0, 0);
  lv_obj_set_style_radius(v_line, 0, 0);
  lv_obj_set_style_shadow_width(v_line, 0, 0);
  lv_obj_clear_flag(v_line, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(v_line, LV_OBJ_FLAG_CLICKABLE);

  tcal_retry_btn = lv_btn_create(tcal_overlay);
  lv_obj_set_size(tcal_retry_btn, 92, 34);
  lv_obj_align(tcal_retry_btn, LV_ALIGN_BOTTOM_MID, 0, -12);
  lv_obj_set_style_radius(tcal_retry_btn, 14, 0);
  lv_obj_set_style_bg_color(tcal_retry_btn, lv_color_hex(0x0EA5E9), 0);
  lv_obj_set_style_bg_opa(tcal_retry_btn, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(tcal_retry_btn, 0, 0);
  lv_obj_set_style_shadow_width(tcal_retry_btn, 0, 0);
  lv_obj_add_event_cb(tcal_retry_btn, tcal_retry_event_cb, LV_EVENT_CLICKED, NULL);

  tcal_retry_label = lv_label_create(tcal_retry_btn);
  lv_label_set_text(tcal_retry_label, "Retry");
  lv_obj_set_style_text_color(tcal_retry_label, lv_color_hex(0xF8FAFC), 0);
  lv_obj_center(tcal_retry_label);

  tcal_exit_btn = lv_btn_create(tcal_overlay);
  lv_obj_set_size(tcal_exit_btn, 56, 24);
  lv_obj_align(tcal_exit_btn, LV_ALIGN_RIGHT_MID, -6, 0);
  lv_obj_set_style_radius(tcal_exit_btn, 12, 0);
  lv_obj_set_style_bg_color(tcal_exit_btn, lv_color_hex(0x334155), 0);
  lv_obj_set_style_bg_opa(tcal_exit_btn, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(tcal_exit_btn, 0, 0);
  lv_obj_set_style_shadow_width(tcal_exit_btn, 0, 0);
  lv_obj_add_event_cb(tcal_exit_btn, tcal_exit_event_cb, LV_EVENT_CLICKED, NULL);

  tcal_exit_label = lv_label_create(tcal_exit_btn);
  lv_label_set_text(tcal_exit_label, "Exit");
  lv_obj_set_style_text_color(tcal_exit_label, lv_color_hex(0xF8FAFC), 0);
  lv_obj_center(tcal_exit_label);

  lv_obj_add_flag(tcal_retry_btn, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(tcal_overlay, LV_OBJ_FLAG_HIDDEN);
}

static void tcal_show_retry(uint8_t show)
{
  if (tcal_retry_btn == NULL) {
    return;
  }

  if (show != 0U) {
    lv_obj_clear_flag(tcal_retry_btn, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_add_flag(tcal_retry_btn, LV_OBJ_FLAG_HIDDEN);
  }
}

static void tcal_begin_sequence(void)
{
  tcal_prepare_targets();
  tcal_point_index = 0U;
  tcal_state = TCAL_STATE_WAIT_PRESS;
  tcal_failed_hold_active = 0U;
  tcal_failed_hold_tick = 0U;
  tcal_last_rmse_px = 0.0f;
  tcal_last_max_err_px = 0.0f;
  tcal_reset_sample_buffer();
  tcal_show_retry(0U);
  lv_obj_clear_flag(tcal_target, LV_OBJ_FLAG_HIDDEN);
  tcal_position_target(tcal_point_index);
  tcal_set_message("Tap target 1/5 and hold.");
}

static void tcal_close_overlay(void)
{
  tcal_active = 0U;
  tcal_state = TCAL_STATE_IDLE;
  tcal_show_retry(0U);
  if (tcal_overlay != NULL) {
    lv_obj_add_flag(tcal_overlay, LV_OBJ_FLAG_HIDDEN);
  }
}

static void tcal_retry_event_cb(lv_event_t *event)
{
  LV_UNUSED(event);
  if (tcal_overlay == NULL) {
    return;
  }
  tcal_begin_sequence();
}

static void tcal_exit_event_cb(lv_event_t *event)
{
  LV_UNUSED(event);
  tcal_set_message("Calibration canceled.");
  tcal_close_overlay();
}

static void tcal_overlay_event_cb(lv_event_t *event)
{
  if (lv_event_get_code(event) != LV_EVENT_CLICKED) {
    return;
  }

  if (tcal_active == 0U) {
    return;
  }

  if (tcal_state == TCAL_STATE_FAILED) {
    tcal_begin_sequence();
  }
}

static void tcal_finish_failure(const char *text)
{
  tcal_state = TCAL_STATE_FAILED;
  tcal_failed_hold_active = 0U;
  tcal_failed_hold_tick = 0U;
  lv_obj_add_flag(tcal_target, LV_OBJ_FLAG_HIDDEN);
  tcal_show_retry(1U);
  tcal_set_message(text);
}

static void tcal_finish_success(void)
{
  tcal_state = TCAL_STATE_SUCCESS;
  tcal_success_tick = lv_tick_get();
  lv_obj_add_flag(tcal_target, LV_OBJ_FLAG_HIDDEN);
  tcal_show_retry(0U);
  tcal_set_message("Saved.");
}

static uint8_t tcal_touch_down(void)
{
  return (XPT2046_PENIRQ_Read() == XPT2046_PENIRQ_ActiveLevel) ? 1U : 0U;
}

static void tcal_compute_and_save(void)
{
  float coeff[6];
  char msg[96];
  uint8_t strict_ok;
  uint8_t rough_ok;
  uint8_t status;
  int32_t rmse_x10;
  int32_t max_err_x10;

  if (tcal_compute_coeff(coeff) == 0U) {
    tcal_finish_failure("Calibration solver failed. Tap Retry (or hold screen 1s).");
    return;
  }

  strict_ok = tcal_validate_fit(coeff);
  rmse_x10 = (int32_t)lroundf(tcal_last_rmse_px * 10.0f);
  max_err_x10 = (int32_t)lroundf(tcal_last_max_err_px * 10.0f);
  rough_ok = ((tcal_last_rmse_px <= TCAL_ROUGH_RMSE_LIMIT_PX) &&
              (tcal_last_max_err_px <= TCAL_ROUGH_MAX_ERR_LIMIT_PX)) ? 1U : 0U;
  if ((strict_ok == 0U) && (rough_ok == 0U)) {
    (void)snprintf(
        msg,
        sizeof(msg),
        "Err high R%ld.%01ld M%ld.%01ld. Retry/hold 1s.",
        (long)(rmse_x10 / 10),
        (long)(rmse_x10 % 10),
        (long)(max_err_x10 / 10),
        (long)(max_err_x10 % 10));
    tcal_finish_failure(msg);
    return;
  }

  tcal_apply_coeff(LCD_SCAN_MODE, coeff);
  status = tcal_store_record(LCD_SCAN_MODE);
  if (status != 0U) {
    tcal_finish_failure("Save failed (AT24C64). Tap Retry (or hold screen 1s).");
    return;
  }

  if (strict_ok != 0U) {
    tcal_finish_success();
    return;
  }

  tcal_state = TCAL_STATE_SUCCESS;
  tcal_success_tick = lv_tick_get();
  lv_obj_add_flag(tcal_target, LV_OBJ_FLAG_HIDDEN);
  tcal_show_retry(0U);
  (void)snprintf(
      msg,
      sizeof(msg),
      "Saved (rough) R%ld.%01ld M%ld.%01ld.",
      (long)(rmse_x10 / 10),
      (long)(rmse_x10 % 10),
      (long)(max_err_x10 / 10),
      (long)(max_err_x10 % 10));
  tcal_set_message(msg);
}

void touch_calibration_init(void)
{
  memset(tcal_last_message, 0, sizeof(tcal_last_message));
  tcal_overlay = NULL;
  tcal_title_label = NULL;
  tcal_hint_label = NULL;
  tcal_target = NULL;
  tcal_retry_btn = NULL;
  tcal_retry_label = NULL;
  tcal_exit_btn = NULL;
  tcal_exit_label = NULL;
  tcal_state = TCAL_STATE_IDLE;
  tcal_active = 0U;
  tcal_point_index = 0U;
  tcal_sample_count = 0U;
  tcal_sample_valid = 0U;
  tcal_success_tick = 0U;
  tcal_failed_hold_active = 0U;
  tcal_failed_hold_tick = 0U;
  tcal_last_rmse_px = 0.0f;
  tcal_last_max_err_px = 0.0f;
  tcal_reset_sample_buffer();

  if (tcal_load_record() != 0U) {
    tcal_set_message("Touch calibration loaded.");
  } else {
    tcal_set_message("Touch calibration defaults.");
  }
}

void touch_calibration_start(void)
{
  tcal_create_overlay();
  if (tcal_overlay == NULL) {
    tcal_set_message("Calibration UI is unavailable.");
    return;
  }

  lv_obj_clear_flag(tcal_overlay, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(tcal_overlay);
  tcal_active = 1U;
  tcal_begin_sequence();
}

uint8_t touch_calibration_is_active(void)
{
  return tcal_active;
}

void touch_calibration_process(void)
{
  strType_XPT2046_Coordinate raw;
  uint8_t down;

  if (tcal_active == 0U) {
    return;
  }

  if (tcal_state == TCAL_STATE_SUCCESS) {
    if (lv_tick_elaps(tcal_success_tick) >= TCAL_SUCCESS_AUTO_CLOSE_MS) {
      tcal_close_overlay();
    }
    return;
  }

  if (tcal_state == TCAL_STATE_FAILED) {
    down = tcal_touch_down();
    if (down != 0U) {
      if (tcal_failed_hold_active == 0U) {
        tcal_failed_hold_active = 1U;
        tcal_failed_hold_tick = lv_tick_get();
      } else if (lv_tick_elaps(tcal_failed_hold_tick) >= TCAL_FAILED_RETRY_HOLD_MS) {
        tcal_failed_hold_active = 0U;
        tcal_failed_hold_tick = 0U;
        tcal_begin_sequence();
      }
    } else {
      tcal_failed_hold_active = 0U;
      tcal_failed_hold_tick = 0U;
    }
    return;
  }

  if (tcal_state == TCAL_STATE_COMPUTE) {
    tcal_compute_and_save();
    return;
  }

  down = tcal_touch_down();
  switch (tcal_state) {
    case TCAL_STATE_WAIT_PRESS:
      if (down == 0U) {
        return;
      }

      if (XPT2046_GetRawPoint(&raw) == 0U) {
        return;
      }

      tcal_reset_sample_buffer();
      tcal_push_sample(raw.x, raw.y);
      tcal_state = TCAL_STATE_SAMPLING;
      (void)snprintf(
          tcal_last_message,
          sizeof(tcal_last_message),
          "Sampling point %u/5 (%u/8)...",
          (unsigned int)(tcal_point_index + 1U),
          (unsigned int)tcal_sample_count);
      if (tcal_hint_label != NULL) {
        lv_label_set_text(tcal_hint_label, tcal_last_message);
      }
      break;

    case TCAL_STATE_SAMPLING:
      if (down == 0U) {
        tcal_state = TCAL_STATE_WAIT_PRESS;
        tcal_reset_sample_buffer();
        (void)snprintf(
            tcal_last_message,
            sizeof(tcal_last_message),
            "Hold point %u/5 until sampling finishes.",
            (unsigned int)(tcal_point_index + 1U));
        if (tcal_hint_label != NULL) {
          lv_label_set_text(tcal_hint_label, tcal_last_message);
        }
        return;
      }

      if (XPT2046_GetRawPoint(&raw) == 0U) {
        return;
      }

      if (tcal_sample_count < TCAL_SAMPLE_COUNT) {
        tcal_push_sample(raw.x, raw.y);
      }

      if (tcal_sample_count >= TCAL_SAMPLE_COUNT) {
        tcal_sample_valid = tcal_validate_sample();
        tcal_state = TCAL_STATE_WAIT_RELEASE;
        if (tcal_sample_valid != 0U) {
          (void)snprintf(
              tcal_last_message,
              sizeof(tcal_last_message),
              "Release to confirm point %u/5.",
              (unsigned int)(tcal_point_index + 1U));
        } else {
          (void)snprintf(
              tcal_last_message,
              sizeof(tcal_last_message),
              "Point %u/5 unstable. Release to retry.",
              (unsigned int)(tcal_point_index + 1U));
        }
        if (tcal_hint_label != NULL) {
          lv_label_set_text(tcal_hint_label, tcal_last_message);
        }
      } else {
        (void)snprintf(
            tcal_last_message,
            sizeof(tcal_last_message),
            "Sampling point %u/5 (%u/8)...",
            (unsigned int)(tcal_point_index + 1U),
            (unsigned int)tcal_sample_count);
        if (tcal_hint_label != NULL) {
          lv_label_set_text(tcal_hint_label, tcal_last_message);
        }
      }
      break;

    case TCAL_STATE_WAIT_RELEASE:
      if (down != 0U) {
        return;
      }

      if (tcal_sample_valid == 0U) {
        tcal_state = TCAL_STATE_WAIT_PRESS;
        tcal_reset_sample_buffer();
        (void)snprintf(
            tcal_last_message,
            sizeof(tcal_last_message),
            "Tap target %u/5 again.",
            (unsigned int)(tcal_point_index + 1U));
        if (tcal_hint_label != NULL) {
          lv_label_set_text(tcal_hint_label, tcal_last_message);
        }
        return;
      }

      tcal_raw_points[tcal_point_index][0] = (float)tcal_avg_x;
      tcal_raw_points[tcal_point_index][1] = (float)tcal_avg_y;
      tcal_point_index++;

      if (tcal_point_index >= TCAL_POINT_COUNT) {
        tcal_state = TCAL_STATE_COMPUTE;
        tcal_set_message("Computing calibration...");
        return;
      }

      tcal_state = TCAL_STATE_WAIT_PRESS;
      tcal_reset_sample_buffer();
      tcal_position_target(tcal_point_index);
      (void)snprintf(
          tcal_last_message,
          sizeof(tcal_last_message),
          "Tap target %u/5 and hold.",
          (unsigned int)(tcal_point_index + 1U));
      if (tcal_hint_label != NULL) {
        lv_label_set_text(tcal_hint_label, tcal_last_message);
      }
      break;

    default:
      break;
  }
}

const char *touch_calibration_get_last_message(void)
{
  if (tcal_last_message[0] == '\0') {
    return "";
  }
  return tcal_last_message;
}
