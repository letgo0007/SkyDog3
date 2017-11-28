/*
 * hal_gpio.c
 *
 *  Created on: Nov 27, 2017
 *      Author: nickyang
 */

#include "hal_gpio.h"

uint16_t Hal_Gpio_getBoardId(void)
{
    uint8_t bit0;
    uint8_t bit1;
    uint8_t id;

    bit0 = GPIO_getInputPinValue(HAL_GPIO_BOARD_ID0_PORT,HAL_GPIO_BOARD_ID0_PIN);
    bit1 = GPIO_getInputPinValue(HAL_GPIO_BOARD_ID1_PORT,HAL_GPIO_BOARD_ID1_PIN);
    id = (bit1<<1) + bit0;

    return id;
}

uint16_t Hal_Gpio_init(uint8_t board_id)
{
    /* Pin define for MSP430FR6989 launch pad (MSP-EXP430FR6989)
     *
     * |Pin|Name            |Function   |Sel
     *
     * **LFXT**
     * PJ.4 LFXIN           LFXIN       1
     * PJ.5 LFXOUT          LFXOUT      1
     *
     * **KEYs**
     * P1.1 KEY1/S1         GPIO_IN
     * P1.2 KEY2/S2         GPIO_IN
     *
     * **LEDs**
     * P1.0 LED1/LED_R      GPIO_OUT
     * P9.7 LED2/LED_G      GPIO_OUT
     *
     * **UCA0 = SPI Slave**
     * P4.2 SPI_S_MOSI      UCA0SIMO    1
     * P4.3 SPI_S_MISO      UCA0SOMI    1
     * P2.2 SPI_S_CLK       UCA0CLK     1
     * P2.3 SPI_S_CS        UCA0STE     1
     *
     * **UCA1 = UART**
     * P3.0 UART_RTS        NC
     * P3.1 UART_CTS        NC
     * P3.4 UART_TX         UCA1TXD     2
     * P3.5 UART_RX         UCA1RXD     2
     *
     * **UCB0 = SPI Master**
     * P1.4 SPI_M_CLK       UCB0CLK     1
     * P1.5 SPI_M_CS        UCB0STE     1
     * P1.6 SPI_M_MOSI      UCB0SIMO    1
     * P1.7 SPI_M_MISO      UCB0SOMI    1
     *
     * P8.4 SPI_M_CS0       GPIO_OUT
     * P8.5 SPI_M_CS1       GPIO_OUT
     * P8.6 SPI_M_CS2       GPIO_OUT
     * P8.7 SPI_M_CS3       GPIO_OUT
     *
     * **UCB1 = I2C Slave**
     * P4.0 I2C_S_SDA       UCB1SDA     1
     * P4.1 I2C_S_SCL       UCB1SCL     1
     *
     */

    //LFXT
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN4 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    //Keys
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_setAsInputPin(GPIO_PORT_P1, GPIO_PIN2);

    //LEDs
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setAsOutputPin(GPIO_PORT_P9, GPIO_PIN7);

    //SPI Slave
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    //UART
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN4, GPIO_SECONDARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN5, GPIO_SECONDARY_MODULE_FUNCTION);

    //SPI Master
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN5);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN6);
    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN7);

    //I2C Slave
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();
    return STATUS_SUCCESS;
}
