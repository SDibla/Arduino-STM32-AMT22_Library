/*
  AMT22.h - STM32 library for ATM22 series absolute encoders by CUI Devices.
  Created by Simone Di Blasi, December 2020.
*/


#ifndef AMT22_H_
#define AMT22_H_

#include "stdint.h"

#define 	STM32F4					// Comment when using H7
//#define	STM32H7					// Comment when using F4

#ifdef	STM32F4
#include "stm32f446xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx_hal_tim.h"
#elif 	STM32H7
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_tim.h"
#endif


/*
 * @brief 	Sets the state of the SPI line. It isn't necessary but makes the code more readable than having HAL_GPIO_Write everywhere.
 * @param	  encoderPort to select the GPIO peripheral for STM32.
 * @param   encoderPin specifies the port bit to be written.
 * @param 	csLine value to be set.
 * @retval	none.
 *
 */
void setCSLine (GPIO_TypeDef* encoderPort, uint16_t encoderPin, GPIO_PinState csLine);


/*
 * @brief	  Does SPI transfer.
 * @param	  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 * @param	  sendByte to be transmitted.
 * @param   encoderPort to select the GPIO peripheral for STM32.
 * @param   encoderPin specifies the port bit to be written.
 * @param 	releaseLine used to let the spiWriteRead function know if it should release the chip select line after transfer.
 * @param   timer is used to make microsecond delays during data exchange.
 * @retval 	data received from encoder.
 *
 */
uint8_t spiWriteRead(SPI_HandleTypeDef *hspi, uint8_t sendByte, GPIO_TypeDef* encoderPort, uint16_t encoderPin, uint8_t releaseLine, TIM_HandleTypeDef *timer);


/*
 * @brief 	Gets the absolute position from the AMT22 encoder using the SPI bus. The AMT22 position includes 2 checkbits to use
 * 				  for position verification. Both 12-bit and 14-bit encoders transfer position via two bytes, giving 16-bits regardless of resolution.
 * 				  For 12-bit encoders the position is left-shifted two bits, leaving the right two bits as zeros. This gives the impression that the encoder
 * 				  is actually sending 14-bits, when it is actually sending 12-bit values, where every number is multiplied by 4.
 * 				  Error values are returned as 0xFFFF.
 * @param	  hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 * @param   encoderPort to select the GPIO peripheral for STM32.
 * @param   encoderPin specifies the port bit to be written.
 * @param	  resolution to properly format position responses.
 * @param   timer is used to make microsecond delays during data exchange.
 * @retval	currentPosition of the encoder. In case of error returned value is 0xFFFF.
 *
 */
uint16_t getPositionSPI(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, uint8_t resolution, TIM_HandleTypeDef *timer);


/*
 * @brief 	Sets value of the given encoder to ZERO.
 * @param 	hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 * @param	  encoderPort to select the GPIO peripheral for STM32.
 * @param   encoderPin specifies the port bit to be written.
 * @param   timer is used to make microsecond delays during data exchange.
 * @retval	none.
 *
 */
void setZeroSPI(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, TIM_HandleTypeDef *timer);


/*
 * @brief 	Resets given encoder.
 * @param 	hspi pointer to a SPI_HandleTypeDef structure that contains the configuration information for SPI module.
 * @param	  encoderPort to select the GPIO peripheral for STM32.
 * @param   encoderPin specifies the port bit to be written.
 * @param   timer is used to make microsecond delays during data exchange.
 * @retval	none.
 *
 */
void resetAMT22(SPI_HandleTypeDef *hspi, GPIO_TypeDef* encoderPort, uint16_t encoderPin, TIM_HandleTypeDef *timer);


/*
 * @brief	  Delay in microseconds. Uses HW timer in order to unload CPU.
 * @param 	timer - address of the timer.
 * @param	  delayTime.
 * @retval	none.
 *
 */
void delay(TIM_HandleTypeDef *timer, uint32_t delayTime);


#endif /* SRC_AMT22_H_ */
