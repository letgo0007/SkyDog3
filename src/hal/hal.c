/*
 * hal_mcu.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "stdio.h"
#include "hal.h"

uint16_t Mcu_getBoardId(void)
{
    return Hal_Gpio_getBoardId();
}

uint16_t Mcu_reset(void)
{
    /*MCU reset using soft POR*/
    PMMCTL0_H = PMMPW_H;
    PMMCTL0_L |= PMMSWPOR;
    return STATUS_SUCCESS;
}

void Mcu_enterBsl()
{
    ((void (*)())0x1000)();
}

uint16_t Mcu_init()
{
    Hal_Clock_init(MCLK_F);
    Hal_Gpio_init(Hal_Gpio_getBoardId());
    Hal_Rtc_init(0);
    Hal_Watchdog_init();

    Uart_init(HAL_UART_BAUDRATE);
    //SpiMaster_init(1000000);
    SpiSlave_init();
    I2cSlave_init();
    I2cMaster_init();

    __enable_interrupt();
    printf("\r\nMCU Initial Done [Board ID:%d]", Hal_Gpio_getBoardId());

    return STATUS_SUCCESS;
}

