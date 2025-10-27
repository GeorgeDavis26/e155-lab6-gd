/**
    Main Header: Contains general defines and selected portions of CMSIS files
    @file main.h
    @author George DAvis
    @version 10/27/2025

    Main header file for Lab6 IOT temperature sensor funciton.
    Please reference the DS1772 data sheet and the STM32L432KC for pin mapping
*/

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"

#define DEAFULT_RES 8

#define LED_PIN PB3 // LED pin for blinking on Port B pin 3
#define BUFF_LEN 32

#define GPIO_COPI PB5 //6 MOSI 
#define GPIO_CIPO PA6 //5 MISO 
#define GPIO_SCK PA5  //4 SCK
#define GPIO_CS PA7   //3 CS

#endif // MAIN_H