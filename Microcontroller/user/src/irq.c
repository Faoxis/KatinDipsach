#include "irq.h"


extern bool startPlatform;

extern bool startPulse;

extern uint8_t pulseCounter;

uint8_t globalData = (uint8_t) 0;

bool flag_zeroPassed = false;

uint8_t distanceBuffer[50];
uint8_t indexDistanceBuffer = 0;


void USART1_IRQHandler(void) {
	uint8_t data;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data = (uint8_t) USART_ReceiveData(USART1);

		if (data == 228) {
			startPlatform = true;
			runPulse();
			pulseCounter++;
		} else if (data == 229) {
			startPlatform = false;
		}
	}
}


void USART2_IRQHandler(void) {
	uint8_t data;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		data = (uint8_t) USART_ReceiveData(USART2);
		globalData = data;
		distanceBuffer[indexDistanceBuffer] = data;
		indexDistanceBuffer++;
		if (indexDistanceBuffer >= sizeof(distanceBuffer) / sizeof(distanceBuffer[0])) indexDistanceBuffer = 0;
	}
}


// Обработчик прерывания события с кольцевого датчика - 0 градусов
void EXTI0_IRQHandler(void) {
//	GPIOC->ODR^=GPIO_Pin_8; //Инвертируем состояние светодиода
	flag_zeroPassed = true;
	pulseCounter = 0;
	EXTI->PR|=0x01; //Очищаем флаг
}


// Обработчик прерывания события с кольцевого датчика - 180 градусов
void EXTI1_IRQHandler(void) {
//	GPIOC->ODR^=GPIO_Pin_9; //Инвертируем состояние светодиода
	flag_zeroPassed = false;
	pulseCounter = 210;
	EXTI->PR|=0x02; //Очищаем флаг
}



/************************************* *******************************************/
/************************************* *******************************************/
void TIM2_IRQHandler(void) {

	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
		startPulse = true;

		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
	} else if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		uint16_t sum = 0;
		for (int i = 0; i < sizeof(distanceBuffer) / sizeof(distanceBuffer[0]); ++i) {
			sum += distanceBuffer[i];
		}

		USART_SendData(USART1, sum / (sizeof(distanceBuffer) / sizeof(distanceBuffer[0])));

		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);


		if ((pulseCounter < 210) && (startPlatform)) {
			runPulse();
			pulseCounter++;
		}

		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}

}


//--------------------- Настройка таймера на прерывание ------------------------//
void TIM6_DAC_IRQHandler() {
	static uint8_t flag = 0; //

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET) { // Проверяем, что прерывание пришло от таймера
		USART_SendData(USART_DISTANCE, 0xAAU);
		while(USART_GetFlagStatus(USART_DISTANCE, USART_FLAG_TC) == RESET);



		/* Очищаем бит обрабатываемого прерывания */
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
//------------------------------------------------------------------------------//

