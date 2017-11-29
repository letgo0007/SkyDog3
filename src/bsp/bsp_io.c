/*
 * bsp_pm.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "bsp_io.h"
#include "board.h"
#include "hal.h"

uint8_t Bsp_IO_getKey1()
{
    return GPIO_getInputPinValue(BOARD_KEY1_PORT, BOARD_KEY1_PIN);
}

uint8_t Bsp_IO_getKey2()
{
    return GPIO_getInputPinValue(BOARD_KEY2_PORT, BOARD_KEY2_PIN);
}

uint8_t Bsp_IO_setLed1(uint8_t level)
{
    if (level)
    {
        GPIO_setOutputHighOnPin(BOARD_LED1_PORT, BOARD_LED1_PIN);
    }
    else
    {
        GPIO_setOutputLowOnPin(BOARD_LED1_PORT, BOARD_LED1_PIN);
    }
    return GPIO_getInputPinValue(BOARD_LED1_PORT, BOARD_LED1_PIN);
}

uint8_t Bsp_IO_setLed2(uint8_t level)
{
    if (level)
    {
        GPIO_setOutputHighOnPin(BOARD_LED2_PORT, BOARD_LED2_PIN);
    }
    else
    {
        GPIO_setOutputLowOnPin(BOARD_LED2_PORT, BOARD_LED2_PIN);
    }
    return GPIO_getInputPinValue(BOARD_LED2_PORT, BOARD_LED2_PIN);
}

uint16_t Bsp_IO_scan()
{
    if(Bsp_IO_getKey1())
    {
        Bsp_IO_setLed1(1);
    }
    else
    {
        Bsp_IO_setLed1(0);
    }

    if(Bsp_IO_getKey2())
    {
        Bsp_IO_setLed2(1);
    }
    else
    {
        Bsp_IO_setLed2(0);
    }

    return STATUS_SUCCESS;
}
