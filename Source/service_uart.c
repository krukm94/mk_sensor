//NAME: 	"service_uart.c"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018

#include "service_uart.h"

UART_HandleTypeDef  service_uart;

volatile uint8_t ret_value;

/**
  * @brief  Init Service Uart (USART1)
  */
void serviceUartInit(void)
{
	GPIO_InitTypeDef gpio;
	
	//RCC ON
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//GPIO INIT
	gpio.Pin = SERVICE_UART_TX_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	gpio.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(SERVICE_UART_TX_PORT , &gpio);
	
//	gpio.Pin = SERVICE_UART_RX_PIN;
//	gpio.Mode = GPIO_MODE_AF_PP;
//	gpio.Pull = GPIO_PULLUP;
//	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	gpio.Alternate = GPIO_AF7_USART1;
//	HAL_GPIO_Init(SERVICE_UART_RX_PORT , &gpio);
	
	service_uart.Instance = SERVICE_UART_INSTANCE;
	service_uart.Init.BaudRate = 200000;
	service_uart.Init.StopBits = UART_STOPBITS_1;
	service_uart.Init.Parity = UART_PARITY_NONE;
	service_uart.Init.Mode = UART_MODE_TX;
	service_uart.Init.WordLength = UART_WORDLENGTH_8B;
	service_uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	service_uart.Init.OverSampling = UART_OVERSAMPLING_16;
	service_uart.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;

	ret_value = HAL_UART_Init(&service_uart);
	if(ret_value != 0)
	{
		char buf[40];
		sprintf(buf , "HAL_UART_Init = %d Error, File: service_uart.c, line: 46\r\n" , ret_value);
		serviceUartWriteS(buf);
	}
	
	__HAL_UART_ENABLE(&service_uart);

}

/**
  * @brief  Write string on service Uart Tx
  * @param  Pointer to string
  */
void serviceUartWriteS(char *s)
{
	UartWriteString(&service_uart , s);
}

