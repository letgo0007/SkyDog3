/*
 * bsp_console.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_BSP_BSP_CONSOLE_H_
#define SRC_BSP_BSP_CONSOLE_H_

#include <stdio.h>
#include <stdint.h>

#define BSP_CONSOLE_VERSION     "V0.1"

extern uint16_t Bsp_Console_init();

extern uint16_t Bsp_Console_run(void);

extern uint16_t Bsp_Console_insert(uint8_t *str, uint16_t func(uint8_t **), uint8_t *prompt);

#endif /* SRC_BSP_BSP_CONSOLE_H_ */
