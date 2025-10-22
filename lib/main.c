/*
File: Lab_6_JHB.c
Author: Josh Brake
Email: jbrake@hmc.edu
Date: 9/14/19
*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

//GLOBAL VARIABLE

volatile int res = 8;
char sent = 0xAA;
float temp_C = 15.1234;

/////////////////////////////////////////////////////////////////
// Provided Constants and Functions
/////////////////////////////////////////////////////////////////

//Defining the web page in two chunks: everything before the current time, and everything after the current time
char* webpageStart = "<!DOCTYPE html><html><head><title>E155 Web Server Demo Webpage</title>\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
	</head>\
	<body><h1>E155 Web Server Demo Webpage</h1>";
char* ledStr = "<p>LED Control:</p><form action=\"ledon\"><input type=\"submit\" value=\"Turn the LED on!\"></form>\
	<form action=\"ledoff\"><input type=\"submit\" value=\"Turn the LED off!\"></form>";


char* resStr = "<p>Temperature Resolution Control:</p><form action=\"8bit_res\"><input type=\"submit\" value=\"8 bit Resolution (1.0 &deg;C)\"></form>\
                <form action=\"9bit_res\"><input type=\"submit\" value=\"9 bit Resolution (0.5 &deg;C)\"></form>\
                <form action=\"10bit_res\"><input type=\"submit\" value=\"10 bit Resolution (0.25 &deg;C)\"></form>\
                <form action=\"11bit_res\"><input type=\"submit\" value=\"11 bit Resolution (0.125 &deg;C)\"></form>\
                <form action=\"12bit_res\"><input type=\"submit\" value=\"12 bit Resolution (0.0625 &deg;C)\"></form>";

  char* webpageEnd   = "</body></html>";

//determines whether a given character sequence is in a char array request, returning 1 if present, -1 if not present
int inString(char request[], char des[]) {
	if (strstr(request, des) != NULL) {return 1;}
	return -1;
}

int updateLEDStatus(char request[])
{
	int led_status = 0;
	// The request has been received. now process to determine whether to turn the LED on or off
	if (inString(request, "ledoff")==1) {
		digitalWrite(LED_PIN, PIO_LOW);
		led_status = 0;
	}
	else if (inString(request, "ledon")==1) {
		digitalWrite(LED_PIN, PIO_HIGH);
		led_status = 1;
	}

	return led_status;
}

int updateRESStatus(char request[])
{
	int res_status = 0;
	// The request has been received. now process to determine resolution value
	if (inString(request, "8bit_res")==1) {res_status = 10;}
        else if (inString(request, "9bit_res")==1) {res_status = 9;}
	else if (inString(request, "10bit_res")==1) {res_status = 10;}
	else if (inString(request, "11bit_res")==1) {res_status = 11;}
	else if (inString(request, "12bit_res")==1) {res_status = 12;}
	return res_status;
}

/////////////////////////////////////////////////////////////////
// Solution Functions
/////////////////////////////////////////////////////////////////


// int main(void) {
//   configureFlash();
//   configureClock();

//   gpioEnable(GPIO_PORT_A);
//   gpioEnable(GPIO_PORT_B);
//   gpioEnable(GPIO_PORT_C);

//   pinMode(PB3, GPIO_OUTPUT);
  
//   RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
//   initTIM(TIM15);
//   initSPI(7, 0, 0);
 
//   while(1){
//     for(int i=0; i<8; i++){
//       digitalWrite(GPIO_CS, PIO_LOW);
//       spiSendReceive(sent);
// //      delay_millis(TIM15, 1);
//       digitalWrite(GPIO_CS, PIO_HIGH);  
//     }
//   }

// }

int main(void) {
 configureFlash();
 configureClock();

 gpioEnable(GPIO_PORT_A);
 gpioEnable(GPIO_PORT_B);
 gpioEnable(GPIO_PORT_C);

 pinMode(PB3, GPIO_OUTPUT);
  
 RCC->APB2ENR |= (RCC_APB2ENR_TIM15EN);
 initTIM(TIM15);
  
 USART_TypeDef * USART = initUSART(USART1_ID, 125000);


/* Enables the SPI peripheral and intializes its clock speed (baud rate), polarity, and phase.
*    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
*    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state is logical 1).
*    -- cpha: clock phase (0: data captured on leading edge of clk and changed on next edge, 
*          1: data changed on leading edge of clk and captured on next edge)
* Refer to the datasheet for more low-level details. */ 

 initSPI(7, 0, 0);
 configDS(res);

 while(1) {
   /* Wait for ESP8266 to send a request.
   Requests take the form of '/REQ:<tag>\n', with TAG begin <= 10 characters.
   Therefore the request[] array must be able to contain 18 characters.
   */

   // Receive web request from the ESP
   char request[BUFF_LEN] = "                  "; // initialize to known value
   int charIndex = 0;
  
   // Keep going until you get end of line character
   while(inString(request, "\n") == -1) {
     // Wait for a complete request to be transmitted before processing
     while(!(USART->ISR & USART_ISR_RXNE));
     request[charIndex++] = readChar(USART);
   }

   // TODO: Add SPI code here for reading temperature
   //temp_C = 1 readTemp(res);
   // Update string with current LED state
  
   int led_status = updateLEDStatus(request);
   int res = updateRESStatus(request);

   char ledStatusStr[20];
   if (led_status == 1)
     sprintf(ledStatusStr,"LED is on!");
   else if (led_status == 0)
     sprintf(ledStatusStr,"LED is off!");

   char tempStatusStr[20];
   //sprintf(buffer, "The value of pi is %.2f.", pi);
   sprintf(tempStatusStr,"%f \xB0""C", temp_C);

   // finally, transmit the webpage over UART
   sendString(USART, webpageStart); // webpage header code
   sendString(USART, ledStr); // button for controlling LED
   sendString(USART, resStr); // buttons for controlling temp bit resolution

   sendString(USART, "<h2>LED Status</h2>");
   sendString(USART, "<p>");
   sendString(USART, ledStatusStr);
   sendString(USART, "</p>");

   sendString(USART, "<h2>Temperature Reading</h2>");
   sendString(USART, "<p>");
   sendString(USART, tempStatusStr);
   sendString(USART, "</p>");

  
   sendString(USART, webpageEnd);
 }
}

