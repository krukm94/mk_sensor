//NAME: 	"bl652.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#include "bl652.h"

//Service Uart Structure Pointer
extern UsartDriver* 	ServUsart;
extern UsartDriver* 	BL652Usart;

//Global Read byte
volatile uint8_t 			rxReadBl652;

volatile uint8_t 			bl652Flag;

//Global bl652 at rx buff
char 				bl652AtReqBuf[50];
uint8_t 			bl652AtReqCnt;

// Enum Types

enum BL652_FLAGS{
	
		AT_REQ_FLAG			=	0b00000001
};

enum BL652_FLAG_POS{

		AT_REQ_FLAG_POS  = 0
};


/**
  * @brief  Init BL652
	* @ret  	Init Status
  */

uint8_t bl652Init(void){
	
	uint8_t ret_val = ERROR_MK;
	uint8_t timeOut = 0;
	
	//Init BL652 Uart
	bl652UartInit(115200);
	
	BL652Usart -> rxIntFunc = rxIntBl652;
	
	//Set AT reques flag 
	bl652Flag |= (1 << AT_REQ_FLAG_POS);
	
	//Check Device Name
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 0");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
		
		timeOut++;	
		if(timeOut == 100) timeOut = 0; break;	
	}
	
	//Check BLE Stack Build Number
	bl652Flag |= (1 << AT_REQ_FLAG_POS);
	
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 1");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
		
		timeOut++;	
		if(timeOut == 100) timeOut = 0; break;	
	}
	
	//Check Version number of module firmware
	bl652Flag |= (1 << AT_REQ_FLAG_POS);
	
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 3");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
		
		timeOut++;	
		if(timeOut == 100) timeOut = 0; break;	
	}
	
	//Check Chipset ID
	bl652Flag |= (1 << AT_REQ_FLAG_POS);
	
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 5");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
		
		timeOut++;	
		if(timeOut == 100) timeOut = 0; break;	
	}
	
	return ret_val;
}

/**
  * @brief  Rx Int Function
	* @ret  	
  */
uint8_t rxIntBl652(void){
	
	// If AT request flag is set
	if(bl652Flag & AT_REQ_FLAG){
		
		bl652AtReqBuf[bl652AtReqCnt] = BL652Usart -> rxRead;
		
		bl652AtReqCnt++;
		
		if( bl652AtReqBuf[bl652AtReqCnt] == 0x0D){
			
			// Tymczasowe przepychanie odebranych wiadmosci na service uart
			ServUsart ->writeString(ServUsart->usartHandle, bl652AtReqBuf);
			
			bl652AtReqCnt = 0;
			
			//Reset flag
			bl652Flag |= (0 << AT_REQ_FLAG_POS);
		}
	}
	return OK_MK;
}
