/*
 * bsp_db.h
 *
 *  Created on: Dec 1, 2017
 *      Author: nickyang
 */

#ifndef SRC_BSP_BSP_DB_H_
#define SRC_BSP_BSP_DB_H_


#include "driverlib.h"
#include "hal.h"

typedef enum eBspDbType
{
    DB_MODEL_INDEX = 0x00,
    DB_TIME = 0x80,
    DB_VERSION = 0xF0,
}eBspDbType;

uint16_t Bsp_Db_init();

void Bsp_Db_puts(eBspDbType type, uint8_t *value, uint16_t len);

void Bsp_Db_gets(eBspDbType type, uint8_t *value, uint16_t len);

#endif /* SRC_BSP_BSP_DB_H_ */
