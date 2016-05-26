#include "output.h"

void setInitDIR(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef pinDir;
	pinDir.GPIO_Pin = GPIO_Pin_1;
	pinDir.GPIO_Mode = GPIO_Mode_Out_PP;
	pinDir.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &pinDir);

}

