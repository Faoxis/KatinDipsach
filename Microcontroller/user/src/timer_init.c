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
	//1-ая часть ЭТО настройка ножки для работы под контролем ТАЙМЕРА 2

	    // Необходимо настроить ножку для работы в альтернативном режиме
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); // это функция разрешения тактирования порта A
	    GPIO_InitTypeDef Pinyshka; // создаём переменную (структуру) для ножки

	    // заполняем поля структуры для определения режима ножки(ЗАМЕТИМ определение 2-ух ножек)
	    Pinyshka.GPIO_Pin = GPIO_Pin_0;  // определяем ножки!!! порта (это 1 канал таймера 2 по умолчанию)
	    Pinyshka.GPIO_Mode = GPIO_Mode_AF_PP; // определяем режим работы ножки
	    Pinyshka.GPIO_Speed = GPIO_Speed_50MHz; // так как режим на выход то определяем скорость работы ножки

	    // этой функцией мы определяем работу ножек (ссылка на заполненную структуру ножки) + (указываем порт необходимых нам ножек)
	    GPIO_Init( GPIOA , &Pinyshka);

	//2-ая часть ЭТО настройка ТАЙМЕРА 2
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // включаем тактирование таймера

	    /* Настроечные структурки
	     1) TIM_TimeBaseInitTypeDef -- Основные настройки таймера
	     2) TIM_OCInitTypeDef  -- Настройки для управление каналами настроенные как ВЫХОД
	     3) TIM_ICInitTypeDef  -- Настройки для управление каналами настроенные как ВХОД
	     4) TIM_BDTRInitTypeDef  -- Дополнительные разные настройки    */

	    // БАЗОВЫЕ настройки таймера
	    TIM_TimeBaseInitTypeDef Timerushka_Base; // создаём переменную (структуру) для определения режима работы таймера
	    Timerushka_Base.TIM_CounterMode = TIM_CounterMode_Up; // Выбираем режим работы счетчика
	    Timerushka_Base.TIM_Prescaler = 24000 - 1; // пред делитель
	    Timerushka_Base.TIM_Period = 3000; // период //
	   //
	    Timerushka_Base.TIM_Period = 10; // период
	    Timerushka_Base.TIM_ClockDivision = TIM_CKD_DIV1; // CKD[1:0]: Clock division определяем частоту для фильтров (tDTS)
	    // Timerushka_Base.TIM_RepetitionCounter = 1; //Этот параметр только для Т1 и Т8
	    TIM_TimeBaseInit(TIM2, &Timerushka_Base); // Засовываем настройи в регистры

	    /*ОПИСАНИЕ ПОЛЕЙ НАСТРОЙКИ КАНАЛОВ ТАЙМЕРОВ
	    Timerushka_OCI.TIM_OCMode; // рег TIMx_CCMR1 .биты OC2M[2:0] режим работы ножки настроенной на выход
	    Timerushka_OCI.TIM_OutputState; //рег TIMx_ССER .бит CC1E Подключаем нашу ножку к логике таймера
	    Timerushka_OCI.TIM_OutputNState; //  only for TIM1 and TIM8
	    Timerushka_OCI.TIM_Pulse; // регистр TIMx->CCR1 (число сравнения в канале) число между  0x0000 и 0xFFFF
	    Timerushka_OCI.TIM_OCPolarity; // рег TIMx->CCER бит CC1P Полярность выхода
	    Timerushka_OCI.TIM_OCNPolarity; // only for TIM1 and TIM8
	    Timerushka_OCI.TIM_OCIdleState; // only for TIM1 and TIM8
	    Timerushka_OCI.TIM_OCNIdleState; // only for TIM1 and TIM8  */

	    // НАСТРОЙКИ 1ого канала таймера
	    TIM_OCInitTypeDef Timerushka_OCI;// создаём переменную (структуру) для определения режима работы таймера
	    // 1)Timerushka_OCI.TIM_OCMode = TIM_OCMode_Toggle; // Режим инвертирование ножки при совпадении счетного и рег канала
	    // 2)
	    Timerushka_OCI.TIM_OCMode = TIM_OCMode_PWM1; // Режим установки ножки при совпадении счетного и рег канала
	    Timerushka_OCI.TIM_OutputState = TIM_OutputState_Enable; // Подключаем нашу ножку к логике таймера
	    Timerushka_OCI.TIM_Pulse = 1000; // регистр TIMx->CCR1 (число сравнения в канале) число между  0x0000 и 0xFFFF
	    //
	    Timerushka_OCI.TIM_Pulse = 9;
	    Timerushka_OCI.TIM_OCPolarity = TIM_OCPolarity_High; // рег TIMx->CCER бит CC1P Полярность выхода
	    TIM_OC1Init(TIM2, &Timerushka_OCI);

	    // ЗАПУСК всего безобразий !!!
	    TIM_Cmd(TIM2, ENABLE);
}



void setBasicTimer(void) {

}

