#include "pins_init.h"

//----------------------------- ��������� ����� ������ ������� �������� ------------------------------------//
void initDIRPins(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef pinDir;
	pinDir.GPIO_Pin = GPIO_Pin_1;
	pinDir.GPIO_Mode = GPIO_Mode_Out_PP;
	pinDir.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &pinDir);

}


//------------------------------- ��������� ����� �� ���������� -------------------------------------------//
void initInterruptPins(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	GPIO_InitTypeDef pin;
	pin.GPIO_Mode = GPIO_Mode_IPD;
	pin.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;
	pin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &pin);



	//���������� - ��� �������������� ������� �����
	//������� ���� ���������� ��� Alternate function I/O clock enable
	//� �������� RCC_APB2ENR
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN , ENABLE);


	//���� ������ �������� � �������� EXTICR[0] ����� ���������� ���
	// 0000 0000 0001 0000
	// �� ��������� ��� ����, ������� ��������� ������ 1 ���
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PB; // ������ ������� ������� - 4 �����
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB;

	//���������� �� ������� � ������ ���� ���������
	EXTI->IMR |= (EXTI_IMR_MR0 | EXTI_IMR_MR1);

	//���������� �� ����� ����� �� ������������ ������
	EXTI->RTSR |= (EXTI_RTSR_TR0 | EXTI_RTSR_TR1);

	//��������� ��� ����������
	NVIC_EnableIRQ (EXTI0_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);
}




