/*
 * hal_i2c_master.h
 *
 *  Created on: Nov 29, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_I2C_MASTER_H_
#define SRC_HAL_HAL_I2C_MASTER_H_

#include "driverlib.h"

extern uint16_t I2cMaster_init() ;

extern uint16_t I2cMaster_write(uint8_t addr, uint8_t *s, uint16_t len);

extern uint16_t I2cMaster_read(uint8_t addr, uint8_t *s, uint16_t len);

extern uint16_t I2cMaster_wread(uint8_t addr, uint8_t *TxPtr, uint16_t TxLen, uint8_t *RxPtr, uint16_t RxLen);

extern uint16_t I2cMaster_scan(uint8_t addr);

#endif /* SRC_HAL_HAL_I2C_MASTER_H_ */
