//NAME: 	"uart_mk.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#include "uart_mk.h"


//>>>>>>>>>>>>>>>>> Drivers
//Service Uart 
UsartDriver* ServUsart;

//BL652 Uart Driver
UartDriver* BL652Uart;

//>>>>>>>>>>>>>>>>> Handles
//Service Uart Handle (must be global)
USART_HandleTypeDef usart1;

//BL652 Uart
UART_HandleTypeDef  bl652_uart;


/**
  * @brief  USART Write char 
  * @param  char
  */
void UsartWriteChar(USART_HandleTypeDef* handle, uint8_t data){
	
	while(!(__HAL_USART_GET_FLAG(handle, UART_FLAG_TXE)));	
	HAL_USART_Transmit(handle, &data, 1 , 100);
}	
	
	/**
  * @brief  USART Write string on service Uart Tx
  * @param  Pointer to string
  */

void UsartWriteString(USART_HandleTypeDef* handle, char *s){

	while(*s) UsartWriteChar(handle,  *s++);
}


/**
  * @brief  UART Write char 
  * @param  char
  */
void UartWriteChar(UART_HandleTypeDef* handle, uint8_t data){
	
	while(!(__HAL_UART_GET_FLAG(handle, UART_FLAG_TXE)));	
	HAL_UART_Transmit(handle, &data, 1 , 100);
}	
	
	/**
  * @brief  UART Write string on service Uart Tx
  * @param  Pointer to string
  */

void UartWriteString(UART_HandleTypeDef* handle, char *s){

	while(*s) UartWriteChar(handle,  *s++);
}

/**
  * @brief  Init Service Uart (USART1)
  */
uint8_t serviceUartInit(uint32_t baudRate){
	
	GPIO_InitTypeDef gpio;
	uint8_t ret_value;
	
	//Memory alloc fot Service Uart
	ServUsart = (UsartDriver*)malloc(sizeof(UsartDriver));
	
	//Save Service uart handle
	ServUsart ->usartHandle = &usart1;	
	
	//Assign Service Uart Write String function
	ServUsart ->writeString = UsartWriteString;
	ServUsart ->writeChar   = UsartWriteChar;
	
	//RCC ON
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	//GPIO INIT
	gpio.Pin = 	SERVICE_USART_TX_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(SERVICE_USART_TX_PORT , &gpio);//Do zmiany
	
//	gpio.Pin = SERVICE_UART_RX_PIN;
//	gpio.Mode = GPIO_MODE_AF_PP;
//	gpio.Pull = GPIO_PULLUP;
//	gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//	gpio.Alternate = GPIO_AF7_USART1;
//	HAL_GPIO_Init(SERVICE_UART_RX_PORT , &gpio);
	
	ServUsart ->usartHandle->Instance = SERVICE_USART_INSTANCE;		
	ServUsart ->usartHandle->Init.BaudRate = baudRate;								
	ServUsart ->usartHandle->Init.StopBits = USART_STOPBITS_1;
	ServUsart ->usartHandle->Init.Parity = USART_PARITY_NONE;
	ServUsart ->usartHandle->Init.Mode = USART_MODE_TX;
	ServUsart ->usartHandle->Init.WordLength = USART_WORDLENGTH_8B;

	ret_value = HAL_USART_Init(ServUsart ->usartHandle);
	if(ret_value != 0)
	{
		return ERROR_MK;
	}
	 
	__HAL_USART_ENABLE(ServUsart ->usartHandle);
	
	char buf[40];
	sprintf(buf , "$$$ Service Uart Init OK\r\n");
	ServUsart->writeString(ServUsart->usartHandle ,buf);
	
	return ret_value;
}

//**
//  * @brief  BL652 Init
//	* @ret  	Init Status
//  */
uint8_t bl652UartInit(uint32_t baudRate){
	
	// >>>>>>>>>>>>>>> Uart Init
	GPIO_InitTypeDef gpio;
	uint8_t ret_value = ERROR_MK;
	
	//RCC ON
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//Memory alloc fot Service Uart
	BL652Uart = (UartDriver*)malloc(sizeof(UsartDriver));
	
	//Save Service uart handle
	BL652Uart ->usartHandle = &bl652_uart;	
	
	//Assign Service Uart Write String function
	BL652Uart ->writeString = UartWriteString;
	
	//GPIO INIT
	gpio.Pin = USART_TX_BL652_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART_TX_BL652_PORT , &gpio);
	
	gpio.Pin = USART_RX_BL652_PIN;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	gpio.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(USART_RX_BL652_PORT , &gpio);
	
	//Flow control Pins
	gpio.Pin = USART3_RTS_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	//HAL_GPIO_Init(USART3_RTS_PORT, &gpio);
	
	gpio.Pin = USART3_CTS_PIN;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLUP;
	gpio.Speed = GPIO_SPEED_MEDIUM;
	//HAL_GPIO_Init(USART3_CTS_PORT, &gpio);
	
	BL652Uart ->usartHandle->Instance = BL652_USART_INSTANCE;
	BL652Uart ->usartHandle->Init.BaudRate = baudRate;
	BL652Uart ->usartHandle->Init.StopBits = USART_STOPBITS_1;
	BL652Uart ->usartHandle->Init.Parity = USART_PARITY_NONE;
	BL652Uart ->usartHandle->Init.Mode = USART_MODE_TX_RX;
	BL652Uart ->usartHandle->Init.WordLength = USART_WORDLENGTH_8B;
	
	HAL_NVIC_SetPriority(USART3_IRQn, USART_BL652_NVIC_PRIORITY , 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);

	ret_value = HAL_UART_Init(BL652Uart ->usartHandle);
	if(ret_value != 0)
	{
		char buf[40];
		sprintf(buf , "HAL_UART_Init = %d Error, File: bl652.c, line: 53\r\n" , ret_value);
		ServUsart->writeString(ServUsart->usartHandle ,buf);
		
		return ERROR_MK;
	}
	
	//Enable Usart
	__HAL_UART_ENABLE(&bl652_uart);
	
	//Enable Usart IT
	__HAL_UART_ENABLE_IT(BL652Uart ->usartHandle , USART_IT_RXNE);
	
	// >>>>>>>>>>>>>>> Uart Init END <<<<<<<<<<<<<<<<<<<
	
	return ret_value;
}


/**
  * @brief setRTS
	* @ret  
  */
void setRTS(void){
	LL_GPIO_SetOutputPin(USART3_RTS_PORT , USART3_RTS_PIN);
}

/**
  * @brief setRTS
	* @ret  
  */
void resetRTS(void){
	LL_GPIO_ResetOutputPin(USART3_RTS_PORT , USART3_RTS_PIN);
}

/**
  * @brief setRTS
	* @ret  
  */
void setCTS(void){
	LL_GPIO_SetOutputPin(USART3_CTS_PORT , USART3_CTS_PIN);
}

/**
  * @brief setRTS
	* @ret  
  */
void resetCTS(void){
	LL_GPIO_ResetOutputPin(USART3_CTS_PORT , USART3_CTS_PIN);
}

