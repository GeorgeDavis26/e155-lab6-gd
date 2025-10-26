// STM32L432KC_SPI.c
// George Davis
// gdavis@hmc.edu
// 10/22/25
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#include "STM32L432KC.h"

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"
#include "main.h"

/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
 *          1: data changed on leading edge of clk and captured on next edge)
 * Refer to the datasheet for more low-level details. */ 
void initSPI(int br, int cpol, int cpha) {
    // Write proper GPIO registers: Configure GPIO for MOSI, MISO and SCK pins.

    // Enable GPIOA and GPIOB clk 
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // Enable SPI1 clk
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    // Initially assigning SPI pins GPIO_COPI
    pinMode(GPIO_CIPO, GPIO_ALT);  // Controller in Peripheral Out (CIPO)
    pinMode(GPIO_COPI, GPIO_ALT);  // Controller out Peripheral in (COPI)
    pinMode(GPIO_SCK, GPIO_ALT);   // Serial Clock
    pinMode(GPIO_CS, GPIO_OUTPUT); // Chip Select

    // AF05 for SPI alternate functions
    //PB5: MOSI
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);
    //PA6: MISO
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5);
    //PA5: SCK
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);

    // Configure the serial clock baud rate using the BR[2:0] bits
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_BR, br);

    //GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5);

    // Configure the CPOL and CPHA bits combination to define one of the four
    // relationships between the data transfer and the serial clock
    SPI1->CR1 &= ~SPI_CR1_CPOL; //Clock polarity control - SCK pin has a low-level idle state
    SPI1->CR1 &= ~SPI_CR1_CPHA; //the first edge on the SCK pin captures the first data bit transacted
    
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
    SPI1->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);

    // Configure the LSBFIRST bit to define the frame format
    // ds1772 specifies MSB first
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;

    // Configure SSM and SSI 
    // Peripheral Select (NSS - Slave Select) Pin Management
    //SPI1->CR1 &= ~SPI_CR1_SSM;

    // Configure the MSTR bit 
    SPI1->CR1 |= (SPI_CR1_MSTR);

    // Configure the DS[3:0] bits to select the data length for the transfer.
    SPI1->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); //set data fram bit size to 9

    // Configure SSOE
    SPI1->CR2 |= SPI_CR2_SSOE;

    // Configure the FRXTH bit
    SPI1->CR2 |= SPI_CR2_FRXTH;

    // Enable SPI
    SPI1->CR1 |= (SPI_CR1_SPE); 
}

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(char send) {
    // send: the character to send over SPI
    // wait for TX (Transmit Buffer) to be empty 
    while(!(SPI1->SR & SPI_SR_TXE)); 
    *(volatile char *) &SPI1->DR = send;
    // wait for RX (Recieve Buffer) to be empty 
    while(!(SPI1->SR & SPI_SR_RXNE));
    // return: the character received over SPI
    char recieve = (volatile char) SPI1->DR;
    return recieve;
}