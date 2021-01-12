/*
 * AMT22_Arduino_Sample_Code.ino
 * Author: Simone Di Blasi
 * Date: December, 2020
 * 
 * This sample code can be used with the Arduino Uno to control the AMT22 encoder.
 * It uses SPI to control the encoder and the the Arduino UART to report back to the PC
 * via the Arduino Serial Monitor.
 * 
 * After uploading code to Arduino Uno open the open the Serial Monitor under the Tools 
 * menu and set the baud rate to 115200 to view the serial stream the position from the AMT22.
 * 
 * Arduino Pin Connections
 * SPI Chip Select Enc:     Pin  2
 * SPI MOSI                 Pin 11
 * SPI MISO                 Pin 12
 * SPI SCLK:                Pin 13
 * 
 * 
 * AMT22 Pin Connections
 * Vdd (5V):                Pin  1
 * SPI SCLK:                Pin  2
 * SPI MOSI:                Pin  3
 * GND:                     Pin  4
 * SPI MISO:                Pin  5
 * SPI Chip Select:         Pin  6
 * 
 * This code takes inspiration and is based on the code shared by CUI Devices which authorizes its diffusion and modification.
 */

#include "AMT22_lib.h"

/* Serial rates for UART */
#define BAUDRATE        115200


/* Define special ascii characters */
#define NEWLINE         0x0A
#define TAB             0x09

/* We will use these define macros so we can write code once compatible with 12 or 14 bit encoders */
#define RES12           12
#define RES14           14

/* SPI pins */
#define ENC             2
#define SPI_MOSI        11
#define SPI_MISO        12
#define SPI_SCLK        13

/*Object creation*/
AMT22* Encoder;

void setup(){
  //Initialize the UART serial connection for debugging
  Serial.begin(BAUDRATE);
  //Initialize the SPI communication
  setUpSPI(SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CLOCK_DIV32); //This operation only needs to be done once, not once for each Encoder object.
  //Initialize the encoder object with necessary parameters 
  Encoder = new AMT22(ENC,RES14);
}

void loop() 
{
  //create a 16 bit variable to hold the encoders position
  uint16_t encoderPosition;
  //let's also create a variable where we can count how many times we've tried to obtain the position in case there are errors
  uint8_t attempts;


  //if you want to set the zero position before beggining uncomment the following function call
  //Encoder->setZeroSPI();

  //once we enter this loop we will run forever
  while(1)
  {
    //set attemps counter at 0 so we can try again if we get bad position    
    attempts = 0;

    //this function gets the encoder position and returns it as a uint16_t
    encoderPosition = Encoder->getPositionSPI();

    //if the position returned was 0xFFFF we know that there was an error calculating the checksum
    //make 3 attempts for position. we will pre-increment attempts because we'll use the number later and want an accurate count
    while(encoderPosition == 0xFFFF && ++attempts < 3)
      encoderPosition = Encoder->getPositionSPI(); //try again

    if(encoderPosition == 0xFFFF){ //position is bad, let the user know how many times we tried
      Serial.print("Encoder 0 error. Attempts: ");
      Serial.print(attempts, DEC); //print out the number in decimal format. attempts - 1 is used since we post incremented the loop
      Serial.write(NEWLINE);
    }else{ //position was good, print to serial stream
      Serial.print("Encoder 0: ");
      Serial.print(encoderPosition, DEC); //print the position in decimal format
      Serial.write(NEWLINE);
    }

    //For the purpose of this demo we don't need the position returned that quickly so let's wait a half second between reads
    //delay() is in milliseconds
    delay(500);
  }
}
