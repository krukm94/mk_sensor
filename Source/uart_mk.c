//NAME: 	"uart_mk.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#include "uart_mk.h"

//Service Uart 
UsartDriver* ServUsart;
//Service Uart Handle (must be global)
USART_HandleTypeDef usart1;


/**
  * @brief  Write char 
  * @param  char
  */
void UsartWriteChar(USART_HandleTypeDef* handle, uint8_t data){
	
	while(!(__HAL_USART_GET_FLAG(handle, UART_FLAG_TXE)));	
	HAL_USART_Transmit(handle, &data, 1 , 100);
}	
	
	/**
  * @brief  Write string on service Uart Tx
  * @param  Pointer to string
  */

void UsartWriteString(USART_HandleTypeDef* handle, char *s){

	while(*s) UsartWriteChar(handle,  *s++);
}
/**
  * @brief  Init uart_drivers
  */
void InitUsartDrivers(void){
	
	// Init service uart
	serviceUartInit();
}

/**
  * @brief  Init Service Uart (USART1)
  */
void serviceUartInit(void)
{
	GPIO_InitTypeDef gpio;
	uint8_t ret_value;
	
	//Memory alloc fot Service Uart
	ServUsart = (UsartDriver*)malloc(sizeof(UsartDriver));
	
	//Save Service uart handle
	ServUsart ->usartHandle = &usart1;	
	
	//Assign Service Uart Write String function
	ServUsart ->writeString = UsartWriteString;
	
	//RCC ON
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//GPIO INIT
	gpio.Pin = GPIO_PIN_2;  //SERVICE_UART_TX_PIN
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA , &gpio);
	
//	gpio.Pin = SERVICE_UART_RX_PIN;
//	gpio.Mode = GPIO_MODE_AF_PP;
//	gpio.Pull = GPIO_PULLUP;
//	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	gpio.Alternate = GPIO_AF7_USART1;
//	HAL_GPIO_Init(SERVICE_UART_RX_PORT , &gpio);
	
	ServUsart ->usartHandle->Instance = USART2;
	ServUsart ->usartHandle->Init.BaudRate = 115200;
	ServUsart ->usartHandle->Init.StopBits = USART_STOPBITS_1;
	ServUsart ->usartHandle->Init.Parity = USART_PARITY_NONE;
	ServUsart ->usartHandle->Init.Mode = USART_MODE_TX;
	ServUsart ->usartHandle->Init.WordLength = USART_WORDLENGTH_8B;

	ret_value = HAL_USART_Init(ServUsart ->usartHandle);
	if(ret_value != 0)
	{
		char buf[40];
		sprintf(buf , "HAL_UART_Init = %d Error, File: service_uart.c, line: 46\r\n" , ret_value);
		ServUsart ->writeString(ServUsart->usartHandle , buf);
	}
	 
	__HAL_USART_ENABLE(ServUsart ->usartHandle);
	
	char buf[40];
	sprintf(buf , "$$$ Service Uart Init OK\r\n");
	ServUsart->writeString(ServUsart->usartHandle ,buf);
	
}

