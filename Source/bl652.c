//NAME: 	"bl652.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#include "bl652.h"


//Service Uart Structure Pointer
extern UsartDriver* ServUsart;
extern UsartDriver* BL652Usart;

//Global Read byte
volatile uint8_t rxReadBl652;

/**
  * @brief  Init BL652
	* @ret  	Init Status
  */

uint8_t bl652Init(void){
	
	uint8_t ret_val = ERROR_MK;
	
	//Init BL652 Uart
	bl652UartInit(115200);
	
	BL652Usart ->rxIntFunc = rxIntBl652;
	
	//Check Device Name
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 0");
	
	//Check BLE Stack Build Number
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 1");
	
	//Check Version number of module firmware
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 3");
	
	//Check Chipset ID
	BL652Usart -> writeString(BL652Usart ->usartHandle , "AT i 5");
	
	return ret_val;
}

/**
  * @brief  Rx Int Function
	* @ret  	
  */

uint8_t rxIntBl652(void){
	
	// Tymczasowe przepychanie odebranych wiadmosci na service uart
	ServUsart ->writeChar(ServUsart->usartHandle, BL652Usart ->rxRead);
	
	return OK_MK;
}
