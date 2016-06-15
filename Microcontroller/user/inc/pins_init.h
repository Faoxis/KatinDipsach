#ifndef OUTPUT_H
#define OUTPUT_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"

void initDIRPins(void);
void initInterruptPins(void);

#endif
