/*
 * hal_i2c_master.c
 *
 *  Created on: Nov 29, 2017
 *      Author: nickyang
 */

#include "hal_i2c_master.h"
#include "hal_clock.h"

#define I2C_M_BUS_ERR_TIMEOUT       MCLK_F/100     //10ms
#define I2C_M_ACK_TIMEOUT           MCLK_F/100     //10ms
#define I2C_M_SCL_LOW_TIMEOUT       MCLK_F/100     //10ms
#define I2C_M_RESTART_DELAY         MCLK_F/10000  //10us

#define CHECK_RET(u16Ret, func) \
    do {\
        u16Ret = func;\
        if (STATUS_SUCCESS != u16Ret)\
        {\
            return STATUS_FAIL;\
        }\
    } while (0)

uint16_t I2cMaster_init()
{
    EUSCI_B_I2C_initMasterParam param =
    { 0 };
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
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
        EUSCI_B_I2C_masterSendMultiByteStartWithTimeout(EUSCI_B0_BASE, *s++, I2C_M_ACK_TIMEOUT);
        len--;

        while (len > 1)
        {
            EUSCI_B_I2C_masterSendMultiByteNextWithTimeout(EUSCI_B0_BASE, *s++, I2C_M_SCL_LOW_TIMEOUT);
            len--;
        }
        EUSCI_B_I2C_masterSendMultiByteFinishWithTimeout(EUSCI_B0_BASE, *s++, I2C_M_SCL_LOW_TIMEOUT);
        EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    }
    else if (len == 1)
    {
        EUSCI_B_I2C_masterSendSingleByteWithTimeout(EUSCI_B0_BASE, *s, I2C_M_SCL_LOW_TIMEOUT);
    }

    while (EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent(EUSCI_B0_BASE))
    {
        uint32_t timeout = I2C_M_BUS_ERR_TIMEOUT;
        if (timeout == 0)
        {
            return STATUS_FAIL;
        }
        timeout--;
    }

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_read(uint8_t addr, uint8_t *s, uint16_t len)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    if (len > 1)
    {
        EUSCI_B_I2C_masterReceiveStart(EUSCI_B0_BASE);

        while (EUSCI_B_I2C_SENDING_START == EUSCI_B_I2C_masterIsStartSent(EUSCI_B0_BASE))
        {
            uint32_t timeout = I2C_M_BUS_ERR_TIMEOUT;
            if (timeout == 0)
            {
                return STATUS_FAIL;
            }
            timeout--;
        }

        while (len > 1)
        {
            *s++ = EUSCI_B_I2C_masterReceiveSingle(EUSCI_B0_BASE);
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
        uint32_t timeout = I2C_M_BUS_ERR_TIMEOUT;
        if (timeout == 0)
        {
            return STATUS_FAIL;
        }
        timeout--;
    }
    return STATUS_SUCCESS;
}

uint16_t I2cMaster_wwrite(uint8_t addr, uint8_t *TxPtr1, uint16_t TxLen1, uint8_t *TxPtr2, uint16_t TxLen2)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    //1 Write data to slave, without Stop.
    if (TxLen1 > 1)
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *TxPtr1++);
        TxLen1--;

        while (TxLen1 > 0)
        {
            EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *TxPtr1++);
            TxLen1--;
        }
        //EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *TxPtr++);

    }
    else if (TxLen1 == 1)
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *TxPtr1);
    }

    //[NickYang]: Restart delay is usually required by I2C slaves to react.
    __delay_cycles(I2C_M_RESTART_DELAY);

    //2 Write data to slave, with Restart.
    I2cMaster_write(addr, TxPtr2, TxLen2);

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_wread(uint8_t addr, uint8_t *TxPtr, uint16_t TxLen, uint8_t *RxPtr, uint16_t RxLen)
{
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    //1 Write data to slave, without Stop.
    if (TxLen > 1)
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *TxPtr++);
        TxLen--;

        while (TxLen > 0)
        {
            EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *TxPtr++);
            TxLen--;
        }
        //EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE, *TxPtr++);

    }
    else if (TxLen == 1)
    {
        EUSCI_B_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, *TxPtr);
    }

    //[NickYang]: Restart delay is usually required by I2C slaves to react.
    __delay_cycles(I2C_M_RESTART_DELAY);

    //2 Read data from slave, with Restart.
    I2cMaster_read(addr, RxPtr, RxLen);

    return STATUS_SUCCESS;
}

uint16_t I2cMaster_scan(uint8_t addr)
{
    uint16_t result;
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE, addr);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_I2C_NAK_INTERRUPT);
    UCB0CTLW0 |= UCTR + UCTXSTT + UCTXSTP;

    __delay_cycles(I2C_M_ACK_TIMEOUT);

    if (EUSCI_B_I2C_NAK_INTERRUPT == EUSCI_B_I2C_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_I2C_NAK_INTERRUPT))
    {
        result = 0x00;
    }
    else
    {
        result = 0x01;
    }

    UCB0IFG = 0;
    return result;
}
