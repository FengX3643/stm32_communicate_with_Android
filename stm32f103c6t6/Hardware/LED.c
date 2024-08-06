#include "stm32f10x.h"                  // Device header

int8_t d1=0;
int8_t d2=0;
void LED_Init(void)
{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED1_ON(void)
{
	d1=1;
}

void LED1_OFF(void)
{
	d1=0;
}
u8 LED1_State(void)
{
	return d1;
}


void LED1_UP(void)
{
	;
}
void LED1_DN(void)
{
	;
}
void LED2_ON(void)
{
	d2=1;
}

void LED2_OFF(void)
{
	d2=0;
}
u8 LED2_State(void)
{
	return d2;
}


