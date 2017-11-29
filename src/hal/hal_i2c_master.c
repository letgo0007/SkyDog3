/*
 * hal_i2c_master.c
 *
 *  Created on: Nov 29, 2017
 *      Author: nickyang
 */

#include "hal_i2c_master.h"

uint16_t I2cMaster_init()
{
    EUSCI_B_I2C_initMasterParam param =
    { 0 };
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_100KBPS;
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_write(uint8_t addr, uint8_t *s, uint16_t len)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    if (len > 1) // Multiple bytes
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *s++);
        len--;
        while(len > 1)
        {
            EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *s++);
            len--;
        }
        EUSCI_B_I2C_masterSendMultiByteFinish(EUSCI_B0_BASE, *s);

    }
    else if (len == 1)
    {
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0_BASE, *s);
    }

    while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE))
    {
        ;
    }

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_read(uint8_t addr, uint8_t *s, uint16_t len)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    if (len > 1)
    {
        EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);
        while(len >1)
        {
            *s++ = EUSCI_B_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
            len--;
        }
        *s = EUSCI_B_I2C_masterReceiveMultiByteFinish(EUSCI_B0_BASE);
    }
    else if (len == 1)
    {
        *s = EUSCI_B_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
    }

    while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE))
    {
        ;
    }
    return STATUS_SUCCESS;
}

uint16_t I2cMaster_wread(uint8_t addr, uint8_t *TxPtr, uint16_t TxLen, uint8_t *RxPtr, uint16_t RxLen)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    if (TxLen > 0) // Write data without stop
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *TxPtr++);
        TxLen--;
        while(TxLen > 0)
        {
            EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *TxPtr++);
            TxLen--;
        }
    }

    if (RxLen > 1) // Read data with re-start.
    {
        EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);
        while(RxLen >1)
        {
            *RxPtr++ = EUSCI_B_I2C_masterReceiveMultiByteNext(EUSCI_B0_BASE);
            RxLen--;
        }
        *RxPtr = EUSCI_B_I2C_masterReceiveMultiByteFinish(EUSCI_B0_BASE);
    }
    else if (RxLen == 1)
    {
        *RxPtr = EUSCI_B_I2C_masterReceiveSingleByte(EUSCI_B0_BASE);
    }

    while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE))
    {
        ;
    }

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_scan(uint8_t addr)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);
    UCB0CTLW0 |= UCTXSTT;

    while(EUSCI_B_I2C_SENDING_START == EUSCI_B_I2C_masterIsStartSent(EUSCI_B0_BASE))
    {
        _NOP();
    }

    if(EUSCI_B_I2C_NAK_INTERRUPT == EUSCI_B_I2C_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_I2C_NAK_INTERRUPT))
    {
        UCB0CTLW0 |= UCTXSTP;
        return 0;
    }
    else
    {
        UCB0CTLW0 |= UCTXSTP;
        return 1;
    }
}
