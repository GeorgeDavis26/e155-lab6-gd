/**
    Main Header: Contains general defines and selected portions of CMSIS files
    @file main.h
    @author Josh Brake
    @version 1.0 10/7/2020
*/

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"

#define LED_PIN PB3 // LED pin for blinking on Port B pin 3
#define BUFF_LEN 32

#define GPIO_COPI PB0 //6 MOSI (RX)
#define GPIO_CIPO PB7 //5 MISO (TX)
#define GPIO_SCK PB6  //4 SCK
#define GPIO_CE PA7   //3 CS

#define BR      // br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
#define CPOL 1  // cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
#define CPHA 0  // cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
                //                    1: data changed on leading edge of clk and captured on next edge)

#endif // MAIN_H