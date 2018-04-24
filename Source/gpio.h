//NAME: 	"gpio.h"
//AUTHOR:	M.KRUK
//DATE:		VI.2018

#ifndef __gpio_H__
#define __gpio_H__

#include "init.h"

void 		gpioInit(void);

void 		ledInit(void);
void 		ledOn(uint8_t led_nr);
void 		ledOff(uint8_t led_nr);
void		ledToggle(uint8_t led_nr);

void pwrPinsInit(void);


#endif  //__gpio_H__
