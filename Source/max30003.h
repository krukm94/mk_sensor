//NAME: 	"max30003.h"
//AUTHOR:	M.KRUK
//DATE 	:	IV.2018

#ifndef __max30003_H_
#define __max30003_H_

#include "init.h"

// >>>>>>>>>>>> DEFINES
#define MAX_INFO_REG				((uint8_t) 0x0F)
#define MAX_SW_RST					((uint8_t) 0x08)
#define MAX_NO_OP						((uint8_t) 0x00)


uint8_t maxInit(void);
int32_t max30003ReadReg (int8_t reg_addr);
void max30003WriteReg (int8_t reg_addr, int32_t reg_val);

#endif //__max30003_H_

