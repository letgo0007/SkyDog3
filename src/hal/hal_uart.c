/*
 * hal_uart.c
 *
 *  Created on: Nov 24, 2017
 *      Author: nickyang
 */

#include "driverlib.h"
#include "hal_uart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"

#define HAL_UART_RXBUF_SIZE     256
#define HAL_UART_TXBUF_SIZE     256
#define HAL_UART_LOOPBACK       1
#define HAL_UART_END_CHR        '\n'

static uint8_t u8Hal_UartRxBuf[HAL_UART_RXBUF_SIZE];
static uint16_t u16Hal_UartRxCount = 0;
static uint16_t u16Hal_UartRxLineFlag = 0;

uint16_t Hal_Uart_init(void)
{
    // Configure UART
    EUSCI_A_UART_initParam param =
    { 0 };
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    //SMCLK = 16MHz, Baudrate = 115200 , refer to slau367.pdf P766
    param.clockPrescalar = 8;
    param.firstModReg = 10;
    param.secondModReg = 0xF7;

    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, &param))
    {
        return STATUS_FAIL;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);

    //Clear & Enable USCI_A1 RX interrupt
    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE,EUSCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}

/*! Put 1 char to UART TX. */
void Hal_Uart_putc(char byte)
{
    EUSCI_A_UART_transmitData(EUSCI_A1_BASE,byte);
}

/*! Put string (end with 0x00) to UART TX. */
unsigned char Hal_Uart_puts(char *string)
{
    unsigned char i, len;
    len = strlen(string);
    for (i = 0; i < len; i++)
    {
        Hal_Uart_putc(string[i]);
    }
    return len;
}

/*! Return last received data */
unsigned char Hal_Uart_getc(void)
{
    return EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
}

/*! Copy all buffered data out. */
unsigned int Hal_Uart_gets(unsigned char *s)
{
    if (u16Hal_UartRxCount)
    {
        memcpy(s, u8Hal_UartRxBuf, u16Hal_UartRxCount);
    }
    return u16Hal_UartRxCount;
}

/*! Copy 1 line data out (before 'end character'). */
unsigned int Hal_Uart_getl(unsigned char *s)
{
    if(u16Hal_UartRxLineFlag)
    {
        memcpy(s, u8Hal_UartRxBuf, u16Hal_UartRxLineFlag);
    }

    return u16Hal_UartRxLineFlag;
}

/*! Clear UART RX buffer.  */
void Hal_Uart_clear(void)
{
    memset(u8Hal_UartRxBuf, 0x00, u16Hal_UartRxCount);
    u16Hal_UartRxCount = 0;
    u16Hal_UartRxLineFlag = 0;
}

/*! Print using UART. The same as printf.
 *  Use this when UART_PRINTF_OVERRIDE = 0 when you don't want to override original printf
 */
void Hal_Uart_print(char *fmt, ...)
{
    va_list ap;
    char string[HAL_UART_TXBUF_SIZE];
    va_start(ap, fmt);
    vsprintf(string, fmt, ap);
    Hal_Uart_puts(string);
    va_end(ap);
}

#if HAL_UART_PRINTF_OVERRIDE
int fputc(int _c, register FILE *_fp)
{
    Hal_Uart_putc(_c);
    return (unsigned char) _c;
}

int fputs(const char *_ptr, register FILE *_fp)
{
    return Hal_Uart_puts((char*) _ptr);
}
#endif

//******************************************************************************
//
//This is the USCI_A1 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A1_VECTOR)))
#endif
void USCI_A1_ISR(void)
{
    switch (__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE:
        break;
    case USCI_UART_UCRXIFG:
    {
        uint8_t rxdata;
        rxdata = UCA1RXBUF;
#if HAL_UART_LOOPBACK
        //Loop back function.
        UCA1TXBUF = UCA1RXBUF;
#endif
        //Buffer reveived data
        u8Hal_UartRxBuf[u16Hal_UartRxCount] = UCA1RXBUF;
        u16Hal_UartRxCount ++;

        //Set line end flag.
        if(rxdata == HAL_UART_END_CHR)
        {
            u16Hal_UartRxLineFlag = u16Hal_UartRxCount;
        }

        break;
    }
    case USCI_UART_UCTXIFG:
        break;
    case USCI_UART_UCSTTIFG:
        break;
    case USCI_UART_UCTXCPTIFG:
        break;
    }
}
