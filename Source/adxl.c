//NAME: 	"adxl.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018 


#include "adxl.h"

//Handles 
SPI_HandleTypeDef spi3_ad;

volatile uint8_t spi_read;

volatile acc_axis_adxl acc_adxl;	//Acc structure declaration

#define LSB_8g 							15.25878906				//15.25878906 ug
#define LSB_4g							7.629394531				//7.629394531 ug
#define LSB_2g							3.814697266				//3.814697266 ug

//Service Uart Structure Pointer
extern UsartDriver* ServUsart;

/**
  * @brief  Initialization adxl
  */
uint8_t adxlInit(void)
{
	//GPIO handle
	GPIO_InitTypeDef gpio;

	uint8_t set = 0, read = 0;
	uint8_t ret_value = 1;
	
	char print[30];
	
	//RCC On
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= ADXL_MISO_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADXL_MISO_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= ADXL_MOSI_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADXL_MOSI_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= ADXL_CLK_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADXL_CLK_PORT , &gpio);
	
	gpio.Mode 			= GPIO_MODE_OUTPUT_PP;
	gpio.Pull 			= GPIO_PULLDOWN;
	gpio.Pin 				= ADXL_CS_PIN;
	gpio.Speed			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ADXL_CS_PORT , &gpio);
	
	spi3_ad.Instance 							= ADXL_SPI_INSTANCE;
	spi3_ad.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	spi3_ad.Init.DataSize 					= SPI_DATASIZE_8BIT;
	spi3_ad.Init.Direction 				= SPI_DIRECTION_2LINES;
	spi3_ad.Init.Mode 							= SPI_MODE_MASTER;
	spi3_ad.Init.CLKPhase 					= SPI_PHASE_1EDGE;
	spi3_ad.Init.TIMode 						= SPI_TIMODE_DISABLE;
	spi3_ad.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
	spi3_ad.Init.CLKPolarity 			= SPI_POLARITY_LOW;
	spi3_ad.Init.NSS 							= SPI_NSS_SOFT;
	spi3_ad.Init.CRCPolynomial 		= 7;
	spi3_ad.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
	spi3_ad.Init.CRCLength 				= SPI_CRC_LENGTH_DATASIZE;
	spi3_ad.Init.NSSPMode 					= SPI_NSS_PULSE_ENABLE;
	
	ret_value = HAL_SPI_Init(&spi3_ad);
	
	if(ret_value != HAL_OK)
	{
		char buf[50];
		sprintf(buf , "$$$ ErrorHAL_SPI_Init = %d, file: adxl.c, line:70 \n\r" , ret_value);
		ServUsart->writeString(ServUsart->usartHandle ,buf);
		
		return 1;
	}
	
	LL_GPIO_SetOutputPin(ADXL_CS_PORT , ADXL_CS_PIN);
	
	HAL_Delay(10);
	
	adxlRead(ADXL_CHIP_ID ,&read, 1);
	if(read != 0xAD){
		sprintf(print , "$$$ Error, ADXL Bad CHIP ID: 0x%0.2X , should be 0xAD\n\r" , read);
		ServUsart->writeString(ServUsart->usartHandle ,print);
		
		return 1;
	}
	else {
		ret_value = 0;
		
		sprintf(print , "$$$ ADXL CHIP ID: 0x%0.2X\n\r" , read);
		ServUsart->writeString(ServUsart->usartHandle ,print);
	}
	
	
	adxlRead(ADXL_POWER , &set , 1);									
	set = 0x00;
 	adxlWrite(ADXL_POWER , &set , 1);									//SET MEASURMENT ON
	
	sprintf(print , "$$$ SET: 0x%0.2X \n\r" , set);
	ServUsart->writeString(ServUsart->usartHandle ,print);

	adxlRead(ADXL_RANGE , &set , 1);
	set |= RANGE_2G;
 	adxlWrite(ADXL_RANGE , &set , 1);									//SET RANGE = 8g
	
	adxlRead(ADXL_ODR_LPF , &set , 1);
	set |= 0x02;
 	adxlWrite(ADXL_ODR_LPF , &set , 1);								//SET ODR
	
	return ret_value;
}


/**
  * @brief  Read adxl register
	* @param  addr_reg : adres of register
	*         pData    : Pointer to store read data
	*					Size     : Size of data to read
  */
void adxlRead(uint8_t addr_reg , uint8_t* pData , uint8_t Size)
{
	uint8_t addr = ((addr_reg << 1) | 1);											//Address to send

	LL_GPIO_ResetOutputPin(ADXL_CS_PORT , ADXL_CS_PIN); 					//CS LOW

	HAL_SPI_Transmit(&spi3_ad , (uint8_t*) &addr , 1 , 100);			//SEND REGISTER ADDRESS

	HAL_SPI_Receive(&spi3_ad, (uint8_t *)pData , Size , 100 );		//RECEIVE DATA

	LL_GPIO_SetOutputPin(ADXL_CS_PORT , ADXL_CS_PIN); 						//CS HIGH
}


/**
  * @brief  adxl Write Reg
	* @param  addr_teg : adres of register to write
	*  				pData    : Pionter to data
	*					Size		 : Size of data to send
  */
void adxlWrite(uint8_t addr_reg , uint8_t* pData , uint8_t Size)
{
	uint8_t addr = (addr_reg << 1);												//Address to send
	
	LL_GPIO_ResetOutputPin(ADXL_CS_PORT , ADXL_CS_PIN);			//CS LOW
	
	HAL_SPI_Transmit(&spi3_ad , &addr , 1 , 100);						//SEND REGISTER ADDRES
	HAL_SPI_Transmit(&spi3_ad , pData , 1 ,100);							//SEND DATA
	
	LL_GPIO_SetOutputPin(ADXL_CS_PORT , ADXL_CS_PIN);				//CS HIGH
}


/**
  * @brief  Read bmi160 accelerometer values
  * @param  acc_x, acc_y and acc_y are pointers to store acc data
  */
void adxlReadAcc(int32_t *acc_x , int32_t *acc_y , int32_t *acc_z)
{
	//uint8_t read, read1 , read2;
	char print_acc[100];
	
	adxlRead( ADXL_X_DATA , (uint8_t *)acc_adxl.acc_buf , 9);
	
	acc_adxl.acc_x = (acc_adxl.acc_buf[0] << 12) | acc_adxl.acc_buf[1] << 4 | acc_adxl.acc_buf[2] >> 4;
	acc_adxl.acc_y = (acc_adxl.acc_buf[3] << 12) | acc_adxl.acc_buf[4] << 4 | acc_adxl.acc_buf[5] >> 4;
	acc_adxl.acc_z = (acc_adxl.acc_buf[6] << 12) | acc_adxl.acc_buf[7] << 4 | acc_adxl.acc_buf[8] >> 4;
	
	acc_adxl.X = (acc_adxl.acc_x * LSB_8g)/1000000;												
	acc_adxl.Y = (acc_adxl.acc_y * LSB_8g)/1000000;
	acc_adxl.Z = (acc_adxl.acc_z * LSB_8g)/1000000;
	
	acc_adxl.acc_g[0] = (float)sqrt(acc_adxl.X*acc_adxl.X + acc_adxl.Y*acc_adxl.Y + acc_adxl.Z*acc_adxl.Z);
	
	sprintf(print_acc, "$$$ %d %d %d\r\n" , acc_adxl.acc_x , acc_adxl.acc_y , acc_adxl.acc_z);
	
	ServUsart->writeString(ServUsart->usartHandle ,print_acc);
}


