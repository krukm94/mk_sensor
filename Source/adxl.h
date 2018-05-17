//NAME: 	"adxl.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#ifndef __adxl_H_
#define __adxl_H_

#include "init.h"

// >>>>>>>>> definitions
#define ADXL_CHIP_ID				((uint8_t)0x00)

#define ADXL_X_DATA					((uint8_t)0x08)
#define ADXL_Y_DATA					((uint8_t)0x0B)
#define ADXL_Z_DATA					((uint8_t)0x0E)

#define ADXL_POWER					((uint8_t)0x2D)
#define ADXL_ODR_LPF				((uint8_t)0x28)
#define ADXL_RANGE					((uint8_t)0x2C)
#define ADXL_SYNC						((uint8_t)0x2B)
#define ADXL_RESET					((uint8_t)0x2F)
#define ADXL_SELF_TEST			((uint8_t)0x2E)

#define RANGE_8G						((uint8_t)0x03)
#define RANGE_4G						((uint8_t)0x02)
#define RANGE_2G						((uint8_t)0x01)

//Settings
typedef struct{
		uint8_t 	acc_buf[9];
		int32_t 	acc_x;
		int32_t 	acc_y;
		int32_t 	acc_z;
		float     X;
		float		  Y;
	  float			Z;	
		float			acc_g[40];
} acc_axis_adxl;


// >>>>>>>>> functions
uint8_t adxlInit(void);
void adxlRead(uint8_t addr_reg , uint8_t* pData , uint8_t Size);
void adxlWrite(uint8_t addr_reg , uint8_t* pData , uint8_t Size);

void adxlReadAcc(int32_t *acc_x , int32_t *acc_y , int32_t *acc_z);

void spiADXLWriteByte (int8_t byte);
int8_t spiADXLReadByte (void);
int8_t spiADXLWriteReadByte (int8_t byte);

void adxlWriteTS(uint8_t addr_reg , uint8_t wrData);
void adxlReadTS(uint8_t addr_reg , uint8_t* pData , uint8_t Size);


#endif //__adxl_H_

