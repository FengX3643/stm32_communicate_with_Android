#ifndef __MYRTC_H
#define __MYRTC_H
#include "stm32f10x.h"                  // Device header
#include <time.h>

extern uint16_t Read_MyRTC_Time[];

void MyRTC_Init(uint16_t Set_MyRTC_Time[]);
void MyRTC_SetTime(uint16_t Set_MyRTC_Time[]);
time_t MyRTC_ReadTime(void);

#endif
