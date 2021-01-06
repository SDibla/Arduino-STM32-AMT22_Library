# ATM22 Library
Arduino and STM32 libraries for absolute encoder of the ATM22 series (12 bit or 14 bit version) by CUI Devices.

## Device
CUI Devices AMT22 series is a rugged, high accuracy absolute encoder outputting 12 bits or 14 bits of absolute position information with Serial Peripheral Interface (SPI) communication. Combined with an operating temperature range of -40°C to +125°C, high accuracy, a compact package, and low current draw, the AMT22 provides a compelling solution for a range of industrial, automation, robotics, and renewable energy applications.
|                               Features                              |
|:-------------------------------------------------------------------:|
| 12-bit absolute (4096) position or 14-bit absolute (16384) position |
| SPI communication                                                   |
| Resilient to dust, dirt and oil                                     |
| Low current draw                                                    |
| Modular locking hub design for simple assembly                      |
| -40 to +125°C temperature range                                     |

|    Specifications    |            |
|:--------------------:|------------|
| Input Voltage (Vdc)  | 3.8 ~ 5.5  |
| Operating Temp. (°C) | -40 ~ 125  |
| Speed, Max. (RPM)    | 4000, 8000 |

## Arduino Library
The library consists of a class where its constructor needs several parameters such as the pins for the connection (MOSI, MISO, SCLK, CS), the clock divider and the resolution.
All initializations are carried out within the constructor.
The public functions present are:
* getPositionSPI()
* setZeroSPI()
* resetAMT22()
* setResolution(uint8 t resolution)

The first function gets the absolute position from the AMT22 encoder using the SPI bus. The AMT22 position includes 2 checkbits to use for position verification. Both 12-bit and 14-bit possible encoders transfer position via two bytes, giving 16-bits regardless of resolution.
For 12-bit encoders the position is left-shifted two bits, leaving the right two bits as zeros. This gives the impression that the encoder is actually sending 14-bits, when it is actually sending 12-bit values, where every number is multiplied by 4.
In case of error the return value is 0xFFFF.
The following two functions bring the encoder to a default position.
These functions take advantage of two other private functions to work:
* spiWriteRead(uint8 t sendByte, uint8 t releaseLine)
* setCSLine(uint8 t csLine)

Used to transfer a data on the SPI lines and for set the state of the SPI line.

## STM32 Library

## Documents
The documents folder contains the PDF of the datasheet and the PDF of the assembly instructions.



The libraries and examples takes inspiration and is based on the code shared by CUI Devices which authorizes its diffusion and modification.
