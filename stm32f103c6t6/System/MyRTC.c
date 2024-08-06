#include "stm32f10x.h"                  // Device header
#include <time.h>


void MyRTC_SetTime(uint16_t Set_MyRTC_Time[]);				//函数声明
uint16_t Read_MyRTC_Time[6];
/**
  * 函    数：RTC初始化
  * 参    数：无
  * 返 回 值：无
  */

//如果LSE无法起振导致程序卡死在初始化函数中
//可将初始化函数替换为下述代码，使用LSI当作RTCCLK
//LSI无法由备用电源供电，故主电源掉电时，RTC走时会暂停

void MyRTC_Init(uint16_t Set_MyRTC_Time[])
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	PWR_BackupAccessCmd(ENABLE);

	RCC_LSICmd(ENABLE);
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
	
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	
	RTC_WaitForSynchro();
	RTC_WaitForLastTask();
	
	RTC_SetPrescaler(40000 - 1);
	RTC_WaitForLastTask();
	
	MyRTC_SetTime(Set_MyRTC_Time);
	
	BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}

/**
  * 函    数：RTC设置时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，全局数组里时间值将刷新到RTC硬件电路
  */
void MyRTC_SetTime(uint16_t Set_MyRTC_Time[])
{
	time_t time_cnt;		//定义秒计数器数据类型
	struct tm time_date;	//定义日期时间数据类型
	
	time_date.tm_year = Set_MyRTC_Time[0] - 1900;		//将数组的时间赋值给日期时间结构体
	time_date.tm_mon = Set_MyRTC_Time[1] - 1;
	time_date.tm_mday = Set_MyRTC_Time[2];
	time_date.tm_hour = Set_MyRTC_Time[3];
	time_date.tm_min = Set_MyRTC_Time[4];
	time_date.tm_sec = Set_MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8 * 60 * 60;	//调用mktime函数，将日期时间转换为秒计数器格式
													//- 8 * 60 * 60为东八区的时区调整
	
	RTC_SetCounter(time_cnt);						//将秒计数器写入到RTC的CNT中
	RTC_WaitForLastTask();							//等待上一次操作完成
}

/**
  * 函    数：RTC读取时间
  * 参    数：无
  * 返 回 值：无
  * 说    明：调用此函数后，RTC硬件电路里时间值将刷新到全局数组
  */
time_t MyRTC_ReadTime(void)
{
	time_t time_cnt;		//定义秒计数器数据类型
	struct tm time_date;	//定义日期时间数据类型
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;		//读取RTC的CNT，获取当前的秒计数器
													//+ 8 * 60 * 60为东八区的时区调整
	
	time_date = *localtime(&time_cnt);				//使用localtime函数，将秒计数器转换为日期时间格式
	
	Read_MyRTC_Time[0] = time_date.tm_year + 1900;		//将日期时间结构体赋值给数组的时间
	Read_MyRTC_Time[1] = time_date.tm_mon + 1;
	Read_MyRTC_Time[2] = time_date.tm_mday;
	Read_MyRTC_Time[3] = time_date.tm_hour;
	Read_MyRTC_Time[4] = time_date.tm_min;
	Read_MyRTC_Time[5] = time_date.tm_sec;
	return time_cnt;
}
