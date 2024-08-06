#include "stm32f10x.h"                  // Device header
#include <string.h>
#include <stdlib.h>
#include "Serial.h"
#include "Delay.h"
#include <time.h>
#include "My_function.h"

uint16_t all;
uint16_t tt;
uint16_t string2int(char str[],int8_t a,int8_t b) //12位
{
	int8_t i;
	int8_t j=0;
	char temp[b-a+2];
	for(i=a;i<b;i++)
	{
		temp[j++] = str[i];
	}
	temp[j] = '\0';
	all=(uint16_t) strtol(temp,NULL,10);
	return all;
}

int8_t Set_Clock(uchar Clock_Time[]) {
    if ((string2int(Serial_RxPacket, 1, 3) < 24) && (string2int(Serial_RxPacket, 1, 3) >= 0)) {
        // 时
        Clock_Time[0] = string2int(Serial_RxPacket,1, 3);
    } else {
        return 0;
    }

    if ((string2int(Serial_RxPacket, 3, 5) < 60) && (string2int(Serial_RxPacket, 3, 5) >= 0)) {
        // 分
        Clock_Time[1] = string2int(Serial_RxPacket, 3, 5);
        return 1;
    } else {
        return 0;
    }
}

int8_t Please_Set_Time(uchar Time_Data[])
{
	
//		Serial_SendString("输入当前时间,格式为：@xxxx年xx月xx日xx时xx分xx秒#");
		
		
			memmove(Serial_RxPacket, Serial_RxPacket + 1, strlen(Serial_RxPacket)); // 移动字符串中的字符，去除第一个字符
			if(strlen(Serial_RxPacket) == 14)
			{
				tt = string2int(Serial_RxPacket,0,4);
				if( (string2int(Serial_RxPacket,0,4)<3000) && 
						(string2int(Serial_RxPacket,0,4)>2022))
				{
					//年
					Time_Data[0] = string2int(Serial_RxPacket,2,4);
				}else{
					
					return 0;
				}
				if( (string2int(Serial_RxPacket,4,6)<13) && 
						(string2int(Serial_RxPacket,4,6)>0))
				{
					//月
					Time_Data[1] = string2int(Serial_RxPacket,4,6);
				}else{
					
					return 0;
				}
				if( (string2int(Serial_RxPacket,6,8)<32) && 
						(string2int(Serial_RxPacket,6,8)>0))
				{
					//日
					Time_Data[2] = string2int(Serial_RxPacket,6,8);
				}else{
					
					return 0;
				}
				if( (string2int(Serial_RxPacket,8,10)<24) && 
						(string2int(Serial_RxPacket,8,10)>=0))
				{
					//时
					Time_Data[3] = string2int(Serial_RxPacket,8,10);
				}else{
					
					return 0;
				}
				if( (string2int(Serial_RxPacket,10,12)<60) && 
						(string2int(Serial_RxPacket,10,12)>=0))
				{
					//分
					Time_Data[4] = string2int(Serial_RxPacket,10,12);
					//秒
					Time_Data[5]=string2int(Serial_RxPacket,12,14);
					Serial_RxFlag = 0;
					return 1;
				}else{
					Serial_RxFlag = 0;
					return 0;
				}
			}else
			return 0;
}
time_t Time2cnt(uchar Time_Data[])
{
	time_t time_cnt;		//定义秒计数器数据类型
	struct tm time_date;	//定义日期时间数据类型
	
	time_date.tm_year = Time_Data[0] - 1900;		//将数组的时间赋值给日期时间结构体
	time_date.tm_mon = Time_Data[1] - 1;
	time_date.tm_mday = Time_Data[2];
	time_date.tm_hour = Time_Data[3];
	time_date.tm_min = Time_Data[4];
	time_date.tm_sec = Time_Data[5];
	time_cnt = mktime(&time_date);	//调用mktime函数，将日期时间转换为秒计数器格式
	return time_cnt;
}
