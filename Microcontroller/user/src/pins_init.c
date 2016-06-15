#include "pins_init.h"

//----------------------------- Настройка ножки выбора стороны поворота ------------------------------------//
void initDIRPins(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef pinDir;
	pinDir.GPIO_Pin = GPIO_Pin_1;
	pinDir.GPIO_Mode = GPIO_Mode_Out_PP;
	pinDir.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &pinDir);

}


//------------------------------- Настройка ножек на прерывание -------------------------------------------//
void initInterruptPins(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	GPIO_InitTypeDef pin;
	pin.GPIO_Mode = GPIO_Mode_IPD;
	pin.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
	pin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &pin);



	//Прерывания - это альтернативная функция порта
	//поэтому надо установить бит Alternate function I/O clock enable
	//в регистре RCC_APB2ENR
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN , ENABLE);


	//Наша задача получить в регистре EXTICR[0] такую комбинацию бит
	// 0000 0000 0001 0000
	// по умолчанию там ноли, поэтому установим только 1 бит
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB; // Каждый элемент массива - 4 ножки
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB;

	//Прерывания от нулевой и первой ноги разрешены
	EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1);

	//Прерывания на обоих ногах по нарастающему фронту
	EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1);

	//Разрешаем оба прерывания
	NVIC_EnableIRQ (EXTI0_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);
}




