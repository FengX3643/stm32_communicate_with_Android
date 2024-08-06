#ifndef __MY_FUNCTION_H
#define __MY_FUNCTION_H
#include "stm32f10x.h" 
#include <time.h>
#define uchar signed short
#define uint unsigned int
	
uint16_t string2int(char str[],int8_t a,int8_t b);
int8_t Please_Set_Time(uchar Set_MyRTC_Time[]);
int8_t Set_Clock(uchar Clock_Time[]);
time_t Time2cnt(uchar Set_MyRTC_Time[]);

#endif
