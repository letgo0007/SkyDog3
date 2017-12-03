/*
 * hal_mcu.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_H_
#define SRC_HAL_HAL_H_

#include "driverlib.h"
#include "hal_clock.h"
#include "hal_gpio.h"
#include "hal_i2c_master.h"
#include "hal_i2c_slave.h"
#include "hal_spi_master.h"
#include "hal_spi_slave.h"
#include "hal_uart.h"

#define HAL_UART_BAUDRATE   115200
#define HAL_SPI_M_FREQ      1000000
#define HAL_I2C_S_ADD_CMD   0x10
#define HAL_I2C_S_ADD_REG   0x12

uint16_t Mcu_getBoardId(void);
uint16_t Mcu_init(void);
uint16_t Mcu_reset(void);
void Mcu_enterBsl(void);

#endif /* SRC_HAL_HAL_H_ */
