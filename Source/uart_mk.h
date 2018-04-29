//NAME: 	"uart_mk.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#ifndef __uart_mk_H_
#define __uart_mk_H_

#include "init.h"

void UsartWriteChar(USART_HandleTypeDef* handle, uint8_t data);
void UsartWriteString(USART_HandleTypeDef* handle, char *s);
void InitUsartDrivers(void);

void serviceUartInit(void);

typedef struct{

		USART_HandleTypeDef* usartHandle;
		void (*writeString)(USART_HandleTypeDef* handle, char* s);
	
}UsartDriver;

#endif
