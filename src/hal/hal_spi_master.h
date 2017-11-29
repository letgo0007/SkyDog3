/*
 * hal_spi.h
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#ifndef SRC_HAL_HAL_SPI_MASTER_H_
#define SRC_HAL_HAL_SPI_MASTER_H_


#include "driverlib.h"

#define SPI_M_CSPIN_MAX  16

typedef struct stCsMatrix
{
    unsigned int bit;
    unsigned int port;
    unsigned int pin;
} stCsMatrix;

static const stCsMatrix Spi_M_CsMatrix[SPI_M_CSPIN_MAX] =
{
{ .bit = BIT0, .port = GPIO_PORT_P8, .pin = GPIO_PIN4 },
{ .bit = BIT1, .port = GPIO_PORT_P8, .pin = GPIO_PIN5 },
{ .bit = BIT2, .port = GPIO_PORT_P8, .pin = GPIO_PIN6 },
{ .bit = BIT3, .port = GPIO_PORT_P8, .pin = GPIO_PIN7 }, };

extern void SpiMaster_init(uint32_t Freq);

extern void SpiMaster_putc(uint8_t c);

extern void SpiMaster_puts(uint8_t *s, uint16_t len);

extern uint8_t SpiMaster_getc(void);

extern void SpiMaster_gets(uint8_t *s, uint16_t len);

extern uint16_t SpiMaster_getTxCnt(uint16_t total);

extern uint16_t SpiMaster_getRxCnt(uint16_t total);

extern uint16_t SpiMaster_initCsPins();

extern uint16_t SpiMaster_setCsPins(uint16_t cs_sel);

#endif /* SRC_HAL_HAL_SPI_MASTER_H_ */
