#ifndef TOUCH_CALIBRATION_H
#define TOUCH_CALIBRATION_H

#include <stdint.h>

void touch_calibration_init(void);
void touch_calibration_start(void);
uint8_t touch_calibration_is_active(void);
void touch_calibration_process(void);
const char *touch_calibration_get_last_message(void);

#endif
