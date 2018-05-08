//NAME: 	"bl652.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#include "bl652.h"

//Service Uart Structure Pointer
extern UsartDriver* 	ServUsart;
extern UartDriver* 	BL652Uart;

//Global flag variable
extern volatile uint8_t irqGlobalFlag;

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

uint8_t bl652Init_MK(void){
	
	uint8_t ret_val = ERROR_MK;
	uint32_t timeOut = 0;
	
	//Init BL652 Uart
	bl652UartInit(115200);						
	
	BL652Uart -> rxIntFunc = rxIntBl652;
	
	//setCTS();
	
	//Set AT reques flag 
	bl652Flag |= AT_REQ_FLAG;
	
	//Check Device Name
	BL652Uart->writeString(BL652Uart-> usartHandle , "AT I 0\r");
	
	//resetCTS();
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
	
		timeOut++;	
		if(timeOut == 10000000) timeOut = 0; break;	
	}
	ServUsart ->writeString(ServUsart->usartHandle, "$$$ BL652 AT I 0 (Device Name) response: ");
	ServUsart ->writeString(ServUsart->usartHandle, bl652AtReqBuf);
	bl652AtReqCnt = 0;
	memset(bl652AtReqBuf, 0 , sizeof(bl652AtReqBuf));
	
	HAL_Delay(10);
	
	//Check BLE Stack Build Number
	bl652Flag |= AT_REQ_FLAG;
	
	BL652Uart -> writeString(BL652Uart ->usartHandle , "AT i 1\r");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
	
		timeOut++;	
		if(timeOut == 10000000) timeOut = 0; break;	
	}
	ServUsart ->writeString(ServUsart->usartHandle, "$$$ BL652 AT I 1 (BLE Stack Build Number) response: ");
	ServUsart ->writeString(ServUsart->usartHandle, bl652AtReqBuf);
	bl652AtReqCnt = 0;
	memset(bl652AtReqBuf, 0 , sizeof(bl652AtReqBuf));
	
	HAL_Delay(10);
	
	//Check Version number of module firmware
	bl652Flag |= AT_REQ_FLAG;
	
	BL652Uart -> writeString(BL652Uart ->usartHandle , "AT i 3\r");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
	
		timeOut++;	
		if(timeOut == 10000000) timeOut = 0; break;	
	}
	ServUsart ->writeString(ServUsart->usartHandle, "$$$ BL652 AT I 3 (Version number of module firmware) response: ");
	ServUsart ->writeString(ServUsart->usartHandle, bl652AtReqBuf);
	bl652AtReqCnt = 0;
	memset(bl652AtReqBuf, 0 , sizeof(bl652AtReqBuf));
	
	HAL_Delay(10);
	
	bl652Flag |= AT_REQ_FLAG;
	
	BL652Uart -> writeString(BL652Uart ->usartHandle , "AT i 5\r");
	
	//Wait for BL652 response
	while(bl652Flag & AT_REQ_FLAG){
	
		timeOut++;	
		if(timeOut == 10000000) timeOut = 0; break;	
	}
	ServUsart ->writeString(ServUsart->usartHandle, "$$$ BL652 AT I 5 (Chipset ID) response: ");
	ServUsart ->writeString(ServUsart->usartHandle, bl652AtReqBuf);
	bl652AtReqCnt = 0;
	memset(bl652AtReqBuf, 0 , sizeof(bl652AtReqBuf));
	
	return ret_val;
}

/**
  * @brief  Rx Int Function
	* @ret  	
  */
uint8_t rxIntBl652(void){
	
	// If AT request flag is set
	if(bl652Flag & AT_REQ_FLAG){
		
		bl652AtReqBuf[bl652AtReqCnt] = BL652Uart -> rxRead;
		bl652AtReqCnt++;
			
		//ServUsart->writeChar(ServUsart ->usartHandle , USART3 -> RDR);
		if( bl652AtReqBuf[bl652AtReqCnt - 1] == 0x0D){
			
			//Reset flag
			bl652Flag &= ~(1 << AT_REQ_FLAG_POS);
			
			ledOff(1);
		}
		
	}
	return OK_MK;
}
