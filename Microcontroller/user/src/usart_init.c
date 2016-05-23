#include "usart_init.h"


void setInitUSART(void) {

	// Pins setting
	GPIO_InitTypeDef MyPin;
	GPIO_StructInit(&MyPin); // Настройка структуры по умолчанию

	// RCC setting
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// PA10 = RX - usart inputs
	MyPin.GPIO_Pin = GPIO_Pin_10;
	MyPin.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &MyPin);

	// PA9 = TX - Настроить на вход
	MyPin.GPIO_Pin = GPIO_Pin_9;
	MyPin.GPIO_Mode = GPIO_Mode_AF_PP;
	MyPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &MyPin);

	USART_InitTypeDef myUSART;
	USART_StructInit(&myUSART);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	myUSART.USART_BaudRate            = 115200;
	myUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myUSART.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	myUSART.USART_Parity              = USART_Parity_No;
	myUSART.USART_StopBits            = USART_StopBits_1;
	myUSART.USART_WordLength          = USART_WordLength_8b;

	USART_Init(USART1, &myUSART);
	USART_Cmd(USART1, ENABLE);

	__enable_irq();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_SetPriority(USART1_IRQn, 7);
	NVIC_EnableIRQ(USART1_IRQn);

}
