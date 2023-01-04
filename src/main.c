#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "dht11.h"
#include <stdio.h>

int main(void)
{
	GPIO_InitTypeDef gpioInit;
	char szDisplay[17];
	uint8_t u8Buff[4];
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = GPIO_Pin_13;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &gpioInit);
	
	Delay_Init();
	LCD_Init();
	dht11_init();
	
	LCD_GotoXY(0, 0);
	LCD_Puts("STM32 LCD1602");
	LCD_GotoXY(0, 1);
	LCD_Puts("STM32 DHT11");
	
	while (1) {
		
		/* da doc duoc nhiet do va do am */
		/* gui do am */
		if (dht11_read(u8Buff)) {
			//send(u8Buff[0]);
			sprintf(szDisplay, "Do am: %d       ", u8Buff[0]);
			LCD_GotoXY(0, 0);
			LCD_Puts(szDisplay);
			//Delay_Ms(20);
			
			/* gui nhiet do */
			//send(u8Buff[2]);
			sprintf(szDisplay, "Nhiet do: %d    ", u8Buff[2]);
			LCD_GotoXY(0, 1);
			LCD_Puts(szDisplay);
		} else {
			LCD_GotoXY(0, 0);
			LCD_Puts("STM32 - LCD1602 ");
			LCD_GotoXY(0, 1);
			LCD_Puts("DHT11: ERROR    ");
		}
		
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		Delay_Ms(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		Delay_Ms(500);
	}
	
}
