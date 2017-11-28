/*
 * hal_clock.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_CLOCK_H_
#define SRC_HAL_HAL_CLOCK_H_

#include "driverlib.h"

uint16_t Hal_Clock_init(void);

uint16_t Hal_Watchdog_init(void);
uint16_t Hal_Watchdog_feed(void);

uint16_t Hal_Rtc_init(uint32_t TimeInMs);
uint32_t Hal_Rtc_getTimeStamp(void);

#endif /* SRC_HAL_HAL_CLOCK_H_ */
