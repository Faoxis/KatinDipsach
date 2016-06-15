#include "input.h"


void setInitInterrapt() {

	//����������� ��� ��� �����
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);

	//���������� - ��� �������������� ������� �����
	//������� ���� ���������� ��� Alternate function I/O clock enable
	//� �������� RCC_APB2ENR
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN , ENABLE);

	//���� ������ �������� � �������� EXTICR[0] ����� ���������� ���
	// 0000 0000 0001 0000
	// �� ��������� ��� ����, ������� ��������� ������ 1 ���
	AFIO->EXTICR[0]|=AFIO_EXTICR1_EXTI1_PA;

	//���������� �� ������� � ������ ���� ���������
	EXTI->IMR|=(EXTI_IMR_MR0 | EXTI_IMR_MR1);

	//���������� �� ����� ����� �� ������������ ������
	EXTI->RTSR|=(EXTI_RTSR_TR0 | EXTI_RTSR_TR1);

	//��������� ��� ����������
	NVIC_EnableIRQ (EXTI0_IRQn);
	NVIC_EnableIRQ (EXTI1_IRQn);


}
