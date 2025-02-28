//NAME: 	"bmi160.c"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018


#include "bmi160.h"

//LSB defines for ACC
#define LSB_2g	 58.688890193 //ug
#define LSB_4g	 117.37089201 //ug
#define LSB_8g	 234.74178403 //ug
#define LSB_16g  469.48356807 //ug

//Handles 
SPI_HandleTypeDef spi3;

TIM_HandleTypeDef			tim16;


//Variables for avrage filter
float average = 0.9;
uint16_t dt   = 10;

//Variables for Acc variables and flass
volatile float rawData;
volatile float filteredData;
volatile uint8_t threshold_flag;
volatile uint8_t threshold_detection;
volatile uint8_t threshold_cnt;
volatile uint8_t bmi_read_flag;
volatile uint8_t no_motion_flag;
volatile uint8_t acc_done_flag;
float acc_lsb;
volatile acc_axis acc;	//Acc structure declaration

//PWR flags
extern volatile uint8_t go_to_sleep_mode;
										
char print_acc[80];

//FatFS variables
//extern FIL file;
 
//GPS variables
//extern volatile gps_data gps_nmea;
//extern volatile uint8_t gps_done_flag;

//extern char buf_gps_fix[10000];
//extern uint16_t buf_gps_fix_cnt;

//Service Uart Structure Pointer
extern UsartDriver* ServUsart;

/**
  * @brief  Initialization BMI160
  */
uint8_t bmi160Init(void)
{
	//GPIO handle
	GPIO_InitTypeDef gpio;
	
	uint8_t bmi160_init_cnt = 0,cnt = 0;
	uint8_t read = 0;
	
	uint8_t return_value = 1; 
	
//	uint8_t acc_800Hz = 0x4B;
//	uint8_t acc_1600Hz = 0x4C;
		uint8_t soft_reset = 0x6B;
	
	char print[30];
	
	//RCC On
	__HAL_RCC_SPI3_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
//	gpio.Pin				= BMI160_INT1_PIN;
//	gpio.Mode				= GPIO_MODE_IT_FALLING;
//	gpio.Pull 			= GPIO_PULLUP;
//	HAL_GPIO_Init(BMI160_INT1_PORT , &gpio);
	
	gpio.Mode 			= GPIO_MODE_OUTPUT_PP;
	gpio.Pull 			= GPIO_PULLUP;
	gpio.Pin 				= BMI160_CS_PIN;
	gpio.Speed			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMI160_CS_PORT , &gpio);
	
	LL_GPIO_SetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN);	
	HAL_Delay(5);
	LL_GPIO_ResetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN);	
	HAL_Delay(5);
	LL_GPIO_SetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN);	
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= BMI160_MISO_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMI160_MISO_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= BMI160_MOSI_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMI160_MOSI_PORT , &gpio);
	
	gpio.Alternate 	= GPIO_AF6_SPI3;
	gpio.Pin 				= BMI160_SCK_PIN;
	gpio.Mode 			= GPIO_MODE_AF_PP;
	gpio.Pull				= GPIO_NOPULL;
	gpio.Speed 			= GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BMI160_SCK_PORT , &gpio);
	

	spi3.Instance 							= BMI160_SPI_INSTANCE;
	spi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	spi3.Init.DataSize 					= SPI_DATASIZE_8BIT;
	spi3.Init.Direction 				= SPI_DIRECTION_2LINES;
	spi3.Init.Mode 							= SPI_MODE_MASTER;
	spi3.Init.CLKPhase 					= SPI_PHASE_2EDGE;
	spi3.Init.TIMode 						= SPI_TIMODE_DISABLE;
	spi3.Init.FirstBit 					= SPI_FIRSTBIT_MSB;
	spi3.Init.CLKPolarity 			= SPI_POLARITY_HIGH;
	spi3.Init.NSS 							= SPI_NSS_SOFT;
	spi3.Init.CRCPolynomial 		= 7;
	spi3.Init.CRCCalculation 		= SPI_CRCCALCULATION_DISABLE;
	spi3.Init.CRCLength 				= SPI_CRC_LENGTH_DATASIZE;
	spi3.Init.NSSPMode 					= SPI_NSS_PULSE_ENABLE;

	if(HAL_SPI_Init(&spi3))
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	
	HAL_Delay(5);
	
	bmi160Write(BMI160_CMD , &soft_reset , 1);
	
	HAL_Delay(50);
	
	bmi160Read(BMI160_CHECK_ADDR, &read , 1);

	read = 0x00;
	
	//Read CHIP ID REG
	bmi160Read(BMI160_CHIP_ID , &read , 1);
	
		//Check Cheap ID
	if(read != 0xD1) {
		sprintf(print , "$$$ ERROR, BMI160 BAD CHIP_ID: 0x%0.2X ,should be 0xD1\r\n" , read);
		ServUsart->writeString(ServUsart->usartHandle , print);
		return 1;
	}
	else {
		sprintf(print , "$$$ SPI3(BMI160) INIT OK CHIP_ID: 0x%0.2X \r\n" , read);
		ServUsart->writeString(ServUsart->usartHandle , print);
		return_value = 0;
	}
	
	for(bmi160_init_cnt = 0 ; bmi160_init_cnt <20 ; bmi160_init_cnt++)
	{

		//set acc range
		setAccRange(2);
	
		//bmi160Write(BMI160_ACC_CONF , &acc_800Hz , 1);
		//bmi160Write(BMI160_ACC_CONF , &acc_1600Hz , 1);
	
		HAL_Delay(10);
		
		bmi160DefaultConfiguration();

		bmi160Read(BMI160_PMU_STATUS , &read , 1);
		if(read & (1<<4)) break;
		
		cnt++;
	}
	
	bmi160Read(BMI160_ERR , &read , 1);
	sprintf(print , "\n\r$$$ ERR: 0x%0.2X , cnt:%d" , read ,cnt);
	ServUsart->writeString(ServUsart->usartHandle ,print);
	
	bmi160Read(BMI160_PMU_STATUS , &read , 1);
	sprintf(print , "\n\r$$$ STATUS PMU: 0x%0.2X " , read);
	ServUsart->writeString(ServUsart->usartHandle ,print);
	
	bmi160Read(BMI160_STATUS , &read , 1);
	sprintf(print , "\n\r$$$ STATUS: 0x%0.2X " , read);
	ServUsart->writeString(ServUsart->usartHandle ,print);
	
	bmi160Read(BMI160_ACC_CONF , &read , 1);
	sprintf(print , "\n\r$$$ ACC CONF: 0x%0.2X " , read);
	ServUsart->writeString(ServUsart->usartHandle ,print);
	
	//Set No Motion interrupt
	//setNoMotionInt();
	
	//fifoConfig();
	
	//tim_16_init();
	
	return return_value;
}

/**
  * @brief  Read bmi160 register
	* @param  addr_reg : adres of register
	*         pData    : Pointer to store read data
	*					Size     : Size of data to read
  */
void bmi160Read(uint8_t addr_reg , uint8_t* pData , uint8_t Size)
{
	addr_reg |= 0x80;																		
	
	LL_GPIO_ResetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN); 		

	HAL_SPI_Transmit(&spi3 , &addr_reg , 1 , 1000);
		
	HAL_SPI_Receive(&spi3, (uint8_t *)pData , Size , 1000 );	
	
	//HAL_SPI_TransmitReceive(&spi3, &addr_reg, pData , 2, 100);
	
	LL_GPIO_SetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN); 		
}

/**
  * @brief  BMI160 Write Reg
	* @param  addr_teg : adres of register to write
	*  				pData    : Pionter to data
	*					Size		 : Size of data to send
  */
void bmi160Write(uint8_t addr_reg , uint8_t* pData , uint8_t Size)
{
	LL_GPIO_ResetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN);
	
	HAL_SPI_Transmit(&spi3 , &addr_reg , 1 , 1000);
	HAL_SPI_Transmit(&spi3 , pData , 1 ,1000);
	
	LL_GPIO_SetOutputPin(BMI160_CS_PORT , BMI160_CS_PIN);
}

/**
  * @brief  Default config for bmi160
  */
void bmi160DefaultConfiguration(void)
{
	uint8_t acc_pmu_normal = 0x11;

	//Power up acc
	bmi160Write(BMI160_CMD, &acc_pmu_normal , 1);
	HAL_Delay(10);
}	
 
/**
  * @brief  Set range of accelerometer
	* @param  lsb: range of acc it can be ( 2 , 4 , 8 or 16 )
  */
void setAccRange(uint8_t lsb)
{
	uint8_t acc_range_2  = 0x03;
	uint8_t acc_range_4  = 0x05;
	uint8_t acc_range_8  = 0x08;
	uint8_t acc_range_16 = 0x0C;
	
	switch(lsb)
	{
		case 2:
			acc_lsb = LSB_2g;
			bmi160Write(BMI160_ACC_RANGE , &acc_range_2 , 1); 
		break;
		
		case 4:
			acc_lsb = LSB_4g;
			bmi160Write(BMI160_ACC_RANGE , &acc_range_4 , 1); 
		break;
		
		case 8:
			acc_lsb = LSB_8g;
			bmi160Write(BMI160_ACC_RANGE , &acc_range_8 , 1); 
		break;
		
		case 16:
			acc_lsb = LSB_16g;
			bmi160Write(BMI160_ACC_RANGE , &acc_range_16 , 1); 
		break;
	}
}

/**
  * @brief  Read bmi160 accelerometer values
  * @param  acc_x, acc_y and acc_y are pointers to store acc data
  */
void bmi160ReadAcc(int16_t *acc_x , int16_t *acc_y , int16_t *acc_z)
{
	uint8_t read, read1 , read2;
	bmi160Read( BMI160_ACC_X , (uint8_t *)acc.acc_buf , 6);
	
	acc.acc_x = (acc.acc_buf[1] << 8) | acc.acc_buf[0];
	acc.acc_y = (acc.acc_buf[3] << 8) | acc.acc_buf[2];
	acc.acc_z = (acc.acc_buf[5] << 8) | acc.acc_buf[4];
	
	acc.X = (acc.acc_x * acc_lsb)/1000000;												
	acc.Y = (acc.acc_y * acc_lsb)/1000000;
	acc.Z = (acc.acc_z * acc_lsb)/1000000;
	
	acc.acc_g[0] = (float)sqrt(acc.X*acc.X + acc.Y*acc.Y + acc.Z*acc.Z);
	
	bmi160Read(BMI160_INT_STATUS0 , &read , 1);
	bmi160Read(BMI160_INT_STATUS1 , &read1 , 1);
	bmi160Read(BMI160_INT_STATUS2 , &read2 , 1);
	
	sprintf(print_acc,
	"$$$ X: %f [g], Y: %f [g], Z: %f [g] , G: %f [g] , INT0: 0x%0.2X  , INT1: 0x%0.2X , INT2: 0x%0.2X\r\n", 
	acc.X , acc.Y , acc.Z , acc.acc_g[0] , read, read1 , read2);
	
	ServUsart->writeString(ServUsart->usartHandle ,print_acc);
}

/**
  * @brief  Read bmi160 FUNCTION FOR TESTS
  */
void bmi160ReadAccTEST(void)
{
	uint8_t read, read1 , read2;
	bmi160Read( BMI160_ACC_X , (uint8_t *)acc.acc_buf , 6);
	
	acc.acc_x = (acc.acc_buf[1] << 8) | acc.acc_buf[0];
	acc.acc_y = (acc.acc_buf[3] << 8) | acc.acc_buf[2];
	acc.acc_z = (acc.acc_buf[5] << 8) | acc.acc_buf[4];
	
	acc.X = (acc.acc_x * acc_lsb)/1000000;												
	acc.Y = (acc.acc_y * acc_lsb)/1000000;
	acc.Z = (acc.acc_z * acc_lsb)/1000000;
	
	acc.acc_g[0] = (float)sqrt(acc.X*acc.X + acc.Y*acc.Y + acc.Z*acc.Z);
	
	bmi160Read(BMI160_INT_STATUS0 , &read , 1);
	bmi160Read(BMI160_INT_STATUS1 , &read1 , 1);
	bmi160Read(BMI160_INT_STATUS2 , &read2 , 1);
	
	sprintf(print_acc,
	"$$$ %d %d %d\r\n", acc.acc_x , acc.acc_y , acc.acc_z);
	
	ServUsart->writeString(ServUsart->usartHandle ,print_acc);
}

/**
  * @brief  Set Significant Motion Interrupt in BMI160
  */
void setSigMotionInt(void)
{
	uint8_t int_motion3 	= 0x12;
	uint8_t int_map0 			= 0x04;
	uint8_t	int_out_ctrl 	= 0x08;
	uint8_t int_en 				= 0x07;
	
	//set siginificant motion interrupt enable
	bmi160Write(BMI160_INT_MOTION3 , &int_motion3 , 1);
	
	//map siginificant motion to INT 1 pin
	bmi160Write(BMI160_INT_MAP0 , &int_map0 , 1);
	
	//set INT1 Pin output
	bmi160Write(BMI160_INT_OUT_CTRL , &int_out_ctrl , 1);
	
	//set INT eN
	bmi160Write(BMI160_INT_EN0 , &int_en , 1);
}

/**
  * @brief  Set anymotion detection interrupt in bmi160
  */
void setAnyMotionInt(void)
{
	uint8_t int_motion1 	= 0x40; //0x40 = 64 (64 * 15.63 = 1000mg) Threshold
	uint8_t int_reset     = 0xB1;
	uint8_t int_ctrl 			= 0x08;
	uint8_t int_en0				= 0x07;
	uint8_t int_map0			= 0x04;
	
	//Reset interrupt 
	bmi160Write(BMI160_CMD , &int_reset , 1);
	
	//set ant motion int config
	bmi160Write(BMI160_INT_MOTION1 , &int_motion1 , 1);
	
	//ustawienie wyjscia
	bmi160Write(BMI160_INT_OUT_CTRL, &int_ctrl ,1);

	//zmapownaie int do int 1 pin
	bmi160Write(BMI160_INT_MAP0 , &int_map0 , 1);
	
	//uruchomienie przerwania
	bmi160Write(BMI160_INT_EN0 , &int_en0 , 1);
}

/**
  * @brief  Set no motion detection interrupt in bmi160
  */
void setNoMotionInt(void)
{
	uint8_t int_reset = 0xB1;
	uint8_t motion0	 	= 0x40;  //0x40 Duration of no motion int 10 [s]
	uint8_t motion2 	= 0x3D;	 //0x3D = 61 ( 61 * 15.63 mg = 950 mg) Threshold lvl
	uint8_t motion3 	= 0x01;
	uint8_t ctrl    	= 0x08;
	uint8_t map0    	= 0x08;
	uint8_t en2     	= 0x07;
	
	//interrupt reset
	bmi160Write(BMI160_CMD , &int_reset , 1);
	
	//set time of no motion
	bmi160Write(BMI160_INT_MOTION0 , &motion0 , 1);
	
	//set threshould of no motion
	bmi160Write(BMI160_INT_MOTION2 , &motion2 , 1);
	
	//set no motion on
	bmi160Write(BMI160_INT_MOTION3 , &motion3 , 1);
	
	//set output for INT2
	bmi160Write(BMI160_INT_OUT_CTRL , &ctrl , 1);
	
	//set maping fot int 2
	bmi160Write(BMI160_INT_MAP0 , &map0 , 1);
	
	//set no motion interrup enable
	bmi160Write(BMI160_INT_EN2 , &en2 , 1);
}


/**
  * @brief  No Motion Detected
  */
void bmi160IntFromInt1(void)
{
	uint8_t read[2];
	uint8_t int_reset = 0xB1;
	
	bmi160Read(BMI160_INT_STATUS0 , read , 2);
	
	//reset int
	bmi160Write(BMI160_CMD , &int_reset , 1);
	
	ServUsart->writeString(ServUsart->usartHandle ,"$$$ Bmi160 Interrupt\r\n");
	
	if(read[1] & 0x80)		//noMotion Int
	{
		ServUsart->writeString(ServUsart->usartHandle ,"$$$ No Motion Interrupt\r\n");
		setAnyMotionInt();
		no_motion_flag = 1;
	}
	
	if(read[0] & 0x04)		//anyMotionInt
	{
		ServUsart->writeString(ServUsart->usartHandle ,"$$$ Any motion interrupt\r\n");
		setNoMotionInt();
		no_motion_flag = 0;
	}
}


/**
  * @brief  Configuration of Fifo
  */
void fifoConfig(void)
{
	uint8_t int_reset = 0xB1;
	uint8_t bmi160_fifo_flush = 0xB0;
	uint8_t fifo_config = 0x40;
	
	//reset int
	bmi160Write(BMI160_CMD , &int_reset , 1);
	
	//Flush fifo
	bmi160Write(BMI160_CMD , &bmi160_fifo_flush , 1);
	
	//Set enable Fifo for acc in header les mode.
	bmi160Write(BMI160_FIFO_CONFIG1 , &fifo_config , 1);
}

/**
  * @brief  Reading acc data from fifo
  */
void bmi160FifoAccRead(void)
{
	uint8_t fifo_length[2];
	
	acc.acc_g_size = 0;
	
	//Read fifo lvl 
	bmi160Read(BMI160_FIFO_LENGTH0 , fifo_length , 2);
	
	fifo_length[1] = (fifo_length[1] << 5);
	fifo_length[1] = (fifo_length[1] >> 5);
	
	acc.fifo_lvl = ((fifo_length[1] << 8 ) | (fifo_length[0]));
	
	acc.fifo_v = (acc.fifo_lvl/6);
	
	bmi160Read(BMI160_FIFO_DATA , (uint8_t *)acc.acc_fifo_read , acc.fifo_lvl );
}
	
/**
  * @brief  Result G vector
  */
void bmi160ResultG(void)
{
	uint16_t cnt = 0, cnt_buf = 0;
	
	for(cnt_buf = 0 ; cnt_buf < acc.fifo_v - 1 ; cnt_buf++)
	{
		if(cnt > acc.fifo_lvl) break;
		
		acc.acc_x = (acc.acc_fifo_read[1 + cnt] << 8) | acc.acc_fifo_read[0 + cnt];
		acc.acc_y = (acc.acc_fifo_read[3 + cnt] << 8) | acc.acc_fifo_read[2 + cnt];
		acc.acc_z = (acc.acc_fifo_read[5 + cnt] << 8) | acc.acc_fifo_read[4 + cnt];
		
		acc.X = (acc.acc_x * acc_lsb)/1000000;												
		acc.Y = (acc.acc_y * acc_lsb)/1000000;
		acc.Z = (acc.acc_z * acc_lsb)/1000000;
	
		acc.acc_g[cnt_buf] = (float)sqrt(acc.X*acc.X + acc.Y*acc.Y + acc.Z*acc.Z);
		acc.acc_g_size++;
		
		cnt+=6;		
	}
}


/**
  * @brief  Timer 16 Init function
*/
void tim_16_init(void)
{
	uint8_t bmi160_fifo_flush = 0xB0;
	
	__HAL_RCC_TIM16_CLK_ENABLE();
	
	tim16.Instance = TIM16;
	tim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim16.Init.Prescaler = 2000 - 1;
	tim16.Init.Period = 	8000 - 1;		//4 Hz
	
	HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, TIM16_NVIC_PRIORITY , 0);
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
	
	__HAL_TIM_ENABLE_IT(&tim16, TIM_IT_UPDATE);
	
	bmi160Write(BMI160_CMD , &bmi160_fifo_flush , 1);
	
	if(HAL_TIM_Base_Init(&tim16) != HAL_OK){
		ServUsart->writeString(ServUsart->usartHandle ,"$$$ error:bmi160.c(400):HAL_TIM_Base_Init\r\n");
	}
	__HAL_TIM_ENABLE(&tim16);
	
	ServUsart->writeString(ServUsart->usartHandle ,"$$$ TIM4 INIT OK\r\n");
}

/**
  * @brief  Timer 16 interrupt function
*/
void TIM1_UP_TIM16_IRQHandler(void)
{
	/* UPDATE INTERRUPT */
	if(__HAL_TIM_GET_FLAG(&tim16, TIM_SR_UIF))			
	{
		__HAL_TIM_CLEAR_FLAG(&tim16, TIM_SR_UIF); 
	
		bmi160FifoAccRead();  // ok 300 [us]
		bmi_read_flag = 1;	
	}
	
}

/**
  * @brief  Read from fifo, analyze, and save to SD
  */
void bmi160Analyze(void)
{
//	uint16_t cnt;
//	
//	acc_done_flag = 0;
//	
//	bmi160ResultG();      // ok 138 [us]
//	
//	//Srednia kroczaca
//	for(cnt = 0 ; cnt < acc.acc_g_size ; cnt++) // 45 [us]
//	{		
//		acc.acc_g_sre[cnt] = sre(acc.acc_g[cnt]);
//		rawData = acc.acc_g[cnt];
//		filteredData = acc.acc_g_sre[cnt];
//		
//		if(filteredData > ((float)1.0)) threshold_flag++;
//		else threshold_flag = 0x00;
//		
//		if(threshold_flag == 2)
//		{
//			threshold_flag = 0;
//			threshold_detection = 0x01;
//			threshold_cnt = cnt;
//		}
//		else threshold_detection = 0;
//	}
//	
//	//save data // 0.8 [us]
//	if(threshold_detection == 0x01)
//	{
//		uint8_t cnt = 0;
//		threshold_detection = 0x00;
//	
//		sprintf(print_acc , "$$$ Motion: %f Time: %s>>  \r\n\r\n" , 
//			acc.acc_g_sre[threshold_cnt] , gps_nmea.UTC_time);
//		
//		while(print_acc[cnt] != NULL)
//		{
//			buf_gps_fix[buf_gps_fix_cnt] = print_acc[cnt];
//			
//			buf_gps_fix_cnt++;
//			cnt++;
//			
//			if((cnt == sizeof(print_acc)) || (print_acc[cnt] == NULL)) break;
//		}		

//	}
//	
//		//Flush bufers 									// 5.8 [us]
//		memset((void *)acc.acc_fifo_read , 0 , sizeof(acc.acc_fifo_read));
//		memset((void *)acc.acc_g , 0 , sizeof(acc.acc_g));
//		memset((void *)acc.acc_g_sre , 0 , sizeof(acc.acc_g_sre));
//	
//	acc_done_flag = 1;
//	if(acc_done_flag == 1 && gps_done_flag == 1) go_to_sleep_mode = 1;

}


/**
  * @brief  Srednia kroczaca
  * @param  wynik = new sample
	*   			sre = variable to save result
  */
float sre(float sample)
{
	average = average * dt;
	average = average + sample;
	average = average / (dt + 1);
	
	return average;
}
