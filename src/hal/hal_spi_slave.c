/*
 * hal_spi_slave.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "hal_spi_slave.h"

/*****************************************************************************
 * [ SPI Master ] operation buffers.
 *****************************************************************************/
static uint8_t *Spi_S_TxPtr = 0;
static uint16_t Spi_S_TxLen = 0;

uint16_t SpiSlave_init(void)
{
    //Initialize slave to MSB first, inactive high clock polarity and 3 wire SPI
    EUSCI_A_SPI_initSlaveParam param =
    { 0 };
    param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    param.spiMode = EUSCI_A_SPI_3PIN;
    EUSCI_A_SPI_initSlave(EUSCI_A0_BASE, &param);

    //Enable SPI Module
    EUSCI_A_SPI_enable(EUSCI_A0_BASE);

    //Enable Transmit interrupt
    EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);
    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);

    return STATUS_SUCCESS;

}

//******************************************************************************
//
//This is the USCI_A0 interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch (__even_in_range(UCA0IV, 4))
    {
    case 0:     // Vector 0 - no interrupt
        break;
    case 2:     // Vector 2 - RXIFG
        break;
    case 4:     // Vector 4 - TXIFG
        if (Spi_S_TxLen == 0)
        {
            _NOP();
            //EUSCI_A_SPI_disableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_TRANSMIT_INTERRUPT);    //Disable TX Interrupt when finish.
        }
        else
        {
            UCA0TXBUF = *Spi_S_TxPtr++;
            Spi_S_TxLen--;
        }
        break;
    default:
        break;
    }
}

void SpiSlave_putc(uint8_t c)
{
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, c);
}

void SpiSlave_puts(uint8_t *s, uint16_t len)
{
    //Handle first byte
    UCA0TXBUF = *s++;

    //Set transmit buffer if ther are more than 1 byte
    if (len > 1)
    {
        Spi_S_TxPtr = s;
        Spi_S_TxLen = len - 1;
    }

    //Clear & Enable USCI_A0 TX interrupt
    //EUSCI_A_SPI_clearInterrupt(EUSCI_A0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
    //EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
}

uint8_t SpiSlave_getc(void)
{
    return EUSCI_A_SPI_receiveData(EUSCI_A0_BASE);
}

void SpiSlave_gets(uint8_t *s, uint16_t len)
{
    DMA_initParam param =
    { 0 };
    param.channelSelect = DMA_CHANNEL_0;
    param.transferModeSelect = DMA_TRANSFER_SINGLE;
    param.transferSize = len;
    //DMA trigger 14 = UCA0RX , 15 = UCA0TX, refer to datasheet P85
    param.triggerSourceSelect = DMA_TRIGGERSOURCE_14;
    param.transferUnitSelect = DMA_SIZE_SRCBYTE_DSTBYTE;
    param.triggerTypeSelect = DMA_TRIGGER_HIGH;
    DMA_init(&param);

    DMA_setSrcAddress(DMA_CHANNEL_0, EUSCI_A_SPI_getReceiveBufferAddress(EUSCI_A0_BASE), DMA_DIRECTION_UNCHANGED);
    DMA_setDstAddress(DMA_CHANNEL_0, &*s, DMA_DIRECTION_INCREMENT);

    //Enable transfers on DMA channel 0
    DMA_enableTransfers(DMA_CHANNEL_0);
}

uint16_t SpiSlave_getTxCnt(uint16_t total)
{
    return (total - Spi_S_TxLen);
}

uint16_t SpiSlave_getRxCnt(uint16_t total)
{
    if(DMA0CTL & DMAEN) //DMA not finish.
    {
        return (total - DMA0SZ);
    }
    else //DMA finished.
    {
        return DMA0SZ;
    }
}
