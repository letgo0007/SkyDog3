/*
 * hal_uart.h
 *
 *  Created on: Nov 24, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_Hal_Uart_H_
#define SRC_HAL_HAL_Hal_Uart_H_

#include "stdio.h"

#define HAL_UART_PRINTF_OVERRIDE    1

extern uint16_t Uart_init(uint32_t baudrate);
extern void Uart_putc(uint8_t c);
extern uint16_t Uart_puts(uint8_t *s, uint16_t len);
extern uint8_t Uart_getc(void);
extern uint16_t Uart_gets(uint8_t *s, uint16_t len);
void Uart_print(char *fmt, ...);

#if HAL_UART_PRINTF_OVERRIDE
extern int fputc(int _c, register FILE *_fp);
extern int fputs(const char *_ptr, register FILE *_fp);
#endif

#endif /* SRC_HAL_HAL_Hal_Uart_H_ */
