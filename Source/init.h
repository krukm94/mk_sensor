//NAME: 	"init.h"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018


#ifndef __INIT_H_
#define __INIT_H_

#include "stm32l4xx.h"

#include "stm32l4xx_hal.h"

#include "stm32l4xx_ll_gpio.h"

#include "stdio.h"
#include <cstdio>
#include <cmath>

#include "gpio.h"
#include "service_uart.h"
#include "adxl.h"

// >>>>>>>>>> NVIC PRIORITETS
#define SYSTIC_NVIC_PRIOTITY       	0 

// >>>>>>>>>>>>>> PIN DEFINES
// PORT A
#define SYS_WKUP_1_PIN							GPIO_PIN_0
#define SYS_WKUP_1_PORT							GPIOA

#define SIO_4_BL652_PIN							GPIO_PIN_1
#define SIO_4_BL652_PORT						GPIOA

#define SYS_WKUP_4_PIN							GPIO_PIN_2
#define SYS_WKUP_4_PORT							GPIOA

#define LPUART1_RX_EXT_PIN					GPIO_PIN_3
#define LPUART1_RX_EXT_PORT					GPIOA

#define LED2_PIN										GPIO_PIN_4
#define LED2_PORT										GPIOA

#define	SPI1_CLK_uSD_PIN						GPIO_PIN_5
#define SPI1_CLK_uSD_PORT						GPIOA

#define	SPI1_MISO_uSD_PIN						GPIO_PIN_6
#define SPI1_MISO_uSD_PORT					GPIOA

#define SPI1_MOSI_uSD_PIN						GPIO_PIN_7
#define SPI1_MOSI_uSD_PORT					GPIOA

#define PPG_CLK_PIN									GPIO_PIN_8
#define PPG_CLK_PORT								GPIOA

#define SERVICE_UART_TX_PIN					GPIO_PIN_9
#define SERVICE_UART_TX_PORT				GPIOA

#define SERVICE_UART_RX_PIN					GPIO_PIN_10
#define SERVICE_UART_RX_PORT				GPIOA

#define USB_DM_PIN									GPIO_PIN_11
#define USB_DM_PORT									GPIOA

#define USB_DP_PIN									GPIO_PIN_12
#define USB_DP_PORT									GPIOA

#define SWDIO_PIN										GPIO_PIN_13
#define SWDIO_PORT									GPIOA

#define SWDCLK_PIN									GPIO_PIN_14
#define SWDCLK_PORT									GPIOA

#define	BMI160_CS										GPIO_PIN_15
#define BMI160_CS_PORT							GPIOA

// >>>>>>>>>>>>>> PIN DEFINES
// PORT B
#define uSD_EN_PIN									GPIO_PIN_0
#define uSD_EN_PORT									GPIOB

#define VDD_12_ON_PIN								GPIO_PIN_1
#define VDD_12_ON_PORT 							GPIOB

#define SIO_09_BL652_PIN 						GPIO_PIN_2
#define SIO_09_BL652_PORT 					GPIOB

#define UART_RTS_BL652_PIN 					GPIO_PIN_3
#define UART_RTS_BL652_PORT 				GPIOB

#define UART_CTS_BL652_PIN 					GPIO_PIN_4
#define UART_CTS_BL652_PORT 				GPIOB

#define SPI_CS_EXT2_PIN 						GPIO_PIN_5
#define	SPI_CS_EXT2_PORT 						GPIOB

#define PPG_I2C_SCL_PIN 						GPIO_PIN_6
#define PPG_I2C_SCL_PORT 						GPIOB

#define PPG_I2C_SDA_PIN 						GPIO_PIN_7
#define PPG_I2C_SDA_PORT 						GPIOB

#define PPG_ARDY_PIN 								GPIO_PIN_8
#define PPG_ARDY_PORT 							GPIOB

#define PPG_RES_PIN 								GPIO_PIN_9
#define PPG_RES_PORT 								GPIOB

#define USART3_TX_PIN 							GPIO_PIN_10
#define USART3_TX_PORT 							GPIOB
#define UART_RX_BL652_PIN 					USART3_TX_PIN		//CROSS
#define UART_RX_BL652_PORT 					USART3_TX_PORT	//CROSS

#define USART3_RX_PIN 							GPIO_PIN_11
#define USART3_RX_PORT 							GPIOB	
#define UART_TX_BL652_PIN						USART3_RX_PIN		//CROSS
#define UART_TX_BL652_PORT 					USART3_RX_PORT	//CROSS

#define nCH_BQ24_PIN 								GPIO_PIN_12
#define nCH_BQ24_PORT 							GPIOB

#define I2C2_SCL_EXT_PIN 						GPIO_PIN_13
#define I2C2_SCL_EXT_PORT 					GPIOB

#define I2C2_SDA_EXT_PIN 						GPIO_PIN_14
#define I2C2_SDA_EXT_PORT 					GPIOB

#define SPI_CS_EXT1_PIN 						GPIO_PIN_15
#define SPI_CS_EXT1_PORT 						GPIOB

// >>>>>>>>>>>>>> PIN DEFINES
// PORT C

#define V_AKU_ADC_PIN								GPIO_PIN_0
#define V_AKU_ADC_PORT 							GPIOC

#define LPUART1_TX_EXT_PIN					GPIO_PIN_1
#define LPUART1_TX_EXT_PORT 				GPIOC

#define LED1_PIN										GPIO_PIN_2
#define LED1_PORT 									GPIOC

#define USER_BUTTON_PIN							GPIO_PIN_3
#define USER_BUTTON_PORT 						GPIOC

#define uSD_CS_PIN									GPIO_PIN_4
#define uSD_CS_PORT 								GPIOC

#define CS_ADXL_PIN									GPIO_PIN_6
#define CS_ADXL_PORT 								GPIOC

#define DRDY_ADXL_PIN								GPIO_PIN_7
#define DRDY_ADXL_PORT 							GPIOC

#define nCE_BQ24_PIN								GPIO_PIN_8
#define nCE_BQ24_PORT 							GPIOC

#define PWR_MAINTAIN_PIN						GPIO_PIN_9
#define PWR_MAINTAIN_PORT 					GPIOC

#define SPI3_CLK_PIN								GPIO_PIN_10
#define SPI3_CLK_PORT 							GPIOC

#define SPI3_MISO_PIN								GPIO_PIN_11
#define SPI3_MISO_PORT 							GPIOC

#define SPI3_MOSI_PIN								GPIO_PIN_12
#define SPI3_MOSI_PORT 							GPIOC

#define INT_MAX_PIN									GPIO_PIN_13
#define INT_MAX_PORT 								GPIOC

#define OSC32_IN_PIN								GPIO_PIN_14
#define OSC32_IN_PORT 							GPIOC

#define OSC32_OUT_PIN								GPIO_PIN_15
#define OSC32_OUT_PORT 							GPIOC

// >>>>>>>>>>>>>> INTERFACESS

#define SERVICE_UART_INSTANCE				USART1

#define BL652_UART_INSTANCE					USART3

#define LPUART_EXT_INSTANCE					LPUART1

#define uSD_SPI_INSTANCE						SPI1

#define I2C_EXT_INSTANCE						I2C2

#define BMI160_SPI_INSTANCE					SPI3

#define ADXL_SPI_INSTANE						SPI3 

#define MAX30003_SPI_INSTANCE				SPI3 

#define SPI_EXT_INSTANCE						SPI3

#define PPG_I2C_INSTANCE						I2C1

#define V_AKU_INSTANCE							ADC1


// >>>>>>>>>>>>>> FUNCTIONS

void SystemClock_Config(void);

void init(void);

void errorFunc(char *error_massage);
void _Error_Handler(char * file, int line);


#endif //__INIT_H_

