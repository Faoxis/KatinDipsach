#include "input.h"


void setInitInterrapt() {

	//Затактируем все три порта
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

	//Прерывания - это альтернативная функция порта
	//поэтому надо установить бит Alternate function I/O clock enable
	//в регистре RCC_APB2ENR
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN , ENABLE);

	//Наша задача получить в регистре EXTICR[0] такую комбинацию бит
	// 0000 0000 0001 0000
	// по умолчанию там ноли, поэтому установим только 1 бит
	AFIO->EXTICR[0]|=AFIO_EXTICR1_EXTI1_PA;

	//Прерывания от нулевой и первой ноги разрешены
	EXTI->IMR|=(EXTI_IMR_MR0 | EXTI_IMR_MR1);

	//Прерывания на обоих ногах по нарастающему фронту
	EXTI->RTSR|=(EXTI_RTSR_TR0 | EXTI_RTSR_TR1);

	//Разрешаем оба прерывания
	NVIC_EnableIRQ (EXTI0_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);


}
