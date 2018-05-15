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

	//ADXL read axis values
	int32_t axis_x, axis_y, axis_z;
	
	// Init HAL
	HAL_Init();
	
	// Init MK
	init();
	
	while(1)
	{
			// ADXL355 Data read
			adxlReadAcc(&axis_x, &axis_y, &axis_z);
			
			//Delay(17);
			HAL_Delay(200);		//wahit 17 ms
	}
}


