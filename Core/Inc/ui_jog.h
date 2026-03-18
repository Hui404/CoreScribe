#ifndef UI_JOG_H
#define UI_JOG_H

#include <stdint.h>

#define UI_JOG_STATUS_NONE 0xFFU

#define UI_JOG_ENQUEUE_OK      0U
#define UI_JOG_ENQUEUE_FULL    1U
#define UI_JOG_ENQUEUE_INVALID 2U

void ui_jog_init(void);
uint8_t ui_jog_enqueue(int16_t x_mm_x100, int16_t y_mm_x100, uint16_t feed_mm_min);
uint8_t ui_jog_enqueue_line(const char *line);
void ui_jog_process(void);
uint8_t ui_jog_get_pending_count(void);
uint8_t ui_jog_get_last_status(void);

#endif
