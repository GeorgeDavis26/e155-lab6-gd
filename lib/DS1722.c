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

void configDS(void){
    digitalWrite(GPIO_CS, PIO_LOW);
    // set bit resolution
    spiSendRecieve(0x80); //acces config register
    if (res == 12){spiSendRecieve(TWELVEBITRES);};
    else if (res == 11){spiSendRecieve(ELEVENBITRES);};
    else if (res == 10){spiSendRecieve(TENBITRES);};
    else if (res == 9){spiSendRecieve(NINEBITRES);};
    else {spiSendRecieve(EIGHTBITRES);}; // default to 8-bit res
};

void readTemp(void){
    spiSendRecieve(0x01); 
    int16_t MSb = spiSendRecieve(DUMMY); 
    spiSendRecieve(0x02); 
    int16_t LSb = spiSendRecieve(DUMMY);

    int16_t temp_bits = MSb << 8 | LSb;

    // Convert MSb and LSb to Decimal value
    if (res == 12)      {temp = TWELVEBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));};
    else if (res == 11) {temp = ELEVENBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));};
    else if (res == 10) {temp = TENBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));};
    else if (res == 9)  {temp = NINEBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));};
    else                {temp = EIGHTBIT_RES_SCALAR * (temp_bits >> (TEMPBIT_WIDTH-res));};
};

