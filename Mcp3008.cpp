/*
MCP3008 Library Copyright 2014 by Jason Campbell

Revisions:
    infomaniac50 - Sun Mar  8 12:12:04 CDT 2015
    On case-sensitive filesystems include statments must exactly match the filename.
 */

#include "Mcp3008.h"

Mcp3008::Mcp3008(byte adcChipSelectPin) {

  //Set the chip select pin to output and pull it high (disable)
  _adcChipSelectPin = adcChipSelectPin;
  pinMode(_adcChipSelectPin, OUTPUT);
  digitalWrite(_adcChipSelectPin, HIGH);
  
  //Start SPI
  SPI.begin();
  
}


#ifdef MCP3008HIRES
    
word Mcp3008::read(byte adcNum) {
    
    byte _commandout = (B00001000 | adcNum) << 4; // set the S/D bit to 1 (single), combine with address
    word _adcout;
    
    //Pull chip select low
    digitalWrite(_adcChipSelectPin, LOW);
    
    SPI.transfer(B1); // send the start bit with 7 leading zeroes.
    
    _adcout = SPI.transfer(_commandout); // send commandout, receive first two bits
    _adcout &= B00000011; //only want the last two bits.  other data is garbage
    _adcout <<= 8;  //make way for the next incoming byte
    
    _adcout &= SPI.transfer(B0); // receive second part of the adc value.  chip doesn't care what you send
    
    //Pull chip select high
    digitalWrite(_adcChipSelectPin, HIGH);
    
    return _adcout;

#else
    
byte Mcp3008::read(byte adcNum) {

    byte _commandout = B11000000 | (adcNum<<3);  // start bit, S/D bit, and address
    byte _adcout;

    //Pull chip select low
    digitalWrite(_adcChipSelectPin, LOW);
    
    _adcout = SPI.transfer(_commandout);
    _adcout &= B00000001; // we only get the first incoming bit for now.  the rest is garbage
    _adcout <<= 7;
    _adcout |= ((SPI.transfer(B0) >> 1) & B01111111); // get the rest of the bits. drop the last bit
    
    //Pull chip select high
    digitalWrite(_adcChipSelectPin, HIGH);
    
    //Return the value
    return _adcout;

#endif

}

