/*
 * app_bist.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "app_bist.h"
#include "hal.h"

uint16_t Bist_SpiMaster_loop()
{
    //Connect SPI Master MOSI <-> MISO
    //Should print out what send out of MOSI

    uint8_t buf[16] =
    { 0 };
    SpiMaster_gets(buf, 10);
    SpiMaster_setCsPins(0xFF);
    SpiMaster_puts("1234567890", 10);
    SpiMaster_setCsPins(0x00);
    printf("%s", buf);
    return 1;
}

uint16_t Bist_SpiMaster_To_SpiSlave()
{
    //Connect SPI Master <-> SPI Slave
    uint8_t buf1[16] =
    { 0 };
    uint8_t buf2[16] =
    { 0 };

    //Prepare Slave first
    SpiSlave_gets(buf1, 10);
    SpiSlave_puts("HelloWorld", 10);

    //Prepare Master
    SpiMaster_gets(buf2, 10);
    SpiMaster_puts("1234567890", 10);

    //Print received data
    /* Expected result:
     * MOSI:1234567890  RxCnt:10 TxCnt:10
     * MISO:HelloWorld  RxCnt:10 TxCnt:10
     */
    printf("MOSI:%s  RxCnt:%d TxCnt:%d\r\n", buf1, SpiSlave_getRxCnt(10), SpiMaster_getTxCnt(10));
    printf("MISO:%s  RxCnt:%d TxCnt:%d\r\n", buf2, SpiMaster_getRxCnt(10), SpiSlave_getTxCnt(10));

    return 1;
}

uint16_t Bist_I2cMaster_To_I2cSlave()
{
    uint8_t buf[16] = {0};
    I2cMaster_scan(0x10);

#if 0
    I2cSlave_gets(buf,10);
    I2cSlave_puts("ABCDEFGHIJ",5);
    I2cMaster_write(0x10,"1234567890",5);
    printf("%s",buf);
#endif
    return 1;
}
uint16_t App_Bist(void)
{
    //printf("\r\nEnter BIST (build-in self test) Mode");
    //Bist_SpiMaster_loop();
    Bist_SpiMaster_To_SpiSlave();
    //Bist_I2cMaster_To_I2cSlave();

    return 1;

}

