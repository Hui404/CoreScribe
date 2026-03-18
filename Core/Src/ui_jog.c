#include "ui_jog.h"

#include "grbl.h"

#include <stdio.h>

#define UI_JOG_QUEUE_DEPTH 8U
#define UI_JOG_LINE_LENGTH 40U

typedef enum {
  UI_JOG_COMMAND_JOG = 0,
  UI_JOG_COMMAND_LINE = 1,
} ui_jog_command_type_t;

typedef struct {
  uint8_t type;
  union {
    struct {
      int16_t x_mm_x100;
      int16_t y_mm_x100;
      uint16_t feed_mm_min;
    } jog;
    char line[UI_JOG_LINE_LENGTH];
  } data;
} ui_jog_command_t;

static ui_jog_command_t ui_jog_queue[UI_JOG_QUEUE_DEPTH];
static volatile uint8_t ui_jog_head;
static volatile uint8_t ui_jog_tail;
static volatile uint8_t ui_jog_last_status;
static volatile uint8_t ui_jog_processing;

static uint8_t ui_jog_queue_count_locked(void)
{
  if (ui_jog_head >= ui_jog_tail) {
    return (uint8_t)(ui_jog_head - ui_jog_tail);
  }

  return (uint8_t)(UI_JOG_QUEUE_DEPTH - ui_jog_tail + ui_jog_head);
}

static void ui_jog_append_axis(char *line, uint32_t line_size, char axis, int16_t value_x100)
{
  uint32_t used;
  long value;
  unsigned long abs_value;

  if (value_x100 == 0) {
    return;
  }

  used = (uint32_t)strlen(line);
  if (used >= (line_size - 1U)) {
    return;
  }

  value = (long)value_x100;
  abs_value = (value < 0L) ? (unsigned long)(-value) : (unsigned long)value;

  if (value < 0L) {
    (void)snprintf(
        &line[used],
        line_size - used,
        "%c-%lu.%02lu",
        axis,
        abs_value / 100UL,
        abs_value % 100UL);
    return;
  }

  (void)snprintf(
      &line[used],
      line_size - used,
      "%c%lu.%02lu",
      axis,
      abs_value / 100UL,
      abs_value % 100UL);
}

static uint8_t ui_jog_execute_line(const char *line)
{
  char line_copy[UI_JOG_LINE_LENGTH];

  if ((line == NULL) || (line[0] == '\0')) {
    return STATUS_INVALID_STATEMENT;
  }

  (void)snprintf(line_copy, sizeof(line_copy), "%s", line);
  if (line_copy[0] == '$') {
    return system_execute_line(line_copy);
  }

  if (sys.state & (STATE_ALARM | STATE_JOG)) {
    return STATUS_SYSTEM_GC_LOCK;
  }

  return gc_execute_line(line_copy);
}

static uint8_t ui_jog_execute(const ui_jog_command_t *command)
{
  char line[UI_JOG_LINE_LENGTH];

  (void)snprintf(line, sizeof(line), "$J=G91G21");
  ui_jog_append_axis(line, sizeof(line), 'X', command->data.jog.x_mm_x100);
  ui_jog_append_axis(line, sizeof(line), 'Y', command->data.jog.y_mm_x100);
  (void)snprintf(
      &line[strlen(line)],
      sizeof(line) - strlen(line),
      "F%u",
      (unsigned int)command->data.jog.feed_mm_min);

  return system_execute_line(line);
}

void ui_jog_init(void)
{
  ui_jog_head = 0;
  ui_jog_tail = 0;
  ui_jog_last_status = UI_JOG_STATUS_NONE;
  ui_jog_processing = 0;
}

uint8_t ui_jog_enqueue(int16_t x_mm_x100, int16_t y_mm_x100, uint16_t feed_mm_min)
{
  uint32_t primask;
  uint8_t next_head;

  if (((x_mm_x100 == 0) && (y_mm_x100 == 0)) || (feed_mm_min == 0U)) {
    return UI_JOG_ENQUEUE_INVALID;
  }

  primask = __get_PRIMASK();
  __disable_irq();

  next_head = (uint8_t)(ui_jog_head + 1U);
  if (next_head >= UI_JOG_QUEUE_DEPTH) {
    next_head = 0;
  }

  if (next_head == ui_jog_tail) {
    __set_PRIMASK(primask);
    return UI_JOG_ENQUEUE_FULL;
  }

  ui_jog_queue[ui_jog_head].type = UI_JOG_COMMAND_JOG;
  ui_jog_queue[ui_jog_head].data.jog.x_mm_x100 = x_mm_x100;
  ui_jog_queue[ui_jog_head].data.jog.y_mm_x100 = y_mm_x100;
  ui_jog_queue[ui_jog_head].data.jog.feed_mm_min = feed_mm_min;
  ui_jog_head = next_head;

  __set_PRIMASK(primask);
  return UI_JOG_ENQUEUE_OK;
}

uint8_t ui_jog_enqueue_line(const char *line)
{
  uint32_t primask;
  uint8_t next_head;
  uint32_t idx;

  if ((line == NULL) || (line[0] == '\0')) {
    return UI_JOG_ENQUEUE_INVALID;
  }

  if (strlen(line) >= UI_JOG_LINE_LENGTH) {
    return UI_JOG_ENQUEUE_INVALID;
  }

  primask = __get_PRIMASK();
  __disable_irq();

  next_head = (uint8_t)(ui_jog_head + 1U);
  if (next_head >= UI_JOG_QUEUE_DEPTH) {
    next_head = 0;
  }

  if (next_head == ui_jog_tail) {
    __set_PRIMASK(primask);
    return UI_JOG_ENQUEUE_FULL;
  }

  ui_jog_queue[ui_jog_head].type = UI_JOG_COMMAND_LINE;
  for (idx = 0; idx < UI_JOG_LINE_LENGTH; idx++) {
    ui_jog_queue[ui_jog_head].data.line[idx] = line[idx];
    if (line[idx] == '\0') {
      break;
    }
  }
  ui_jog_queue[ui_jog_head].data.line[UI_JOG_LINE_LENGTH - 1U] = '\0';
  ui_jog_head = next_head;

  __set_PRIMASK(primask);
  return UI_JOG_ENQUEUE_OK;
}

void ui_jog_process(void)
{
  uint32_t primask;
  uint8_t next_tail;
  uint8_t status;
  ui_jog_command_t command;

  if (sys.abort) {
    ui_jog_init();
    return;
  }

  primask = __get_PRIMASK();
  __disable_irq();

  if (ui_jog_processing || (ui_jog_head == ui_jog_tail)) {
    __set_PRIMASK(primask);
    return;
  }

  next_tail = (uint8_t)(ui_jog_tail + 1U);
  if (next_tail >= UI_JOG_QUEUE_DEPTH) {
    next_tail = 0;
  }

  command = ui_jog_queue[ui_jog_tail];
  ui_jog_tail = next_tail;
  ui_jog_processing = 1U;
  __set_PRIMASK(primask);

  if (command.type == UI_JOG_COMMAND_LINE) {
    status = ui_jog_execute_line(command.data.line);
  } else {
    status = ui_jog_execute(&command);
  }

  primask = __get_PRIMASK();
  __disable_irq();
  ui_jog_last_status = status;
  ui_jog_processing = 0U;
  __set_PRIMASK(primask);
}

uint8_t ui_jog_get_pending_count(void)
{
  uint32_t primask;
  uint8_t count;

  primask = __get_PRIMASK();
  __disable_irq();
  count = ui_jog_queue_count_locked();
  if (ui_jog_processing != 0U) {
    count = (uint8_t)(count + 1U);
  }
  __set_PRIMASK(primask);

  return count;
}

uint8_t ui_jog_get_last_status(void)
{
  return ui_jog_last_status;
}
