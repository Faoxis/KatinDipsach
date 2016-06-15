#ifndef USART_INIT_H
#define USART_INIT_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"


void initUSARTForCom(void);
void initUSARTForDevice(void);

#endif
