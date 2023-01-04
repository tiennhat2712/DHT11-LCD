#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x.h"

void Delay_Init(void);
void Delay_Ms(uint32_t u32Delay);
void Delay_Us(uint32_t u32Delay);

#endif
