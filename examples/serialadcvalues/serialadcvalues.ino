/*
MCP3008 ADC Over Serial

Purpose: Read values from an MCP3008 connected through SPI and report as serial values

Author: Jason Campbell

Changelog:
5/18/14 Added hires option
5/17/14 Cleaned it up, changed to scale to 0-255
5/17/14 Initial example program

Notes: channel and slaveSelectPin can be integers, 
      but consider using bytes to save memory/flash 
      if it works for your application
      
      The MCP3008 sends 10 bit values back (0-1023).
      By default this driver drops the two least significant
      bits to return an 8 bit value (0-255).  If you want
      to get all 10 bits add a #define MCP3008HIRES in the 
      MCP3008.h file and when you store the results use and 'word'
      instead of a 'byte.'  This will use slightly more memory, 
      but may be better for precision measurement applications.

USAGE:
======
Mcp3008 myAdc(slaveSelectPin); //Call constructor
myAdc.read(channel); //Channel is 0-7

*/

#include "SPI.h" //This driver depends on the SPI library for hardware fast SPI
#include "Mcp3008.h"

// Use pin for slave select and identify the hardware SPI slave select
const byte slaveSelectPin = 9;
const byte HWslaveSelectPin = 10;

Mcp3008 myAdc(slaveSelectPin); //Constructor for the adc driver

void setup() {
  //If there is another device on the hardware SPI slave select pin, tell it to shut up
  pinMode(HWslaveSelectPin, OUTPUT); 
  digitalWrite(HWslaveSelectPin, HIGH);
  
  //Start serial so we can see what the pot is reading
  Serial.begin(38400);
}

void loop() {
  //Read in adc values and report them over serial
  for (byte channel = 0; channel <7; channel ++){

    //Read the ADC
    byte readValue = myAdc.read(channel); 
    
    //Send formatted serial data back 
    Serial.print("channel: ");
    Serial.print(channel);
    Serial.print("\tvalue:");
    Serial.print(readValue);
    Serial.print("\n"); //send newline character
    
    delay(500); //Not really needed, but makes it easier to read the serial data
  }
}

