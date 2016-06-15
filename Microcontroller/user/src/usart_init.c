#include "usart_init.h"


void initUSARTForCom(void) {

	// Pins setting
	GPIO_InitTypeDef MyPin;
	GPIO_StructInit(&MyPin); // Настройка структуры по умолчанию

	// RCC setting
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// PA10 = RX - usart inputs
	MyPin.GPIO_Pin = GPIO_Pin_10;
	MyPin.GPIO_Mode = GPIO_Mode_IN_FLOATING;
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

void initUSARTForDevice(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Разрешаем тактирование
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


	GPIO_InitTypeDef myPin;

	myPin.GPIO_Pin   = GPIO_Pin_2;
	myPin.GPIO_Mode  = GPIO_Mode_AF_PP;
	myPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &myPin);

	myPin.GPIO_Pin   = GPIO_Pin_3;
	myPin.GPIO_Mode  = GPIO_Mode_IPU;
	myPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &myPin);

	  /* Стандартные настройки COM порта ПК----------------------------------*/
	  /* USART2 configured as follow:
	  - BaudRate = 151800 baud
	  - Word Length = 8 Bits
	  - Two Stop Bit
	  - No parity
	  - Hardware flow control disabled (RTS and CTS signals)
	  - Receive and transmit enabled
	  */

	USART_InitTypeDef myUSART;
	//USART_StructInit(&myUSART);

	myUSART.USART_BaudRate            = 151800;
	myUSART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	myUSART.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	myUSART.USART_Parity              = USART_Parity_No;
	myUSART.USART_StopBits            = USART_StopBits_2;
	myUSART.USART_WordLength          = USART_WordLength_8b;

	USART_Init(USART2, &myUSART); //инизиализируем
	USART_Cmd(USART2, ENABLE);

	__enable_irq();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	NVIC_SetPriority(USART2_IRQn, 7);
	NVIC_EnableIRQ(USART2_IRQn);
}





