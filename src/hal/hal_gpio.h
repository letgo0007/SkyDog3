/*
 * hal_gpio.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_GPIO_H_
#define SRC_HAL_HAL_GPIO_H_

#include "driverlib.h"

/* Use Key1 & Key2 as Board ID
 * P1.1 KEY1/S1         GPIO_IN
 * P1.2 KEY2/S2         GPIO_IN
 */
#define HAL_GPIO_BOARD_ID0_PORT     GPIO_PORT_P1
#define HAL_GPIO_BOARD_ID0_PIN      GPIO_PIN1

#define HAL_GPIO_BOARD_ID1_PORT     GPIO_PORT_P1
#define HAL_GPIO_BOARD_ID1_PIN      GPIO_PIN2

uint16_t Hal_Gpio_getBoardId(void);
uint16_t Hal_Gpio_init(uint8_t board_id);


#endif /* SRC_HAL_HAL_GPIO_H_ */
