/*
 * app_bist.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "app_bist.h"
#include "hal.h"
#include "string.h"

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
    uint8_t buf[64] =
    { 0 };
    uint8_t slave_add = 0x10;

    printf("\r\n**************************\r\n");
    printf("I2C Master(UCB0) <-> I2C Slave(UCB1) self test\r\n");
#if 1
    uint16_t i;

    printf("Scanning available slave address:\r\n");
    for (i = 0x0; i <= 0x7F; i++)
    {
        if (STATUS_SUCCESS == I2cMaster_scan(i))
        {
            printf("SlaveAddr[0x%x]\r\n", i);
        }
    }
#endif

#if 1
    /*  Master Multiple read <-> Slave Multiple write
     *     > Master read1[ABCDEFGHIJ]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_puts("ABCDEFGHIJKLMN", 16);
    I2cMaster_read(slave_add, buf, 10);
    printf("Master read1[%s]\r\n", buf);

    /*  Master Multiple read <-> Slave Single write
     *     > Master read2[z?????????]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_putc('z');
    I2cMaster_read(slave_add, buf, 10);
    printf("Master read2[%s]\r\n", buf);

    /*  Master Single read <-> Slave Multiple write
     *     > Master read3[A]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_puts("ABCDEFGHIJKLMN", 16);
    I2cMaster_read(slave_add, buf, 1);
    printf("Master read3[%s]\r\n", buf);

#endif

#if 1
    /*  Master Multiple write <-> Slave Multiple read
     *     > Master read1[1234567890]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_gets(buf, 16);
    I2cMaster_write(slave_add, "1234567890", 10);
    printf("Slave read1[%s]\r\n", buf);

    /*  Master Multiple write <-> Slave buffer overflow (more data will be through away)
     *     > Master read2[12345]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_gets(buf, 5);
    I2cMaster_write(slave_add, "1234567890", 10);
    printf("Slave read2[%s]\r\n", buf);

    /*  Master single write <-> Slave single read
     *     > Master read3[1]
     */
    memset(buf, 0, sizeof(buf));
    I2cSlave_gets(buf, 1);
    I2cMaster_write(slave_add, "1234567890", 1);
    printf("Slave read3[%s]\r\n", buf);

#endif

#if 1
    slave_add = 0x12;
    uint8_t map[64] = "Good morning, how are you?";
    uint8_t regadd[2] =
    { 0, 0 };

    I2cSlave_setRegMap(map, 64, 2);

    /* Reg Map read test1 : oversize address
     *  > Map read1[Good morning, how are you?]
     */
    memset(buf, 0, sizeof(buf));
    regadd[0] = 1;
    regadd[1] = 0;
    I2cMaster_write(slave_add, regadd, 2);
    I2cMaster_read(slave_add, buf, 32);
    printf("Map read1[%s]\r\n", buf);

    /* Reg Map read test: offset test;
     *  > Map read2[how are you?]
     */
    memset(buf, 0, sizeof(buf));
    regadd[0] = 0;
    regadd[1] = 14;
    I2cMaster_write(slave_add, regadd, 2);
    I2cMaster_read(slave_add, buf, 32);
    printf("Map read2[%s]\r\n", buf);

    /* Reg Map write-read test:
     *  > Map wread[Good morning, XXXXXXXXXXXXXXXX]
     */
    memset(buf, 0, sizeof(buf));
    regadd[0] = 0;
    regadd[1] = 14;
    I2cMaster_wwrite(slave_add, regadd, 2, "XXXXXXXXXXXXXXXX ", 16);
    regadd[0] = 0;
    regadd[1] = 0;
    I2cMaster_wread(slave_add, regadd, 2, buf, 32);
    printf("Map wread[%s]\r\n", buf);

#endif
    return 1;
}
uint16_t App_Bist(void)
{
    //printf("\r\nEnter BIST (build-in self test) Mode");
    //Bist_SpiMaster_loop();
    //Bist_SpiMaster_To_SpiSlave();
    Bist_I2cMaster_To_I2cSlave();

    return 1;

}

