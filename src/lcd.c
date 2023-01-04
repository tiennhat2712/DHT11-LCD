#include "lcd.h"
#include "delay.h"
#include <ctype.h>

static GPIO_InitTypeDef gpioInit = {0};

static void GPIO_Pin_To_Input(GPIO_TypeDef *gpioPort, uint32_t gpioPin);
static void GPIO_Pin_To_Output(GPIO_TypeDef *gpioPort, uint32_t gpioPin);
static void LCD_Write_Byte(uint8_t u8Data, uint8_t u8RS);
static uint8_t LCD_Read_Byte(uint8_t u8RS);
static void LCD_Write_Nibble(uint8_t u8Data, uint8_t u8RS);
	
static void GPIO_Pin_To_Input(GPIO_TypeDef *gpioPort, uint32_t gpioPin)
{
	
	gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioInit.GPIO_Pin = gpioPin;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpioPort, &gpioInit);
}

static void GPIO_Pin_To_Output(GPIO_TypeDef *gpioPort, uint32_t gpioPin)
{
	
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Pin = gpioPin;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(gpioPort, &gpioInit);
}

static void LCD_Write_Byte(uint8_t u8Data, uint8_t u8RS)
{
	
	GPIO_WriteBit(LCD_RW_PORT, LCD_RW, Bit_RESET);
	GPIO_WriteBit(LCD_RW_PORT, LCD_RS, u8RS ? Bit_SET : Bit_RESET);
	Delay_Us(1);
	
	GPIO_Pin_To_Output(LCD_D7_PORT, LCD_D7);
	GPIO_Pin_To_Output(LCD_D6_PORT, LCD_D6);
	GPIO_Pin_To_Output(LCD_D5_PORT, LCD_D5);
	GPIO_Pin_To_Output(LCD_D4_PORT, LCD_D4);
	
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_SET);
	
	GPIO_WriteBit(LCD_D7_PORT, LCD_D7, (u8Data & 0x80) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D6_PORT, LCD_D6, (u8Data & 0x40) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D5_PORT, LCD_D5, (u8Data & 0x20) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D4_PORT, LCD_D4, (u8Data & 0x10) ? Bit_SET : Bit_RESET);
	
	Delay_Us(1);
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	Delay_Us(1);
	
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_SET);
	
	GPIO_WriteBit(LCD_D7_PORT, LCD_D7, (u8Data & 0x08) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D6_PORT, LCD_D6, (u8Data & 0x04) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D5_PORT, LCD_D5, (u8Data & 0x02) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D4_PORT, LCD_D4, (u8Data & 0x01) ? Bit_SET : Bit_RESET);
	
	Delay_Us(1);
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	Delay_Us(1);
}

static void LCD_Write_Nibble(uint8_t u8Data, uint8_t u8RS)
{
	
	GPIO_WriteBit(LCD_RW_PORT, LCD_RW, Bit_RESET);
	GPIO_WriteBit(LCD_RW_PORT, LCD_RS, u8RS ? Bit_SET : Bit_RESET);
	Delay_Us(1);
	
	GPIO_Pin_To_Output(LCD_D7_PORT, LCD_D7);
	GPIO_Pin_To_Output(LCD_D6_PORT, LCD_D6);
	GPIO_Pin_To_Output(LCD_D5_PORT, LCD_D5);
	GPIO_Pin_To_Output(LCD_D4_PORT, LCD_D4);
	
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_SET);
	
	GPIO_WriteBit(LCD_D7_PORT, LCD_D7, (u8Data & 0x80) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D6_PORT, LCD_D6, (u8Data & 0x40) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D5_PORT, LCD_D5, (u8Data & 0x20) ? Bit_SET : Bit_RESET);
	GPIO_WriteBit(LCD_D4_PORT, LCD_D4, (u8Data & 0x10) ? Bit_SET : Bit_RESET);
	
	Delay_Us(1);
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	Delay_Us(1);
}

static uint8_t LCD_Read_Byte(uint8_t u8RS)
{
	uint8_t u8Data = 0x00;
	
	GPIO_WriteBit(LCD_RW_PORT, LCD_RW, Bit_SET);
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS, u8RS ? Bit_SET : Bit_RESET);
	Delay_Us(1);
	
	GPIO_Pin_To_Input(LCD_D7_PORT, LCD_D7);
	GPIO_Pin_To_Input(LCD_D6_PORT, LCD_D6);
	GPIO_Pin_To_Input(LCD_D5_PORT, LCD_D5);
	GPIO_Pin_To_Input(LCD_D4_PORT, LCD_D4);
	
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_SET);
	Delay_Us(1);
	
	if (GPIO_ReadInputDataBit(LCD_D7_PORT, LCD_D7)) {
		u8Data |= 0x80;
	}
	if (GPIO_ReadInputDataBit(LCD_D6_PORT, LCD_D6)) {
		u8Data |= 0x40;
	}
	if (GPIO_ReadInputDataBit(LCD_D5_PORT, LCD_D5)) {
		u8Data |= 0x20;
	}
	if (GPIO_ReadInputDataBit(LCD_D4_PORT, LCD_D4)) {
		u8Data |= 0x10;
	}

	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	Delay_Us(1);
	
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_SET);
	Delay_Us(1);
	
	if (GPIO_ReadInputDataBit(LCD_D7_PORT, LCD_D7)) {
		u8Data |= 0x08;
	}
	if (GPIO_ReadInputDataBit(LCD_D6_PORT, LCD_D6)) {
		u8Data |= 0x04;
	}
	if (GPIO_ReadInputDataBit(LCD_D5_PORT, LCD_D5)) {
		u8Data |= 0x02;
	}
	if (GPIO_ReadInputDataBit(LCD_D4_PORT, LCD_D4)) {
		u8Data |= 0x01;
	}

	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	Delay_Us(1);
	
	return u8Data;
}

static uint8_t LCD_Busy(void)
{
	uint8_t u8Tmp;
	
	u8Tmp = LCD_Read_Byte(0);
	
	return (u8Tmp & 0x80);
}

void LCD_Init(void)
{
	
	Delay_Ms(15);
	LCD_RCC_RS_EN;
	LCD_RCC_RW_EN;
	LCD_RCC_EN_EN;
	LCD_RCC_D4_EN;
	LCD_RCC_D5_EN;
	LCD_RCC_D6_EN;
	LCD_RCC_D7_EN;
	
	GPIO_WriteBit(LCD_RS_PORT, LCD_RS, Bit_RESET);
	GPIO_WriteBit(LCD_RW_PORT, LCD_RW, Bit_RESET);
	GPIO_WriteBit(LCD_EN_PORT, LCD_EN, Bit_RESET);
	GPIO_WriteBit(LCD_D4_PORT, LCD_D4, Bit_RESET);
	GPIO_WriteBit(LCD_D5_PORT, LCD_D5, Bit_RESET);
	GPIO_WriteBit(LCD_D6_PORT, LCD_D6, Bit_RESET);
	GPIO_WriteBit(LCD_D7_PORT, LCD_D7, Bit_RESET);
	
	GPIO_Pin_To_Output(LCD_RS_PORT, LCD_RS);
	GPIO_Pin_To_Output(LCD_RW_PORT, LCD_RW);
	GPIO_Pin_To_Output(LCD_EN_PORT, LCD_EN);
	GPIO_Pin_To_Output(LCD_D4_PORT, LCD_D4);
	GPIO_Pin_To_Output(LCD_D5_PORT, LCD_D5);
	GPIO_Pin_To_Output(LCD_D6_PORT, LCD_D6);
	GPIO_Pin_To_Output(LCD_D7_PORT, LCD_D7);
	
	LCD_Write_Nibble(0x30, 0);
	Delay_Ms(5);
	LCD_Write_Nibble(0x30, 0);
	Delay_Us(100);
	LCD_Write_Nibble(0x30, 0);
	LCD_Write_Nibble(0x20, 0);
	
	/* cau hinh 8 bit 2 dong font 5x8 */
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x28, 0);
	
	/* display off */
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x08, 0);
	
	/* display clear */
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x01, 0);
	
	/* entry mode set */
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x06, 0);
	
	/* display on, cursor on */
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x0E, 0);
}

void LCD_PutC(uint8_t u8Data)
{
	
	if (u8Data == '\f') {
		while (LCD_Busy()) {
		}
		LCD_Write_Byte(0, 1);
	} else if (u8Data == '\n') {
		LCD_GotoXY(0, 1);
	} else {
		if(isprint(u8Data)){
			while (LCD_Busy()) {
			}
			LCD_Write_Byte(u8Data, 1);
		}
	}
}

void LCD_GotoXY(uint8_t x, uint8_t y)
{
	uint8_t u8Tmp;
	
	if (y) {
		u8Tmp = 0x40;
	} else {
		u8Tmp = 0;
	}
	
	u8Tmp += x;
	
	while (LCD_Busy()) {
	}
	LCD_Write_Byte(0x80 | u8Tmp, 0);
}

void LCD_Puts(char *s)
{
	
	while (*s) {
		LCD_PutC(*s++);
	}
}
