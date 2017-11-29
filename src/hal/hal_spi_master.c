/*
 * hal_spi.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "hal_spi_master.h"

/*****************************************************************************
 * [ SPI Master ] operation buffers.
 *****************************************************************************/
static uint8_t *Spi_M_RxPtr = 0;
static uint16_t Spi_M_RxLen = 0;

void SpiMaster_init(uint32_t Freq)
{
    //Initialize Master
    EUSCI_B_SPI_initMasterParam param =
    { 0 };
    param.selectClockSource = EUSCI_B_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = Freq;
    param.msbFirst = EUSCI_B_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT;
    param.clockPolarity = EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH;
    param.spiMode = EUSCI_B_SPI_4PIN_UCxSTE_ACTIVE_LOW;
    EUSCI_B_SPI_initMaster(EUSCI_B0_BASE, &param);

    //Enable SPI module
    EUSCI_B_SPI_enable(EUSCI_B0_BASE);

    //Clear & Enable USCI_B0 RX interrupt
    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
    EUSCI_B_SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);

    SpiMaster_initCsPins();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV, 4))
    {
    case 0:
        break;
    case 2: //Vector 2 - RXIFG
        if (Spi_M_RxLen == 0)
        {
            EUSCI_B_SPI_disableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);    //Disable RX Interrupt when finish.
        }
        else
        {
            *Spi_M_RxPtr++ = EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);
            Spi_M_RxLen--;
        }
        break;
    default:
        break;
    }
}

void SpiMaster_putc(uint8_t c)
{
    //USCI_B0 TX buffer ready?
    while (!EUSCI_B_SPI_getInterruptStatus(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT))
    {
        ;
    }

    //Transmit Data to slave
    EUSCI_B_SPI_transmitData(EUSCI_B0_BASE, c);
}

void SpiMaster_puts(uint8_t *s, uint16_t len)
{
    //Transfer multiple byte.
    while (len--)
    {
        SpiMaster_putc(*s++);
    }

    //Wait SPI finish transfer .
    while (EUSCI_B_SPI_isBusy(EUSCI_B0_BASE))
    {
        ;
    }
}

uint8_t SpiMaster_getc(void)
{
    return EUSCI_B_SPI_receiveData(EUSCI_B0_BASE);;
}

void SpiMaster_gets(uint8_t *s, uint16_t len)
{
    //Set receive buffer pointer
    Spi_M_RxPtr = s;
    Spi_M_RxLen = len;
    //Clear & Enable USCI_B0 RX interrupt
    EUSCI_B_SPI_clearInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
    EUSCI_B_SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_RECEIVE_INTERRUPT);
}

uint16_t SpiMaster_initCsPins()
{
    uint16_t i;
    for (i = 0; i < SPI_M_CSPIN_MAX; i++)
    {
        if(Spi_M_CsMatrix[i].bit == 0)  //Check if empty pin define.
        {
            return STATUS_SUCCESS;
        }

        if (Spi_M_CsMatrix[i].bit) //Set Valid pin to output mode.
        {
            GPIO_setOutputHighOnPin(Spi_M_CsMatrix[i].port, Spi_M_CsMatrix[i].pin);
            GPIO_setAsOutputPin(Spi_M_CsMatrix[i].port, Spi_M_CsMatrix[i].pin);
        }
    }
    return STATUS_SUCCESS;
}

uint16_t SpiMaster_setCsPins(uint16_t cs_sel)
{
    uint16_t i;
    for (i = 0; i < SPI_M_CSPIN_MAX; i++)
    {
        if(Spi_M_CsMatrix[i].bit == 0)  //Check if empty pin define.
        {
            return STATUS_SUCCESS;
        }

        if (cs_sel & Spi_M_CsMatrix[i].bit) //Set Valid pin
        {
            GPIO_setOutputLowOnPin(Spi_M_CsMatrix[i].port, Spi_M_CsMatrix[i].pin);
        }
        else
        {
            GPIO_setOutputHighOnPin(Spi_M_CsMatrix[i].port, Spi_M_CsMatrix[i].pin);
        }
    }

    return STATUS_SUCCESS;
}
