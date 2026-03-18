/*
 * MyDelay.h
 *
 *  Created on: Feb 21, 2026
 *      Author: xuuuu404
 */

#ifndef INC_MYDELAY_H_
#define INC_MYDELAY_H_

#include "stm32f4xx_hal.h"

void MyDelay_Init(void);
void MyDelay_us(unsigned long nus);
void MyDelay_ms(unsigned int nms);

#endif /* INC_MYDELAY_H_ */
