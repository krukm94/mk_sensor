//NAME: 	"main.c"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018

#include "init.h"

//Service Usart Driver
//extern UsartDriver* ServUsart;
//extern UartDriver* BL652Uart;

// >>>>>>>>>>>>>>>>> Variables
//extern volatile uint8_t irqGlobalFlag;

//Global ADXL read flag 
extern volatile uint8_t adxlReadFlag;

int main(void){

	// Init HAL
	HAL_Init();
	
	// Init MK
	init();
	
	while(1)
	{
		
		if(adxlReadFlag){
		
			adxlReadFlag = 0x00;
			
			adxlDataProces();
		}

	}
}


