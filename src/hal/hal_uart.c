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

#define HAL_UART_LOOPBACK       1

static uint8_t *Uart_RxPtr = 0;
static uint16_t Uart_RxLen = 0;
static uint16_t Uart_RxCnt = 0;

uint16_t Uart_init(uint32_t baudrate)
{
    // Configure UART
    EUSCI_A_UART_initParam param =
    { 0 };
    param.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    switch (baudrate)
    {
    case 115200:
        //SMCLK = 16MHz, Baudrate = 115200 , refer to slau367.pdf P766
        param.clockPrescalar = 8;
        param.firstModReg = 10;
        param.secondModReg = 0xF7;
        break;
    case 9600:
        //SMCLK = 16MHz, Baudrate = 115200 , refer to slau367.pdf P766
        param.clockPrescalar = 104;
        param.firstModReg = 2;
        param.secondModReg = 0xD6;
        break;
    default:
        return STATUS_FAIL;
    }

    param.parity = EUSCI_A_UART_NO_PARITY;
    param.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    param.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    param.uartMode = EUSCI_A_UART_MODE;
    param.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    if (STATUS_FAIL == EUSCI_A_UART_init(EUSCI_A1_BASE, &param))
    {
        return STATUS_FAIL;
    }

    EUSCI_A_UART_enable(EUSCI_A1_BASE);

    //Clear & Enable USCI_A1 RX interrupt
    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}

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
#if HAL_UART_LOOPBACK
        //Loop back function.
        UCA1TXBUF = UCA1RXBUF;
#endif
        if (Uart_RxLen == 0)
        {
            _NOP(); //No operation
            //EUSCI_A_UART_disableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);    //Disable RX Interrupt when finish.
        }
        else
        {
            //Buffer recieved data.
            Uart_RxPtr[Uart_RxCnt] = UCA1RXBUF;
            Uart_RxLen--;
            Uart_RxCnt++;
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

void Uart_putc(uint8_t c)
{
    EUSCI_A_UART_transmitData(EUSCI_A1_BASE, c);
}

uint16_t Uart_puts(uint8_t *s, uint16_t len)
{
    uint16_t i;
    for (i = len; i > 0; i--)
    {
        Uart_putc(*s++);
    }
    return len;
}

uint8_t Uart_getc(void)
{
    return EUSCI_A_UART_receiveData(EUSCI_A1_BASE);
}

uint16_t Uart_gets(uint8_t *s, uint16_t len)
{
    //Set receive buffer pointer
    Uart_RxPtr = s;
    Uart_RxLen = len;
    Uart_RxCnt = 0;

    //Clear & Enable USCI_B0 RX interrupt
    EUSCI_A_UART_clearInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    EUSCI_A_UART_enableInterrupt(EUSCI_A1_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    return STATUS_SUCCESS;
}

void Uart_print(char *fmt, ...)
{
    va_list ap;
    char string[256];
    va_start(ap, fmt);
    vsprintf(string, fmt, ap);
    Uart_puts(string, strlen(string));
    va_end(ap);
}

#if HAL_UART_PRINTF_OVERRIDE
int fputc(int _c, register FILE *_fp)
{
    Uart_putc(_c);
    return (unsigned char) _c;
}

int fputs(const char *_ptr, register FILE *_fp)
{
    return Uart_puts((char*) _ptr, strlen((char*) _ptr));
}
#endif

