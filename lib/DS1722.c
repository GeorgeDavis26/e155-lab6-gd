// DS1722.c
// George Davis
// gdavis@hmc.edu
// 10/22/25
// TODO: <SHORT DESCRIPTION OF WHAT THIS FILE DOES>

#include "STM32L432KC.h"
#include "STM32L432KC_SPI.h"

#include "DS1722.h"
#include "main.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"


// SERMODE connected to Vdd to enable SPI

// The four pins are the SDO (Serial Data Out), SDI (Serial Data In), CE (Chip Enable), and SCLK (Serial Clock)

// global variable int res (user defined) and temp

void configDS(int res){
    digitalWrite(GPIO_CS, PIO_LOW);
    // set bit resolution
    spiSendReceive(CONFIG_WRITE); //acces config register
    if (res == 12){spiSendReceive(TWELVEBIT_RES);}
    else if (res == 11){spiSendReceive(ELEVENBIT_RES);}
    else if (res == 10){spiSendReceive(TENBIT_RES);}
    else if (res == 9){spiSendReceive(NINEBIT_RES);}
    else {spiSendReceive(EIGHTBIT_RES);} // default to 8-bit res
};

int readTemp(int res){
    int temp = 0;
    spiSendReceive(LSB_READ); 
    int16_t MSb = spiSendReceive(DUMMY); 
    spiSendReceive(MSB_READ); 
    int16_t LSb = spiSendReceive(DUMMY);
    int16_t temp_bits = MSb << 8 | LSb;
    // Convert MSb and LSb to Decimal value
    if (res == 12)      {temp = TWELVEBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));}
    else if (res == 11) {temp = ELEVENBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));}
    else if (res == 10) {temp = TENBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));}
    else if (res == 9)  {temp = NINEBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));}
    else                {temp = EIGHTBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));}
    return temp;
};

