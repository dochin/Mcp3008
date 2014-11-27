/*
MCP3008 ADC Driver

Purpose: Read controls from an MCP3008 using hardware SPI
Author: Jason Campbell

Changelog:
5/18/14 Added hires (10-bit results) option if MCP3008HIRES macro is defined
5/17/14 converted to library, used more efficient types
5/13/14 rewritten using SPI library instead of bitbanging

Notes: Hardware SPI pins needed are 13:Clock, 12: MISO, 11:MOSI
      the adcChipSelectPin is read in to the constructor
 
        If you want to get 10 bit values back as an integer
        use #define MCP3008HIRES in this file

*/

#ifndef Mcp3008_h
#define Mcp3008_h

#include "Arduino.h"
#include "SPI.h"

class Mcp3008 {
  public:
    Mcp3008(byte adcChipSelectPin);
#ifdef MCP3008HIRES
    word read(byte adcNum);
#else
    byte read(byte adcNum);
#endif
  private:
    byte _adcChipSelectPin;
};

#endif
