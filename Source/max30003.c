//NAME: 	"max30003.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018 


#include "max30003.h"

//Handles 
SPI_HandleTypeDef spi3_max;


//Service Uart Structure Pointer
extern UsartDriver* ServUsart;

/**
  * @brief  Initialization adxl
  */
uint8_t maxInit(void)
{
	//GPIO handle
	GPIO_InitTypeDef gpio;

//	uint8_t set = 0, read = 0;
	uint8_t ret_value = 1;
	
	int32_t rd;
	
	char print[30];
	
	//RCC On
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= MAX_MISO_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAX_MISO_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= MAX_MOSI_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAX_MOSI_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= MAX_CLK_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAX_CLK_PORT , &gpio);
	
	gpio.Mode 			= GPIO_MODE_OUTPUT_PP;
	gpio.Pull 			= GPIO_PULLDOWN;
	gpio.Pin 				= MAX_CS_PIN;
	gpio.Speed			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(MAX_CS_PORT , &gpio);
	
	spi3_max.Instance 								= MAX_SPI_INSTANCE;
	spi3_max.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_32;
	spi3_max.Init.DataSize 					= SPI_DATASIZE_8BIT;
	spi3_max.Init.Direction 					= SPI_DIRECTION_2LINES;
	spi3_max.Init.Mode 							= SPI_MODE_MASTER;
	spi3_max.Init.CLKPhase 					= SPI_PHASE_1EDGE;
	spi3_max.Init.TIMode 						= SPI_TIMODE_DISABLE;
	spi3_max.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
	spi3_max.Init.CLKPolarity 				= SPI_POLARITY_LOW;
	spi3_max.Init.NSS 								= SPI_NSS_SOFT;
	spi3_max.Init.CRCPolynomial 			= 7;
	spi3_max.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
	spi3_max.Init.CRCLength 					= SPI_CRC_LENGTH_DATASIZE;
	spi3_max.Init.NSSPMode 					= SPI_NSS_PULSE_ENABLE;
	
	ret_value = HAL_SPI_Init(&spi3_max);
	
	if(ret_value != HAL_OK)
	{
		char buf[50];
		sprintf(buf , "$$$ ErrorHAL_SPI_Init = %d, file: adxl.c, line:70 \n\r" , ret_value);
		ServUsart->writeString(ServUsart->usartHandle ,buf);
		
		return 1;
	}
	
	HAL_Delay(1);
  max30003WriteReg(MAX_SW_RST,0x000000); // reset
  HAL_Delay(5);
	
	rd=max30003ReadReg(MAX_NO_OP); 
	
	while(1){
		rd=max30003ReadReg(MAX_INFO_REG); 
		//rd=rd&0x00FF3000;
		//printf("MAX30003 INFO reg: 0x%08lX\r\n", rd); // jakas niezgodnosc z dokumentacja
		if(rd==0x00513000) { 
			sprintf(print, "$$$ MAX30003 INFO reg ok!\r\n"); 
			ServUsart->writeString(ServUsart->usartHandle ,print);
		} 
		else
		{
			sprintf(print, "$$$ MAX30003 INFO reg error! rd = %d\r\n 0x%x \r\n 0x%x \r\n 0x%x\r\n" , rd , rd >> 16 , ((rd & 0x0000FF00 )>> 8) , rd & 0x000000FF);
			ServUsart->writeString(ServUsart->usartHandle ,print);
			//return 1;
		}
		
		HAL_Delay(300);
	}
}

//========================================================================
int32_t max30003ReadReg (int8_t reg_addr)
{
	int32_t d32u;
	uint8_t pData[3];
	uint8_t addr = ((reg_addr << 1) | 1);															//Address to send
  
  LL_GPIO_ResetOutputPin(MAX_CS_PORT , MAX_CS_PIN); 
	
//  //spiECGWriteReadByte(reg_addr|BIT0);
//	HAL_SPI_Transmit(&spi3_max , (uint8_t*) &reg_addr , 1 , 100);			//SEND REGISTER ADDRESS
// 
//	//d32u =((t_int32u)spiECGWriteReadByte(0xff)<<16);
//	HAL_SPI_Receive(&spi3_max, &pData , 1 , 100 );		//RECEIVE DATA
//	d32u = pData << 16;
//	
//  //d32u|=((t_int32u)spiECGWriteReadByte(0xff)<<8);
//	HAL_SPI_Receive(&spi3_max, &pData , 1 , 100 );		//RECEIVE DATA
//	d32u = pData << 8;
//	
//  //d32u|=((t_int32u)spiECGWriteReadByte(0xff)<<0);
//	HAL_SPI_Receive(&spi3_max, &pData , 1 , 100 );		//RECEIVE DATA
//	d32u = pData << 0;
	
//	HAL_SPI_Receive(&spi3_max, pData , 3, 100);

 HAL_SPI_TransmitReceive(&spi3_max, &addr, pData , 4, 100);
	
	d32u = pData[0] << 16 | pData[1] << 8 | pData[2]; 
  //DelayUs(1);  
  LL_GPIO_SetOutputPin(MAX_CS_PORT , MAX_CS_PIN); 

  return d32u;  
}

//========================================================================
void max30003WriteReg (int8_t reg_addr, int32_t reg_val)
{
	uint8_t dataToSend[4];
	dataToSend[0] = (reg_addr << 1);		
	dataToSend[1] = (reg_val << 16);
	dataToSend[2] = (reg_val << 8);
	dataToSend[3] = (reg_val << 0);
	
  LL_GPIO_ResetOutputPin(MAX_CS_PORT , MAX_CS_PIN); 

	HAL_SPI_Transmit(&spi3_max ,dataToSend , 4 , 100);
//  spiECGWriteByte((t_int8u)(reg_val>>16));
//  spiECGWriteByte((t_int8u)(reg_val>>8));
//  spiECGWriteByte((t_int8u)(reg_val>>0));
  //DelayUs(1);
  LL_GPIO_SetOutputPin(MAX_CS_PORT , MAX_CS_PIN); 
}
