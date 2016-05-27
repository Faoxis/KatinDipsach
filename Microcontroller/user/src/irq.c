#include "irq.h"

extern uint8_t sine[255];
bool start = false;

void USART1_IRQHandler(void) {
	uint8_t data;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = (uint8_t) USART_ReceiveData(USART1);
		if (data == 0x73U) {
			start = true;
		}
	}
}

void TIM6_DAC_IRQHandler(){

	static uint8_t counter = 0;

	// ��������� �� ������� �� ����������
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) {

//    	if (start) {
    		USART_SendData(USART1, sine[counter]);
    		counter++;
//    	}


    /* ������� ��� ��������������� ���������� */
      TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}


// ���������� ���������� EXTI0
void EXTI0_IRQHandler(void) {
//	GPIOC->ODR^=GPIO_Pin_8; //����������� ��������� ����������
	EXTI->PR|=0x01; //������� ����
}


// ���������� ���������� EXTI1
void EXTI1_IRQHandler(void) {
//	GPIOC->ODR^=GPIO_Pin_9; //����������� ��������� ����������
	EXTI->PR|=0x02; //������� ����
}



