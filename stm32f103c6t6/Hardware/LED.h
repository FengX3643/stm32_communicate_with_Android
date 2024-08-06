#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"                  // Device header

void LED_Init(void);
void LED1_ON(void);
void LED1_OFF(void);

void LED2_ON(void);
void LED2_OFF(void);

u8 LED1_State(void);
u8 LED2_State(void);

void LED1_UP(void);
void LED1_DN(void);

#endif
