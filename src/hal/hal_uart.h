/*
 * hal_uart.h
 *
 *  Created on: Nov 24, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_Hal_Uart_H_
#define SRC_HAL_HAL_Hal_Uart_H_

#include "stdio.h"

#define HAL_UART_PRINTF_OVERRIDE 1

/******************************************************************************
 * @fn      Hal_Uart_init
 * @brief   Initialize MSP430 UART ( USCI_A0 or USCI_A1 ).
 * @param   baudrate    is the baudrate of UART.
 *****************************************************************************/
extern uint16_t Hal_Uart_init(void);

/******************************************************************************
 * @fn      Hal_Uart_putc
 * @brief   Put 1 char to UART TX.
 * @param   byte    the byte to be transmitted.
 *****************************************************************************/
extern void Hal_Uart_putc(char byte);

/******************************************************************************
 * @fn      Hal_Uart_puts
 * @brief   Put string to UART TX.
 * @param   string    the pointer string to be transmitted.
 *****************************************************************************/
extern unsigned char Hal_Uart_puts(char *string);

/******************************************************************************
 * @fn      Hal_Uart_getc
 * @brief   Get 1 char from UART RX.
 * @return  The last char received at UART RX.
 *****************************************************************************/
extern unsigned char Hal_Uart_getc(void);

/******************************************************************************
 * @fn      Hal_Uart_gets
 * @brief   Get string from UART RX (buffered).
 * @note    If buffer is full ,data will be override.
 * @param   s is the pointer to destination of data copy buffer.
 * @return  The number of bytes received.
 *****************************************************************************/
extern unsigned int Hal_Uart_gets(unsigned char *s);

/******************************************************************************
 * @fn      Hal_Uart_getl
 * @brief   Get 1 line of string ,till get '\r'.
 * @param   s is the pointer to destination of data copy buffer.
 * @return  The number of bytes received.
 *****************************************************************************/
extern unsigned int Hal_Uart_getl(unsigned char *s);

/******************************************************************************
 * @fn      Hal_Uart_clear
 * @brief   Clear UART buffer & count.
 *****************************************************************************/
extern void Hal_Uart_clear(void);

/******************************************************************************
 * @fn      Hal_Uart_print
 * @brief   C/C++ style print function , usage is the same as printf().
 * @note    Only %x %d %s are supported.
 * @example Hal_Uart_print("This is a test %d .\r" , i++);
 *****************************************************************************/
void Hal_Uart_print(char *fmt, ...);

#if HAL_UART_PRINTF_OVERRIDE
/******************************************************************************
 * @fn      fputc()
 * @fn      fputs()
 * @brief   C/C++ style print function , usage is the same as printf().
 *          \def Hal_Uart_PRINTF_OVERRIDE == 0 , by default on MSP430 device,
 *          printf() will be routed to JTAG interface.
 *          \def Hal_Uart_PRINTF_OVERRIDE == 1 , by redefine fputc() & fputs(),
 *          printf() will be routed to UART interface.
 *****************************************************************************/
extern int fputc(int _c, register FILE *_fp);
extern int fputs(const char *_ptr, register FILE *_fp);
#endif



#endif /* SRC_HAL_HAL_Hal_Uart_H_ */
