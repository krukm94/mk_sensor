//NAME: 	"uart_mk.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#ifndef __uart_mk_H_
#define __uart_mk_H_

#include "init.h"

void UartWrite(UART_HandleTypeDef *uart_handle, char data);
void UartWriteString(UART_HandleTypeDef *uart_handle, char *s);

#endif
