#include "MCP3008.h"

Mcp3008::Mcp3008(byte adcChipSelectPin) {

  //Set the chip select pin to output and pull it high (disable)
  _adcChipSelectPin = adcChipSelectPin;
  pinMode(_adcChipSelectPin, OUTPUT);
  digitalWrite(_adcChipSelectPin, HIGH);
  
  //Start SPI
  SPI.begin();
  
}

#ifdef MCP3008HIRES
int Mcp3008::read(byte adcNum) {
#else
byte Mcp3008::read(byte adcNum) {
#endif
    
  //Set up the byte to transfer to the ADC including the single/differential (S/D) bit and the address
  byte _commandout = (B1000 | adcNum) << 4; // set the S/D bit to 1 (single), combine with address, shift bits to beginning of byte

  //Pull chip select low to enable communication with the device
  digitalWrite(_adcChipSelectPin, LOW);

#ifdef MCP3008HIRES
    //Send and receive data from the device
    SPI.transfer(B1); // send the start bit with 7 leading zeroes.  SPI library takes care of formatting data
    byte _adcout1 = SPI.transfer(_commandout); // send single/diff and address, receive first part of the adc value
    byte _adcout2 = SPI.transfer(B0); // receive second part of the adc value.  chip doesn't care what you send
    
    //Pull chip select high to disable communication with the device and open the SPI bus
    digitalWrite(_adcChipSelectPin, HIGH);
    
    //Format and return the value

    return ((_adcout1 & B00000011)<<8) | _adcout2;
#else
    //Send and receive data from the device
    SPI.transfer(B1); // send the start bit with 7 leading zeroes.  SPI library takes care of formatting data
    byte _adcout = (SPI.transfer(_commandout) & B00000011) << 6; // send single/diff and address, receive first part of the adc value
    _adcout = _adcout | ((SPI.transfer(B0) >> 2) & B00111111); // receive second part of the adc value.  chip doesn't care what you send
    
    //Pull chip select high to disable communication with the device and open the SPI bus
    digitalWrite(_adcChipSelectPin, HIGH);
    
    //Return the value
    return _adcout;
#endif
}

