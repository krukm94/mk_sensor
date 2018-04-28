//NAME: 	"uart_mk.c"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018

#include "uart_mk.h"

/**
  * @brief  Write One Char Via Uart
	* @param  uart_handle: handle to your uart
	* @param 	data: one char to send
  */
void UartWrite(UART_HandleTypeDef *uart_handle, char data){
	
	while(!(__HAL_USART_GET_FLAG(uart_handle, UART_FLAG_TXE)));	
	SERVICE_UART_INSTANCE ->TDR = data;
}
/**
  * @brief  Write String to Uart
* @param  uart_handle: handle to your uart
* @param s: pointer to your buffer 
  */
void UartWriteString(UART_HandleTypeDef *uart_handle, char *s){
	
	while(*s) UartWrite(uart_handle, *s++);
}
