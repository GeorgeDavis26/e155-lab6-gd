// DS1722.h
// George Davis
// gdavis@hmc.edu
// 10/22/25
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#ifndef DS1722_H
#define DS1722_H

#include <stdint.h>
#include <stm32l432xx.h>

#include "STM32L432KC_SPI.h"

#define DUMMY 0x00

#define LSB_READ 0x01
#define MSB_READ 0x02

#define CONFIG_READ 0x00
#define CONFIG_WRITE 0x80

#define TWELVEBIT_RES 0b11101000
#define ELEVENBIT_RES 0b11100110
#define TENBIT_RES 0b11100100
#define NINEBIT_RES 0b11100010
#define EIGHTBIT_RES 0b11100000

#define TEMPBIT_WIDTH 16

#define TWELVEBIT_RES_SCALAR 0.0625
#define ELEVENBIT_RES_SCALAR 0.125
#define TENBIT_RES_SCALAR 0.25
#define NINEBIT_RES_SCALAR 0.5
#define EIGHTBIT_RES_SCALAR 1

void configDS(int res);
int readTemp(int res);

#endif