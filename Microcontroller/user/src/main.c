#include "main.h"

void delay(uint32_t t) {
	for (uint32_t i = 0; i < t; ++i);
}

uint8_t var = 0x88U;

void USART1_IRQHandler(void) {
	uint8_t data;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = (uint8_t) USART_ReceiveData(USART1);
		USART_SendData(USART1, data);
	}
}

void TIM6_DAC_IRQHandler(){

	static uint8_t Flag = 0; // Определяем чет/нечетн вход в обработчик

	// Проверяем от таймера ли прерывание
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) {
      Flag = ~Flag; // инвертируем значение флага

      if(Flag) { //по флагу определяем ЛОГ уровни на светодиодах
  		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
  		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
  	  }
      else {
  		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
  		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
	  }
    /* Очищаем бит обрабатываемого прерывания */
      TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
    }
}


int main(void) {
	initAll();
	uint8_t i = 0;

	while(1) {
		var = (uint8_t) USART_ReceiveData(USART1);
		USART_SendData( USART1, var);
		delay(0xFFFFFU);
	}

	return 0;
}


