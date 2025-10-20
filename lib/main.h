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

#define GPIO_COPI PA12 //6 MOSI (RX)
#define GPIO_CIPO PA11 //5 MISO (TX)
#define GPIO_SCK PA5  //4 SCK
#define GPIO_CS PA7   //3 CS

#endif // MAIN_H