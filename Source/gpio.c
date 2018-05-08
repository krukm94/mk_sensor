//NAME: 	"gpio.c"
//AUTHOR:	M.KRUK
//DATE:		VI.2018

#include "gpio.h"

/**
  * @brief  Gpios init
  */
void gpioInit(void)
{
	//RCC On
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	
	//Init pins for PWR managment
	pwrPinsInit();
	
	//Init Leds
	ledInit();
	
}

/**
  * @brief  Init PWR pins
  */
void pwrPinsInit(void)
{
	//PWR_MAINTAIN
	LL_GPIO_SetPinMode(PWR_MAINTAIN_PORT, PWR_MAINTAIN_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(PWR_MAINTAIN_PORT , PWR_MAINTAIN_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(PWR_MAINTAIN_PORT , PWR_MAINTAIN_PIN , LL_GPIO_PULL_DOWN);
	
	//SET PWR_MAINTAIN
	LL_GPIO_SetOutputPin(PWR_MAINTAIN_PORT , PWR_MAINTAIN_PIN);
	
	//VDD_12_ON  
	LL_GPIO_SetPinMode(VDD_12_ON_PORT, VDD_12_ON_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(VDD_12_ON_PORT , VDD_12_ON_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(VDD_12_ON_PORT , VDD_12_ON_PIN , LL_GPIO_PULL_DOWN);
	
	//SET VDD_12_ON Low  (It can be High only in Run, Sleep and Stop 0 mode)
	LL_GPIO_SetOutputPin(VDD_12_ON_PORT  , VDD_12_ON_PIN);
	
	//nCE_BQ24
	LL_GPIO_SetPinMode(nCE_BQ24_PORT, nCE_BQ24_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(nCE_BQ24_PORT , nCE_BQ24_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(nCE_BQ24_PORT , nCE_BQ24_PIN , LL_GPIO_PULL_DOWN);
	
	//nCE_BQ24 set High (Charger disable)
	LL_GPIO_ResetOutputPin(nCE_BQ24_PORT , nCE_BQ24_PIN);
	
	//nCH_BQ24
	LL_GPIO_SetPinMode(nCH_BQ24_PORT, nCH_BQ24_PIN , LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(nCH_BQ24_PORT , nCH_BQ24_PIN , LL_GPIO_PULL_DOWN);
	
	//User button
	LL_GPIO_SetPinMode(USER_BUTTON_PORT, USER_BUTTON_PIN , LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(USER_BUTTON_PORT , USER_BUTTON_PIN , LL_GPIO_PULL_UP);
}

/**
  * @brief  Init Leds
  */
void ledInit(void)
{
	//LED1
	LL_GPIO_SetPinMode(LED1_PORT, LED1_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(LED1_PORT , LED1_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(LED1_PORT , LED1_PIN , LL_GPIO_PULL_DOWN);
	
	//LED2
	LL_GPIO_SetPinMode(LED2_PORT, LED2_PIN , LL_GPIO_MODE_OUTPUT);
	LL_GPIO_SetPinOutputType(LED2_PORT , LED2_PIN , LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(LED2_PORT , LED2_PIN , LL_GPIO_PULL_DOWN);
	
	//Set leds OFF
	ledOff(1);ledOff(2);
}

/**
  * @brief  Set led ON
  */
void ledOn(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_SetOutputPin(LED1_PORT , LED1_PIN);
			break;
		case 2:
			LL_GPIO_SetOutputPin(LED2_PORT , LED2_PIN);
	}
}

/**
  * @brief  Set led off
  */
void ledOff(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_ResetOutputPin(LED1_PORT , LED1_PIN);
			break;
		case 2:
			LL_GPIO_ResetOutputPin(LED2_PORT , LED2_PIN);
			break;
	}
}

/**
  * @brief  Toggle led
  */
void ledToggle(uint8_t led_nr)
{
	switch(led_nr)
	{
		case 1:
			LL_GPIO_TogglePin(LED1_PORT , LED1_PIN);
			break;
		case 2:
			LL_GPIO_TogglePin(LED2_PORT , LED2_PIN);
			break;
	}
}


