/*
 * hal_i2c_slave.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_I2C_SLAVE_H_
#define SRC_HAL_HAL_I2C_SLAVE_H_

#include "driverlib.h"

uint16_t I2cSlave_init();

void I2cSlave_putc(uint8_t c);

void I2cSlave_puts(uint8_t *s, uint16_t len);

uint8_t I2cSlave_getc(void);

void I2cSlave_gets(uint8_t *s, uint16_t len);

void I2cSlave_setRegMap(uint8_t *MapPtr, uint16_t MapSize, uint8_t AddrByte);


#endif /* SRC_HAL_HAL_I2C_SLAVE_H_ */
