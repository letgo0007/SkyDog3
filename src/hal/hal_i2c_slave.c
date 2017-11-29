/*
 * hal_i2c_slave.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "hal_i2c_slave.h"

/*I2c Slave 0 operation as command line style*/
#define I2C_S0_ADD      0x10

static uint8_t *I2c_S0_RxPtr = 0;
static uint16_t I2c_S0_RxLen = 0;
static uint16_t I2c_S0_RxCnt = 0;

static uint8_t *I2c_S0_TxPtr = 0;
static uint16_t I2c_S0_TxLen = 0;
static uint16_t I2c_S0_TxCnt = 0;

/*I2c Slave 1 operation as register map style*/

#define I2C_S1_ADD      0x12

static uint8_t *I2c_S1_MapPtr = 0;
static uint16_t I2c_S1_MapSize = 0;
static uint8_t I2c_S1_AddrByte = 0;
static uint16_t I2c_S1_Addr = 0;
static uint16_t I2c_S1_RxCnt = 0;
static uint16_t I2c_S1_TxCnt = 0;

uint16_t I2cSlave_init()
{
    // eUSCI configuration
    EUSCI_B_I2C_initSlaveParam param =
    { 0 };
    param.slaveAddress = I2C_S0_ADD;
    param.slaveAddressOffset = EUSCI_B_I2C_OWN_ADDRESS_OFFSET0;
    param.slaveOwnAddressEnable = EUSCI_B_I2C_OWN_ADDRESS_ENABLE;
    EUSCI_B_I2C_initSlave(EUSCI_B1_BASE, &param);

    //param.slaveAddress = I2C_S1_ADD;
   // param.slaveAddressOffset = EUSCI_B_I2C_OWN_ADDRESS_OFFSET1;
    //param.slaveOwnAddressEnable = EUSCI_B_I2C_OWN_ADDRESS_ENABLE;
    //EUSCI_B_I2C_initSlave(EUSCI_B1_BASE, &param);

    //Enable I2C Slave
    EUSCI_B_I2C_enable(EUSCI_B1_BASE);

    //Trun on interrupt
    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_START_INTERRUPT);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_START_INTERRUPT);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_STOP_INTERRUPT);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_STOP_INTERRUPT);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT0);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT1);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_RECEIVE_INTERRUPT1);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT1);
    EUSCI_B_I2C_enableInterrupt(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT1);

    return 1;
}

//------------------------------------------------------------------------------
// The USCI_B1 data ISR RX vector is used to move received data from the I2C
// master to the MSP430 memory.
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// The USCI_B1 state ISR TX vector is used to wake up the CPU from LPM0 in order
// to process the received data in the main program. LPM0 is only exit in case
// of a (re-)start or stop condition when actual data was received.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B1_VECTOR)))
#endif
void USCIB1_ISR(void)
{
    switch (__even_in_range(UCB1IV, 0x1E))
    {
    case 0x00:
        break;                     // Vector 0: No interrupts break;
    case 0x02:
        break;                     // Vector 2: ALIFG break;
    case 0x04:
        break;                     // Vector 4: NACKIFG break;
    case 0x06:                     // Vector 6: STTIFG break;
    {
        I2c_S0_RxCnt = 0;
        I2c_S0_TxCnt = 0;
        I2c_S1_RxCnt = 0;
        I2c_S1_TxCnt = 0;
        break;
    }
    case 0x08:                     // Vector 8: STPIFG break;
    {
        break;
    }
    case 0x0a:
        break;                     // Vector 10: RXIFG3 break;
    case 0x0c:
        break;                     // Vector 14: TXIFG3 break;
    case 0x0e:
        break;                     // Vector 16: RXIFG2 break;
    case 0x10:
        break;                     // Vector 18: TXIFG2 break;
    case 0x12:                     // Vector 20: RXIFG1 break;
    {
        if(I2c_S1_RxCnt < I2c_S1_AddrByte)
        {
            I2c_S1_Addr = I2c_S1_Addr << 8;
            I2c_S1_Addr = I2c_S1_Addr + UCB1RXBUF;
        }
        else
        {
            if(I2c_S1_Addr < I2c_S1_MapSize)
            {
                I2c_S1_MapPtr[I2c_S1_Addr + I2c_S1_RxCnt] = UCB1RXBUF;
            }
        }

        I2c_S1_RxCnt ++;
        break;
    }

    case 0x14:                     // Vector 22: TXIFG1 break;
    {
        UCB1TXBUF = I2c_S1_MapPtr[I2c_S1_Addr + I2c_S1_TxCnt];
        I2c_S1_TxCnt ++;
        break;
    }
    case 0x16:                     // Vector 24: RXIFG0 break;
    {
        if (I2c_S0_RxLen == 0)
        {
            ;
        }
        else
        {
            I2c_S0_RxPtr[I2c_S0_RxCnt] = UCB1RXBUF;
            I2c_S0_RxCnt++;
            I2c_S0_RxLen--;
        }

        break;
    }
    case 0x18:                      // Vector 26: TXIFG0 break;
    {
        if (I2c_S0_TxLen == 0)
        {
            ;
        }
        else
        {
            UCB1TXBUF = I2c_S0_TxPtr[I2c_S0_RxCnt];
            I2c_S0_TxCnt++;
            I2c_S0_TxLen--;
        }
        break;
    }

    case 0x1a:
        break;                     // Vector 28: BCNTIFG break;
    case 0x1c:
        break;                     // Vector 30: clock low timeout break;
    case 0x1e:
        break;                     // Vector 32: 9th bit break;
    default:
        break;
    }

}

void I2cSlave_putc(uint8_t c)
{
    EUSCI_B_I2C_slavePutData(EUSCI_B1_BASE, c);
}

void I2cSlave_puts(uint8_t *s, uint16_t len)
{
    EUSCI_B_I2C_slavePutData(EUSCI_B1_BASE, *s++);

    if(len > 1)
    {
        I2c_S0_TxPtr = s;
        I2c_S0_TxLen = len - 1;
        I2c_S0_TxCnt = 0;
    }
}

uint8_t I2cSlave_getc(void)
{
    return EUSCI_B_I2C_slaveGetData(EUSCI_B1_BASE);
}

void I2cSlave_gets(uint8_t *s, uint16_t len)
{
    I2c_S0_RxPtr = s;
    I2c_S0_RxLen = len;
    I2c_S0_RxCnt = 0;
}

void I2cSlave_setRegMap(uint8_t *MapPtr, uint16_t MapSize, uint8_t AddrByte)
{
    I2c_S1_MapPtr = MapPtr;
    I2c_S1_MapSize = MapSize;
    I2c_S1_AddrByte = AddrByte;
}
