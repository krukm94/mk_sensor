//NAME: 	"uart_mk.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#ifndef __uart_mk_H_
#define __uart_mk_H_

#include "init.h"

void UsartWriteChar(USART_HandleTypeDef* handle, uint8_t data);
void UsartWriteString(USART_HandleTypeDef* handle, char *s);

uint8_t serviceUartInit(uint32_t baudRate);
uint8_t bl652UartInit(uint32_t baudRate);

typedef struct{
	
		uint8_t rxRead; 
		USART_HandleTypeDef* usartHandle;
		void (*writeChar) (USART_HandleTypeDef* handle, uint8_t byte);
		void (*writeString)(USART_HandleTypeDef* handle, char* s);
	  uint8_t (*rxIntFunc) (void);
	
}UsartDriver;

#endif
