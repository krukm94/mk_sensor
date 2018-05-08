//NAME: 	"main.c"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018

#include "init.h"

//Service Usart Driver
//extern UsartDriver* ServUsart;
//extern UartDriver* BL652Uart;

// >>>>>>>>>>>>>>>>> Variables
//extern volatile uint8_t irqGlobalFlag;

int main(void){

	uint8_t read = 0;
	
	// Init HAL
	HAL_Init();
	
	// Init MK
	init();
	
	while(1)
	{
		
		//Read CHIP ID REG
		adxlRead(ADXL_CHIP_ID ,&read, 1);
		
		HAL_Delay(200);
	}
}


