//NAME: 	"main.c"
//AUTHOR:	M.KRUK
//DATE 	:	VI.2018

#include "init.h"

//Service Usart Driver
extern UsartDriver* ServUsart;
extern UsartDriver* BL652Usart;

// >>>>>>>>>>>>>>>>> Variables
extern volatile uint8_t irqGlobalFlag;

int main(void){

	
	HAL_Init();
	
	init();
	
	while(1)
	{

			//Check Global Flag Variable
			switch (irqGlobalFlag) {
			
				case BL652_RX_FLAG:
					
					//Reset BL652_RX_FLAG
					irqGlobalFlag |= ( 0 << BL652_RX_POS);
					BL652Usart ->rxIntFunc();
				break;
				
				case SERV_USART_RX_FLAG:
					
					//ResetSERV_USART_RX_FLAG
					irqGlobalFlag |= ( 0 << SERV_USART_RX_POS);
				break;
				
				default:
					ServUsart->writeString(ServUsart->usartHandle , "$$$ Wrong IRQ FLAG!!!\r\n"); 
				break;
			}
	}
	
}


