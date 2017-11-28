/*
 * hal_clock.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "hal_clock.h"

uint16_t Hal_Clock_init(void)
{
    // LFXT Setup
    //Set PJ.4 and PJ.5 as Primary Module Function Input.
    GPIO_setAsPeripheralModuleFunctionInputPin(
    GPIO_PORT_PJ,
    GPIO_PIN4 + GPIO_PIN5,
    GPIO_PRIMARY_MODULE_FUNCTION);

    //Set external clock frequency to 32.768 KHz
    CS_setExternalClockSource(32768, 0);
    //Set DCO frequency to 16MHz
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);

    //Set SMCLK = 16MHz, for SPI/UART/I2C
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    //Set MCLK = 16MHz, for CPU
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    //Set ACLK = 32768Hz, External LFXT.
    CS_initClockSignal(CS_ACLK, CS_LFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Start XT1 with no time out
    CS_turnOnLFXT(CS_LFXT_DRIVE_0);

    //Set wait state to 1, for FRAM operation higher than 8MHz
    FRAMCtl_configureWaitStateControl(FRAMCTL_ACCESS_TIME_CYCLES_1);

    return STATUS_SUCCESS;
}

uint16_t Hal_Watchdog_init(void)
{
    //SMCLK = 16MHz, count = 128M, watch dog = 8s
    WDT_A_initWatchdogTimer(WDT_A_BASE, WDT_A_CLOCKSOURCE_SMCLK, WDT_A_CLOCKDIVIDER_128M);
    WDT_A_start(WDT_A_BASE);
    return STATUS_SUCCESS;
}

uint16_t Hal_Watchdog_feed(void)
{
    WDT_A_resetTimer(WDT_A_BASE);
    return STATUS_SUCCESS;
}

uint16_t Hal_Rtc_init(uint32_t TimeInMs)
{
    //Set initial time stamp
    RTC_C_setCounterValue(RTC_C_BASE, TimeInMs);

    //Set Prescale 1 clock : LFXT = 32.768kHz, Prescale = 32, tick ~= 1ms
    RTC_C_initCounterPrescale(RTC_C_BASE, RTC_C_PRESCALE_1, RTC_C_PSCLOCKSELECT_ACLK, RTC_C_PSDIVIDER_32);
    RTC_C_startCounterPrescale(RTC_C_BASE, RTC_C_PRESCALE_1);

    //Init RCT using Prescale clock 1 (1ms tick)
    RTC_C_initCounter(RTC_C_BASE, RTC_C_CLOCKSELECT_RT1PS, RTC_C_COUNTERSIZE_32BIT);
    RTC_C_startClock(RTC_C_BASE);

    return STATUS_SUCCESS;
}

uint32_t Hal_Rtc_getTimeStamp(void)
{
    return RTC_C_getCounterValue(RTC_C_BASE);
}

