/*
 * hal_clock.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_CLOCK_H_
#define SRC_HAL_HAL_CLOCK_H_

#include "driverlib.h"

#define DCO_F_16MHz     16000000
#define DCO_F_8MHz      8000000

#define DCO_F           DCO_F_16MHz
#define LFXT_F          32768
#define HFXT_F          0

#define MCLK_F          DCO_F
#define SMCLK_F         DCO_F
#define ACLK_F          LFXT_F

#define DELAY_MS(ms)    __delay_cycles(ms * MCLK_F / 1000)      /*Delay unit in MS*/
#define DELAY_US(us)    __delay_cycles(us * MCLK_F / 1000000)   /*Delay unit in US*/

uint16_t Hal_Clock_init(uint32_t freq);

uint16_t Hal_Watchdog_init(void);

uint16_t Hal_Watchdog_feed(void);

uint16_t Hal_Watchdog_hold(void);

uint16_t Hal_Rtc_init(uint32_t TimeInMs);

uint32_t Hal_Rtc_getTimeStamp(void);

#endif /* SRC_HAL_HAL_CLOCK_H_ */
