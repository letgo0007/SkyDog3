/*
 * hal_mcu.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "stdio.h"
#include "hal.h"
#include "hal_clock.h"
#include "hal_gpio.h"
#include "hal_i2c_slave.h"
#include "hal_spi_master.h"
#include "hal_spi_slave.h"
#include "hal_uart.h"

uint16_t Hal_Mcu_getBoardId(void)
{
    return Hal_Gpio_getBoardId();
}

uint16_t Hal_Mcu_init()
{
    Hal_Gpio_init(0);
    Hal_Clock_init();
    Hal_Uart_init();

    printf("\nMCU Initial Success.");
    __enable_interrupt();

    return STATUS_SUCCESS;
}
