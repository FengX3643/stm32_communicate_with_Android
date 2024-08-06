#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"                  // Device header

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
void delay_10us(uint32_t x10s);
#endif