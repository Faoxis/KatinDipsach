#include "timer_init.h"

void setInitTimer(void) {

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	TIM_TimeBaseInitTypeDef base_timer;

	base_timer.TIM_Prescaler = 240 - 1;
	base_timer.TIM_Period = 1000;
	TIM_TimeBaseInit(TIM6, &base_timer);

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);

	NVIC_EnableIRQ(TIM6_DAC_IRQn);

}

void setInitPWM(void) {
	//1-�� ����� ��� ��������� ����� ��� ������ ��� ��������� ������� 2

	    // ���������� ��������� ����� ��� ������ � �������������� ������
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); // ��� ������� ���������� ������������ ����� A
	    GPIO_InitTypeDef Pinyshka; // ������ ���������� (���������) ��� �����

	    // ��������� ���� ��������� ��� ����������� ������ �����(������� ����������� 2-�� �����)
	    Pinyshka.GPIO_Pin = GPIO_Pin_0;  // ���������� �����!!! ����� (��� 1 ����� ������� 2 �� ���������)
	    Pinyshka.GPIO_Mode = GPIO_Mode_AF_PP; // ���������� ����� ������ �����
	    Pinyshka.GPIO_Speed = GPIO_Speed_50MHz; // ��� ��� ����� �� ����� �� ���������� �������� ������ �����

	    // ���� �������� �� ���������� ������ ����� (������ �� ����������� ��������� �����) + (��������� ���� ����������� ��� �����)
	    GPIO_Init( GPIOA , &Pinyshka);

	//2-�� ����� ��� ��������� ������� 2
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // �������� ������������ �������

	    /* ����������� ����������
	     1) TIM_TimeBaseInitTypeDef -- �������� ��������� �������
	     2) TIM_OCInitTypeDef  -- ��������� ��� ���������� �������� ����������� ��� �����
	     3) TIM_ICInitTypeDef  -- ��������� ��� ���������� �������� ����������� ��� ����
	     4) TIM_BDTRInitTypeDef  -- �������������� ������ ���������    */

	    // ������� ��������� �������
	    TIM_TimeBaseInitTypeDef Timerushka_Base; // ������ ���������� (���������) ��� ����������� ������ ������ �������
	    Timerushka_Base.TIM_CounterMode = TIM_CounterMode_Up; // �������� ����� ������ ��������
	    Timerushka_Base.TIM_Prescaler = 24000 - 1; // ���� ��������
	    Timerushka_Base.TIM_Period = 3000; // ������ //
	   //
	    Timerushka_Base.TIM_Period = 10; // ������
	    Timerushka_Base.TIM_ClockDivision = TIM_CKD_DIV1; // CKD[1:0]: Clock division ���������� ������� ��� �������� (tDTS)
	    // Timerushka_Base.TIM_RepetitionCounter = 1; //���� �������� ������ ��� �1 � �8
	    TIM_TimeBaseInit(TIM2, &Timerushka_Base); // ���������� �������� � ��������

	    /*�������� ����� ��������� ������� ��������
	    Timerushka_OCI.TIM_OCMode; // ��� TIMx_CCMR1 .���� OC2M[2:0] ����� ������ ����� ����������� �� �����
	    Timerushka_OCI.TIM_OutputState; //��� TIMx_��ER .��� CC1E ���������� ���� ����� � ������ �������
	    Timerushka_OCI.TIM_OutputNState; //  only for TIM1 and TIM8
	    Timerushka_OCI.TIM_Pulse; // ������� TIMx->CCR1 (����� ��������� � ������) ����� �����  0x0000 � 0xFFFF
	    Timerushka_OCI.TIM_OCPolarity; // ��� TIMx->CCER ��� CC1P ���������� ������
	    Timerushka_OCI.TIM_OCNPolarity; // only for TIM1 and TIM8
	    Timerushka_OCI.TIM_OCIdleState; // only for TIM1 and TIM8
	    Timerushka_OCI.TIM_OCNIdleState; // only for TIM1 and TIM8  */

	    // ��������� 1��� ������ �������
	    TIM_OCInitTypeDef Timerushka_OCI;// ������ ���������� (���������) ��� ����������� ������ ������ �������
	    // 1)Timerushka_OCI.TIM_OCMode = TIM_OCMode_Toggle; // ����� �������������� ����� ��� ���������� �������� � ��� ������
	    // 2)
	    Timerushka_OCI.TIM_OCMode = TIM_OCMode_PWM1; // ����� ��������� ����� ��� ���������� �������� � ��� ������
	    Timerushka_OCI.TIM_OutputState = TIM_OutputState_Enable; // ���������� ���� ����� � ������ �������
	    Timerushka_OCI.TIM_Pulse = 1000; // ������� TIMx->CCR1 (����� ��������� � ������) ����� �����  0x0000 � 0xFFFF
	    //
	    Timerushka_OCI.TIM_Pulse = 9;
	    Timerushka_OCI.TIM_OCPolarity = TIM_OCPolarity_High; // ��� TIMx->CCER ��� CC1P ���������� ������
	    TIM_OC1Init(TIM2, &Timerushka_OCI);

	    // ������ ����� ���������� !!!
	    TIM_Cmd(TIM2, ENABLE);
}

