/*
 * hal_spi_slave.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_SPI_SLAVE_H_
#define SRC_HAL_HAL_SPI_SLAVE_H_

#include "driverlib.h"

uint16_t SpiSlave_init();

extern void SpiSlave_putc(uint8_t c);

extern void SpiSlave_puts(uint8_t *s, uint16_t len);

extern uint8_t SpiSlave_getc(void);

extern void SpiSlave_gets(uint8_t *s, uint16_t len);

extern uint16_t SpiSlave_getTxCnt(uint16_t total);

extern uint16_t SpiSlave_getRxCnt(uint16_t total);

#endif /* SRC_HAL_HAL_SPI_SLAVE_H_ */
