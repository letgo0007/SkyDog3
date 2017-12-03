/*
 * bsp_db.c
 *
 *  Created on: Dec 1, 2017
 *      Author: nickyang
 */

#include "bsp_db.h"

#define DB_SIZE     256

#pragma PERSISTENT(Database);
static uint8_t Database[DB_SIZE] =
{ 0 };

uint16_t Bsp_Db_init()
{
    printf("\r\nDatabase Dump\r\n");
    uint16_t i;

    for (i = 0; i < DB_SIZE; i++)
    {
        printf("0x%x\t", Database[i]);
        if (i % 16 == 15)
        {
            printf("\r\n");
        }
    }
    I2cSlave_setRegMap(Database, DB_SIZE, 1);

    return STATUS_SUCCESS;
}

void Bsp_Db_puts(eBspDbType type, uint8_t *value, uint16_t len)
{
    uint16_t i;

    for (i = 0; i < len; i++)
    {
        Database[type + i] = value[i];
    }
}

void Bsp_Db_gets(eBspDbType type, uint8_t *value, uint16_t len)
{
    uint16_t i;

    for (i = 0; i < len; i++)
    {
        value[i] = Database[type + i];
    }
}

