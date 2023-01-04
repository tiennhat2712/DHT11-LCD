#include "delay.h"

void Delay_Init(void)
{
	/* su dung timer nao do de tao delay */
	TIM_TimeBaseInitTypeDef timInit;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timInit.TIM_ClockDivision = TIM_CKD_DIV1;
	timInit.TIM_CounterMode = TIM_CounterMode_Up;
	timInit.TIM_Period = 0xffff;
	timInit.TIM_Prescaler = 72 - 1;
	timInit.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &timInit);
	TIM_Cmd(TIM2, ENABLE);
}

void Delay_Ms(uint32_t u32Delay)
{
	uint32_t i;
	
	for (i = 0; i< u32Delay; ++i) {
		Delay_Us(1000);
	}
}

void Delay_Us(uint32_t u32Delay)
{
	TIM_SetCounter(TIM2, 0);
	while (TIM_GetCounter(TIM2) < u32Delay);
}
