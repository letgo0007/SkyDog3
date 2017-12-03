/*
 * hal_i2c_slave.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_I2C_SLAVE_H_
#define SRC_HAL_HAL_I2C_SLAVE_H_

#include "driverlib.h"

typedef enum eI2cSlaveType
{
    I2CSLAVE_CMDLINE = 0x00, I2CSLAVE_REGMAP = 0x01,
} eI2cSlaveType;

uint16_t I2cSlave_init();

void I2cSlave_putc(uint8_t c);

void I2cSlave_puts(uint8_t *s, uint16_t len);

uint8_t I2cSlave_getc(void);

void I2cSlave_gets(uint8_t *s, uint16_t len);

void I2cSlave_setRegMap(uint8_t *MapPtr, uint16_t MapSize, uint8_t AddrByte);

uint16_t I2cSlave_getStartCount(eI2cSlaveType slv_sel);

uint16_t I2cSlave_getStopCount(eI2cSlaveType slv_sel);

void I2cSlave_clearStartCount(eI2cSlaveType slv_sel);

void I2cSlave_clearStopCount(eI2cSlaveType slv_sel);

uint16_t I2cSlave_getRxCount(eI2cSlaveType slv_sel);

uint16_t I2cSlave_getRxCount(eI2cSlaveType slv_sel);

#endif /* SRC_HAL_HAL_I2C_SLAVE_H_ */
