/*
 * bsp_i2c_cmd.c
 *
 *  Created on: Dec 1, 2017
 *      Author: nickyang
 */


#include "bsp_i2c_cmd.h"
#include "string.h"
#include "hal.h"

#define I2CCMD_SIZE     16
static uint8_t I2cCmd_RxBuf[I2CCMD_SIZE] = {0};
static uint8_t I2cCmd_TxBuf[I2CCMD_SIZE] = {0};

uint16_t Bsp_I2cCmd_init()
{
    I2cSlave_gets(I2cCmd_RxBuf,I2CCMD_SIZE);
    I2cSlave_puts(I2cCmd_TxBuf,I2CCMD_SIZE);

    return STATUS_SUCCESS;
}

uint16_t Bsp_I2cCmd_run()
{
    if(I2cSlave_getStopCount(I2CSLAVE_CMDLINE) == 0 )
    {
        return STATUS_SUCCESS; // No command to handle.
    }

    uint16_t len = 0;
    uint16_t i = 0;

    len = I2cSlave_getRxCount(I2CSLAVE_CMDLINE);

    //Process command.
    printf("\r\nGet I2C Command Length [%d]:\r\n", len);
    for(i=0;i<len;i++)
    {
        printf("0x%x\t",I2cCmd_RxBuf[i]);
    }
    printf("[%s]\r\n",I2cCmd_RxBuf);

    //Clear buffer.
    memset(I2cCmd_RxBuf,0,len);
    I2cSlave_gets(I2cCmd_RxBuf,I2CCMD_SIZE);
    I2cSlave_puts(I2cCmd_TxBuf,I2CCMD_SIZE);
    I2cSlave_clearStopCount(I2CSLAVE_CMDLINE);
    I2cSlave_clearStartCount(I2CSLAVE_CMDLINE);

    return STATUS_SUCCESS;
}
