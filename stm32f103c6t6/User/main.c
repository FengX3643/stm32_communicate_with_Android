#include "stm32f10x.h"
#include <stdio.h>
#include "Delay.h"
#include "LED.h"
#include "Serial.h"
#include <stdlib.h>
#include <string.h>
#include "My_function.h"
#include "PWM.h"
#include "Timer.h"
#include "DHT11.h"
#include "DS1302.h"

int j = 0;
int u = 0;
int8_t i = 0;
time_t temp_time_cnt = 0;
time_t clock_cnt = 0;
int16_t intact_Clock_Time[6]; //设定的闹钟时间
int8_t Clock_flag = 0; 
int8_t Clock_state = 0;
int8_t Time_flag = 0;
uchar Clock_Time[2] = {0};
int8_t gui_LED_v_cnt = 60;
int8_t chuang_LED_v_cnt = 16;
int8_t clock_LED_v_cnt = 16;
int clock_keep_time = 10 * 60;
char Will_Send_String[100];
int16_t init_time[6]; // 时间参数
int main(void)
{
	DHT11_Data_TypeDef DHT11_Data;
	Serial_Init();
	Timer_Init(); // 定时中断初始化
	PWM_Init();
	DHT11_GPIO_Config();
	Serial_SendString("ok");
	while (1)
	{
		if (Serial_RxFlag == 1)
		{
			switch (Serial_RxPacket[0])
			{
			case ('c'):
				if (Read_DHT11(&DHT11_Data) != SUCCESS)
				{
					DHT11_Data.temp_int = 0;
					DHT11_Data.temp_deci = 0;
					DHT11_Data.humi_int = 0;
					DHT11_Data.humi_deci = 0;
				}
				if (Time_flag == 0)
				{
					sprintf(Will_Send_String, "0%02d%02d%02d%02d%d%02d%02d",
							DHT11_Data.temp_int, DHT11_Data.temp_deci,
							DHT11_Data.humi_int, DHT11_Data.humi_deci,
							Clock_flag,
							Clock_Time[0], Clock_Time[1]);
					Serial_SendString(Will_Send_String);
				}
				else
				{
					sprintf(Will_Send_String, "1%02d%02d%02d%02d%d%02d%02d",
							DHT11_Data.temp_int, DHT11_Data.temp_deci,
							DHT11_Data.humi_int, DHT11_Data.humi_deci,
							Clock_flag,
							Clock_Time[0], Clock_Time[1]);
					Serial_SendString(Will_Send_String);
				}
				Serial_RxFlag = 0;
				break;
			case ('d'):
				if (Please_Set_Time(init_time))
				{
					DS1302_init(init_time);
					Serial_SendString("time correction Successfully");
					Time_flag = 1;
				}
				else
					Serial_SendString("Time correction failed");
				Serial_RxFlag = 0;
				break;
			case ('n'):
				if (Set_Clock(Clock_Time) && Time_flag)
				{

					Serial_SendString("Alarm setting successfully");
				}
				else
				{
					Serial_SendString("Alarm setting failed");
				}
				Serial_RxFlag = 0;
				break;
			case ('1'):

				if (strcmp(Serial_RxPacket, "1_on") == 0)
				{
					PWM_SetCompare1(chuang_LED_v_cnt);
					Serial_SendString("open LED");
				}
				else
				{
					PWM_SetCompare1(0);
					Serial_SendString("close LED");
				}
				Serial_RxFlag = 0;
				break;
			case ('2'):
				if (strcmp(Serial_RxPacket, "2_on") == 0)
				{
					PWM_SetCompare2(gui_LED_v_cnt);
					Serial_SendString("open LED2");
				}
				else
				{
					PWM_SetCompare2(0);
					Serial_SendString("close LED2");
				}
				Serial_RxFlag = 0;
				break;
			case ('N'):
				if (strcmp(Serial_RxPacket, "N_on") == 0)
				{
					Clock_flag = 1;
				}
				else
				{
					Clock_flag = 0;
				}
				Serial_RxFlag = 0;
				break;
			case ('S'):
				if (strlen(Serial_RxPacket) == 9)
				{
					clock_keep_time = string2int(Serial_RxPacket, 1, 3) * 60;
					clock_LED_v_cnt = string2int(Serial_RxPacket, 3, 5);
					chuang_LED_v_cnt = string2int(Serial_RxPacket, 5, 7);
					gui_LED_v_cnt = string2int(Serial_RxPacket, 7, 9);
					Serial_RxFlag = 0;
				}
			case ('U'):
				sprintf(Will_Send_String, "T%04d%02d%02d%02d%02d%02dppp%02d",
						time_data[0], time_data[1], time_data[2],
						time_data[3], time_data[4], time_data[5], u);
				Serial_SendString(Will_Send_String);
			default:
				Serial_RxFlag = 0;
			}
		}
	}
}
void TIM3_IRQHandler(void) //每一秒定时中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) // 判断是否是TIM3的更新事件触发的中断
	{
		DS1302_Readtime(); //读取当前时间

		if (Clock_flag && Time_flag)
		{
			temp_time_cnt = Time2cnt(time_data);
			intact_Clock_Time[0] = time_data[0];
			intact_Clock_Time[1] = time_data[1];
			intact_Clock_Time[2] = time_data[2];
			intact_Clock_Time[3] = Clock_Time[0];
			intact_Clock_Time[4] = Clock_Time[1];
			intact_Clock_Time[5] = 0;
			clock_cnt = Time2cnt(intact_Clock_Time); //将闹钟时间转换为时间戳
			u = temp_time_cnt - clock_cnt;

			if ((u > 0) && (u < clock_keep_time))
			{
				Serial_SendString("PWM");
				Serial_SendNumber(j, 3);
				if (j < clock_LED_v_cnt)
				{
					PWM_SetCompare1(j++);
				}
			}
			else
			{
				j = 0;
				if ((u >= clock_keep_time) && (u <= (clock_keep_time + 4)))
					PWM_SetCompare1(0);
			}
			Clock_state = 1;
		}
		else
		{
			if (Clock_state)
			{
				j = 0;
				Clock_state = 0;
				PWM_SetCompare1(0);
			}
		}
		// 中断标志位必须清除
		// 否则中断将连续不断地触发，导致主程序卡死
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
