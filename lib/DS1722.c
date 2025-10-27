// DS1722.c
// George Davis
// gdavis@hmc.edu
// 10/22/25
// The C code to configure the DS1772 temperature module and decode the LSb and MSb into a float to be printed on the IOT website

#include "STM32L432KC.h"
#include "STM32L432KC_SPI.h"

#include "DS1722.h"
#include "main.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"


// SERMODE connected to Vdd to enable SPI

// The four pins are the SDO (Serial Data Out), SDI (Serial Data In), CE (Chip Enable), and SCLK (Serial Clock)

// global variable int res (user defined) and temp

// configDS takes in a resolution integer and writes to the config register on the DS to
// set the IC to that given resolution. See the header file for macro definitions
void configDS(int res){
    // set bit resolution
    digitalWrite(GPIO_CS, PIO_HIGH);
    spiSendReceive(CONFIG_WRITE); //acces config register
    
    if (res == 12){spiSendReceive(TWELVEBIT_RES);}
    else if (res == 11){spiSendReceive(ELEVENBIT_RES);}
    else if (res == 10){spiSendReceive(TENBIT_RES);}
    else if (res == 9){spiSendReceive(NINEBIT_RES);}
    else {spiSendReceive(EIGHTBIT_RES);} // default to 8-bit res
    digitalWrite(GPIO_CS, PIO_LOW);
};


// Twos compliment detects if a char is negative and sends a flag if it is
int twosComp(int MSb_in){
    int sign = 0;
    if (MSb_in & (1 << 7)){sign = 1;}
    else sign = 0;
    return sign;
}


//readTemp returns a float corrosponding to the
//LSb and MSb read off of the SPI interface
float readTemp(int res){
    float temp_C;
    
    digitalWrite(GPIO_CS, PIO_HIGH);
    spiSendReceive(MSB_READ); 
    unsigned char MSb_tc = spiSendReceive(CONFIG_READ);
    digitalWrite(GPIO_CS, PIO_LOW); 

    digitalWrite(GPIO_CS, PIO_HIGH);
    spiSendReceive(LSB_READ); 
    unsigned char LSb = spiSendReceive(CONFIG_READ);
    digitalWrite(GPIO_CS, PIO_LOW);  

    printf("MSb is: %d ", MSb_tc);
    printf("LSb is: %d", LSb);

    int sign = twosComp(MSb_tc);
    float temp_bits = 0;
    if (sign){temp_bits = ((MSb_tc - TWOS_COMP_CONVERT) << TEMP_SB_WIDTH | LSb);}
    else {temp_bits = MSb_tc << TEMP_SB_WIDTH | LSb;}

    temp_C = temp_bits  / (TEMP_SCALAR);
    //printf("temp_bits: %f ", temp_bits);
    //printf("temp %f \n",temp_C);
    return temp_C;
};

