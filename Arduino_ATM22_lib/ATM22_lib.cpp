/*
  ATM22.cpp - Arduino library for ATM22 series absolute encoders by CUI Devices.
  Created by Simone Di Blasi, December 2020.
*/

#include "ATM22_lib.h"


ATM22::ATM22(uint8_t cs, uint8_t resolution) {
    digitalWrite(cs, HIGH);   //Get the CS line high which is the default inactive state
    _cs = cs;
    _resolution = resolution;
}

/*
 * This function gets the absolute position from the AMT22 encoder using the SPI bus. The AMT22 position includes 2 checkbits to use
 * for position verification. Both 12-bit and 14-bit encoders transfer position via two bytes, giving 16-bits regardless of resolution.
 * For 12-bit encoders the position is left-shifted two bits, leaving the right two bits as zeros. This gives the impression that the encoder
 * is actually sending 14-bits, when it is actually sending 12-bit values, where every number is multiplied by 4.
 * Error values are returned as 0xFFFF
 */
uint16_t ATM22::getPositionSPI(){
    uint16_t currentPosition;       //16-bit response from encoder
    bool binaryArray[16];           //after receiving the position we will populate this array and use it for calculating the checksum

    //get first byte which is the high byte, shift it 8 bits. don't release line for the first byte
    currentPosition = spiWriteRead(AMT22_NOP, false) << 8;

    //this is the time required between bytes as specified in the datasheet.
    delayMicroseconds(3);

    //OR the low byte with the currentPosition variable. release line after second byte
    currentPosition |= spiWriteRead(AMT22_NOP,true);

    //run through the 16 bits of position and put each bit into a slot in the array so we can do the checksum calculation
    for(int i = 0; i < 16; i++) binaryArray[i] = (0x01) & (currentPosition >> (i));

    //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent
    if ((binaryArray[15] == !(binaryArray[13] ^ binaryArray[11] ^ binaryArray[9] ^ binaryArray[7] ^ binaryArray[5] ^ binaryArray[3] ^ binaryArray[1])) && (binaryArray[14] == !(binaryArray[12] ^ binaryArray[10] ^ binaryArray[8] ^ binaryArray[6] ^ binaryArray[4] ^ binaryArray[2] ^ binaryArray[0])))
        currentPosition &= 0x3FFF; //we got back a good position, so just mask away the checkbits
    else
        currentPosition = 0xFFFF; //bad position

    //If the resolution is 12-bits, and wasn't 0xFFFF, then shift position, otherwise do nothing
    if ((_resolution == RES12) && (currentPosition != 0xFFFF)) currentPosition = currentPosition >> 2;

    return currentPosition;
}

/*
 * This function does the SPI transfer. sendByte is the byte to transmit.
 * Use releaseLine to let the spiWriteRead function know if it should release the chip select line after transfer.
 * The received data is returned.
 */
uint8_t ATM22::spiWriteRead(uint8_t sendByte, uint8_t releaseLine){
    //holder for the received over SPI
    uint8_t data;

    //set cs low, cs may already be low but there's no issue calling it again except for extra time
    setCSLine(LOW);

    //There is a minimum time requirement after CS goes low before data can be clocked out of the encoder.
    delayMicroseconds(3);

    //send the command
    data = SPI.transfer(sendByte);
    delayMicroseconds(3); //There is also a minimum time after clocking that CS should remain asserted before we release it
    setCSLine(releaseLine); //if releaseLine is high set it high else it stays low

    return data;
}

/*
 * This function sets the state of the SPI line.
 */
void ATM22::setCSLine(uint8_t csLine){
    digitalWrite(_cs, csLine);
}

/*
 * The AMT22 bus allows for extended commands. The first byte is 0x00 like a normal position transfer, but the second byte is the command.
 */
void ATM22::setZeroSPI(){
    spiWriteRead(AMT22_NOP, false);

    //this is the time required between bytes as specified in the datasheet.
    delayMicroseconds(3);

    spiWriteRead(AMT22_ZERO, true);
    delay(250); //250 second delay to allow the encoder to reset
}

/*
 * The AMT22 bus allows for extended commands. The first byte is 0x00 like a normal position transfer, but the second byte is the command.
 */
void ATM22::resetAMT22(){
    spiWriteRead(AMT22_NOP, false);

    //this is the time required between bytes as specified in the datasheet.
    delayMicroseconds(3);

    spiWriteRead(AMT22_RESET, true);

    delay(250); //250 second delay to allow the encoder to start back up
}

/*
 * This function sets the resolution of the encoder.
 */
void ATM22::setResolution(uint8_t resolution) {
    _resolution = resolution;
}

/*
 * This function is not related to the ATM22 class. It allows to set up communication via SPI.
 * It must be performed in the setup section of the Arduino main.
 */
void setUpSPI(uint8_t mosi, uint8_t miso, uint8_t sclk, uint8_t clk_divider){
    pinMode(sclk, OUTPUT);
    pinMode(mosi, OUTPUT);
    pinMode(miso, INPUT);
    SPI.setClockDivider(clk_divider);
    SPI.begin();
}
