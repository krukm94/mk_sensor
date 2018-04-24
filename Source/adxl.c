//NAME: 	"adxl.c"
//AUTHOR:	M.KRUK
//DATE 	:	XI.2017


#include "adxl.h"

//Handles 
SPI_HandleTypeDef spi1;
I2C_HandleTypeDef hi2c1;


volatile uint8_t spi_read;

volatile acc_axis acc;	//Acc structure declaration

#define LSB_8g 							15.25878906				//15.25878906 ug
#define LSB_4g							7.629394531				//7.629394531 ug
#define LSB_2g							3.814697266				//3.814697266 ug


/**
  * @brief  Initialization adxl
  */
void adxlInit(void)
{
	//GPIO handle
	GPIO_InitTypeDef gpio;

	uint8_t set = 0;
	uint8_t ret_value = 0;
	
	char print[30];
	
	//RCC On
	__HAL_RCC_SPI1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	gpio.Alternate 	= GPIO_AF5_SPI1;
	gpio.Pin 				= ADXL_MISO_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ADXL_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF5_SPI1;
	gpio.Pin 				= ADXL_MOSI_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ADXL_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF5_SPI1;
	gpio.Pin 				= ADXL_CLK_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ADXL_PORT , &gpio);
	
	gpio.Mode 			= GPIO_MODE_OUTPUT_PP;
	gpio.Pull 			= GPIO_PULLDOWN;
	gpio.Pin 				= ADXL_CS_PIN;
	gpio.Speed			= GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(ADXL_PORT , &gpio);
	
	spi1.Instance 							= ADXL_SPI_INSTANCE;
	spi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	spi1.Init.DataSize 					= SPI_DATASIZE_8BIT;
	spi1.Init.Direction 				= SPI_DIRECTION_2LINES;
	spi1.Init.Mode 							= SPI_MODE_MASTER;
	spi1.Init.CLKPhase 					= SPI_PHASE_1EDGE;
	spi1.Init.TIMode 						= SPI_TIMODE_DISABLE;
	spi1.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
	spi1.Init.CLKPolarity 			= SPI_POLARITY_LOW;
	spi1.Init.NSS 							= SPI_NSS_SOFT;
	spi1.Init.CRCPolynomial 		= 7;
	spi1.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
	spi1.Init.CRCLength 				= SPI_CRC_LENGTH_DATASIZE;
	spi1.Init.NSSPMode 					= SPI_NSS_PULSE_ENABLE;
	
	ret_value = HAL_SPI_Init(&spi1);
	if(ret_value != HAL_OK)
	{
		char buf[50];
		sprintf(buf , "$ ErrorHAL_SPI_Init = %d, file: adxl.c, line:70 \n\r" , ret_value);
		errorFunc(buf);
	}
	
	LL_GPIO_SetOutputPin(ADXL_PORT , ADXL_CS_PIN);
	
	HAL_Delay(10);
	
	
	adxlRead(ADXL_POWER , &set , 1);									
	set = 0x00;
 	adxlWrite(ADXL_POWER , &set , 1);									//SET MEASURMENT ON
	
	sprintf(print , "$ SET: 0x%0.2X \n\r" , set);
	serviceUartWriteS(print);

	adxlRead(ADXL_RANGE , &set , 1);
	set |= RANGE_2G;
 	adxlWrite(ADXL_RANGE , &set , 1);									//SET RANGE = 8g
	
	adxlRead(ADXL_ODR_LPF , &set , 1);
	set |= 0x02;
 	adxlWrite(ADXL_ODR_LPF , &set , 1);								//SET ODR
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

	LL_GPIO_ResetOutputPin(ADXL_PORT , ADXL_CS_PIN); 					//CS LOW

	HAL_SPI_Transmit(&spi1 , (uint8_t*) &addr , 1 , 100);			//SEND REGISTER ADDRESS

	HAL_SPI_Receive(&spi1, (uint8_t *)pData , Size , 100 );		//RECEIVE DATA

	LL_GPIO_SetOutputPin(ADXL_PORT , ADXL_CS_PIN); 						//CS HIGH
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
	
	LL_GPIO_ResetOutputPin(ADXL_PORT , ADXL_CS_PIN);			//CS LOW
	
	HAL_SPI_Transmit(&spi1 , &addr , 1 , 100);						//SEND REGISTER ADDRES
	HAL_SPI_Transmit(&spi1 , pData , 1 ,100);							//SEND DATA
	
	LL_GPIO_SetOutputPin(ADXL_PORT , ADXL_CS_PIN);				//CS HIGH
}


/**
  * @brief  Read bmi160 accelerometer values
  * @param  acc_x, acc_y and acc_y are pointers to store acc data
  */
void adxlReadAcc(int32_t *acc_x , int32_t *acc_y , int32_t *acc_z)
{
	//uint8_t read, read1 , read2;
	char print_acc[100];
	
	adxlRead( ADXL_X_DATA , (uint8_t *)acc.acc_buf , 9);
	
	acc.acc_x = (acc.acc_buf[0] << 12) | acc.acc_buf[1] << 4 | acc.acc_buf[2] >> 4;
	acc.acc_y = (acc.acc_buf[3] << 12) | acc.acc_buf[4] << 4 | acc.acc_buf[5] >> 4;
	acc.acc_z = (acc.acc_buf[6] << 12) | acc.acc_buf[7] << 4 | acc.acc_buf[8] >> 4;
	
	acc.X = (acc.acc_x * LSB_8g)/1000000;												
	acc.Y = (acc.acc_y * LSB_8g)/1000000;
	acc.Z = (acc.acc_z * LSB_8g)/1000000;
	
	acc.acc_g[0] = (float)sqrt(acc.X*acc.X + acc.Y*acc.Y + acc.Z*acc.Z);
	
	sprintf(print_acc, "%d %d %d\r\n" , acc.acc_x , acc.acc_y , acc.acc_z);
	
	serviceUartWriteS(print_acc);
}


