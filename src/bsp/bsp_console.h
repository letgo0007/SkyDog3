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

uint16_t Bsp_Console_register(uint8_t *str, uint16_t func(char **), uint8_t *prompt);
uint16_t Bsp_Console_help(uint8_t **args);
uint16_t Bsp_Console_Uart(void);
void Bsp_Console_printTimeStamp();

#endif /* SRC_BSP_BSP_CONSOLE_H_ */
