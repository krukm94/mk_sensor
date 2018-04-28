//NAME: 	"bl652.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#include "bl652.h"

UART_HandleTypeDef  bl652_uart;

/**
  * @brief  BL652 Init
	* @ret  	Init Status
  */
uint8_t bl652Init(void){
	
	// >>>>>>>>>>>>>>> Uart Init
	GPIO_InitTypeDef gpio;
	uint8_t ret_value = 1;
	
	//RCC ON
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//GPIO INIT
	gpio.Pin = UART_TX_BL652_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(UART_TX_BL652_PORT , &gpio);
	
	gpio.Pin = UART_RX_BL652_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(UART_RX_BL652_PORT , &gpio);
	
	bl652_uart.Instance = BL652_UART_INSTANCE;
	bl652_uart.Init.BaudRate = 115200;
	bl652_uart.Init.StopBits = UART_STOPBITS_1;
	bl652_uart.Init.Parity = UART_PARITY_NONE;
	bl652_uart.Init.Mode = UART_MODE_TX_RX;
	bl652_uart.Init.WordLength = UART_WORDLENGTH_8B;
	bl652_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	bl652_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	bl652_uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;

	ret_value = HAL_UART_Init(&bl652_uart);
	if(ret_value != 0)
	{
		char buf[40];
		sprintf(buf , "HAL_UART_Init = %d Error, File: bl652.c, line: 53\r\n" , ret_value);
		serviceUartWriteS(buf);
		
		return 1;
	}
	
	__HAL_UART_ENABLE(&bl652_uart);
	
	// >>>>>>>>>>>>>>> Uart Init END <<<<<<<<<<<<<<<<<<<
	
	return ret_value;
}

