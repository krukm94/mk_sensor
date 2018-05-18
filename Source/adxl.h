//NAME: 	"adxl.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#ifndef __adxl_H_
#define __adxl_H_

#include "init.h"

// >>>>>>>>> definitions
#define ADXL_CHIP_ID				((uint8_t)0x00)

#define ADXL_FIFO_ENTRIES		((uint8_t)0x05)
#define ADXL_X_DATA					((uint8_t)0x08)
#define ADXL_Y_DATA					((uint8_t)0x0B)
#define ADXL_Z_DATA					((uint8_t)0x0E)

#define ADXL_FIFO_DATA			((uint8_t)0x11)

#define ADXL_POWER					((uint8_t)0x2D)
#define ADXL_ODR_LPF				((uint8_t)0x28)
#define ADXL_RANGE					((uint8_t)0x2C)
#define ADXL_SYNC						((uint8_t)0x2B)
#define ADXL_INT_MAP				((uint8_t)0x2A)
#define ADXL_FIFO_SIZE			((uint8_t)0x29)
#define ADXL_RESET					((uint8_t)0x2F)
#define ADXL_SELF_TEST			((uint8_t)0x2E)

// REG Values
#define RANGE_8G						((uint8_t)0x03)
#define RANGE_4G						((uint8_t)0x02)
#define RANGE_2G						((uint8_t)0x01)

#define FIFO_FULL_ON_INT2		((uint8_t)0x20)
#define FIFO_FULL_90_SIZE		((uint8_t)0x5A)

#define LSB_8g 							15.258789				//15.25878906 ug
#define LSB_4g							7.629394531				//7.629394531 ug
#define LSB_2g							3.814697266				//3.814697266 ug

//Settings
typedef struct{
		uint8_t 	acc_buf[288];				//Buf for temp Read
		uint8_t		Win1FullFlag;				
		uint8_t		Win2FullFlag;
		int32_t 	acc_x;
		int32_t 	acc_y;
		int32_t 	acc_z;
		int32_t   Win1_axisX[512];		//Window 1 Axis X
		int32_t		Win2_axisX[512];		//Window 2 Axis x
		int32_t   Win1_axisY[512];		//Window 1 Axis Y
		int32_t		Win2_axisY[512];		//Window 2 Axis Y
		int32_t   Win1_axisZ[512];		//Window 1 Axis Z
		int32_t		Win2_axisZ[512];		//Window 2 Axis Z
		uint16_t	Win1WritePtr;				//Pointer to write to win1
		uint16_t	Win2WritePtr;				//Pointer to write to win2
		uint16_t	Win1ReadPtr;				//Pointer to read win1
		uint16_t	Win2ReadPtr;				//Pointer to read win2
		float     X;
		float		  Y;
	  float			Z;	
		float			Win1_Acc[512]; 			//Accelration (accidental vector :) ) value Vin 1
		float			Win2_Acc[512];			//Accelration (accidental vecotr :) ) value Vin 2
		float			acc_g[40];
} acc_axis_adxl;

// >>>>>>>>> functions
uint8_t adxlInit(void);
void adxlRead(uint8_t addr_reg , uint8_t* pData , uint8_t Size);
void adxlWrite(uint8_t addr_reg , uint8_t* pData , uint8_t Size);

void adxlReadAcc(int32_t *acc_x , int32_t *acc_y , int32_t *acc_z);
void adxlDataProces(void);
void winAnalyze(uint8_t winNr);

void spiADXLWriteByte (int8_t byte);
int8_t spiADXLReadByte (void);
int8_t spiADXLWriteReadByte (int8_t byte);

void adxlWriteTS(uint8_t addr_reg , uint8_t wrData);
void adxlReadTS(uint8_t addr_reg , uint8_t* pData , uint8_t Size);


#endif //__adxl_H_

