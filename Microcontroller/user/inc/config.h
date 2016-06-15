#ifndef CONFIG_H
#define CONFIG_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stdbool.h"
#include "config.h"


#define DIR_TOWARDS()   GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET)
#define DIR_BACK()      GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET)
#define runPulse()      TIM_Cmd(TIM2, ENABLE)

#define USART_COM       USART1
#define USART_DISTANCE  USART2


void clearDataFromDevice(void);
uint8_t getDistance(void);


#endif
