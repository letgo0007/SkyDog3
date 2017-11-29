/*
 * board.h
 *
 *  Created on: Nov 24, 2017
 *      Author: nickyang
 */

#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#include "driverlib.h"

#define BOARD_NAME      "MSP-EXP430FR6989"
#define BOARD_REV       "DVT"
#define BOARD_ID        0

#define BOARD_GPIO_ID0
#define BOARD_GPIO_ID1
#define BOARD_GPIO_ID2

#define BOARD_KEY1_PORT     GPIO_PORT_P1
#define BOARD_KEY1_PIN      GPIO_PIN1

#define BOARD_KEY2_PORT     GPIO_PORT_P1
#define BOARD_KEY2_PIN      GPIO_PIN2

#define BOARD_LED1_PORT     GPIO_PORT_P1
#define BOARD_LED1_PIN      GPIO_PIN0

#define BOARD_LED2_PORT     GPIO_PORT_P9
#define BOARD_LED2_PIN      GPIO_PIN7

#endif /* INC_BOARD_H_ */
