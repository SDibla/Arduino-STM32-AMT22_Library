/*
  ATM22.h - Arduino library for ATM22 series absolute encoders by CUI Devices.
  Created by Simone Di Blasi, December 2020.
*/

#ifndef ATM22_LIB_LIBRARY_H
#define ATM22_LIB_LIBRARY_H

#include "Arduino.h"
#include <SPI.h>

/* SPI commands */
#define AMT22_NOP       0x00
#define AMT22_RESET     0x60
#define AMT22_ZERO      0x70

#define RES12           12

class ATM22
{
public:
    ATM22(uint8_t cs, uint8_t resolution);
    uint16_t getPositionSPI();
    void setZeroSPI();
    void resetAMT22();
    void setResolution(uint8_t resolution);


private:
    uint8_t _cs, _resolution;
    uint8_t spiWriteRead(uint8_t sendByte, uint8_t releaseLine);
    void setCSLine (uint8_t csLine);

};

void setUpSPI(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t clk_divider);

#endif //ATM22_LIB_LIBRARY_H
