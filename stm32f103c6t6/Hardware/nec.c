#include "stm32f10x.h" 
#include "nec.h"
#include "Delay.h"
void NECInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIO Ê±ÖÓ£¬ÏÂÃæÒÔ PA10 ÎªÀý
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO Ê±ÖÓ
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// ÒÔÉÏÀ­ÊäÈëÄ£Ê½ÎªÀý
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);	// °Ñ 10 ºÅÒý½ÅµÄÖÐ¶ÏÅä¸ø GPIOA£¬¼´ PA10
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;		// Ñ¡ÔñÖÐ¶ÏÏß
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		// ±íÊ¾¿ª·Å¸ÃÌõÖÐ¶ÏÏß
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// ÖÐ¶ÏÄ£Ê½£¬ÊÂ¼þÄ£Ê½Îª EXTI_Mode_EVENT£¬ßí²»¹ýÕâÊÇ¸öÃ¶¾ÙÀàÐÍ£¬ÄÑµÀ²»ÄÜÒ»Æð¿ªÂð
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// ÒÔÉÏÉýÑØ´¥·¢ÎªÀý
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// ·Ö×é·½Ê½ 2 ¼´ÇÀÕ¼ºÍÏìÓ¦ÓÅÏÈ¼¶¸÷Õ¼ 2 Î» (0 ~ 3)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		// PA10 ¸ÕºÃ¿ÉÒÔ¾ÙÀýÒ»ÏÂÕâ¸ö EXTI15_10£¬ÈçÇ°ËùÊö£¬ÊÇ¹éµ½Ò»ÆðÈ¥ÁË
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// ÉèÖÃ¿ª·Å¸ÃÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ÇÀÕ¼ÓÅÏÈ¼¶£¬ÊýÖµ·¶Î§Òª×¢Òâ·ûºÏÇ°Ãæ·Ö×éµÄ¹æ·¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		// ÏìÓ¦ÓÅÏÈ¼¶£¬ÊýÖµ·¶Î§Òª×¢Òâ·ûºÏÇ°Ãæ·Ö×éµÄ¹æ·¶
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	
}
u8 HW_jssj(void)
	{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)//é«˜ç”µå¹³
	{
	t++;
	Delay_us(20);
	if(t>=250) return t;//è¶…æ—¶æº¢å‡º
	}
	return t;
	}
u8 hw_jsbz =0;
u8 hw_jsm = 0;	
void EXTI15_10_IRQHandler(void) {
	u8 Tim=0,Ok=0,Data,Num=0;
	
	while(1)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)
		{
			Tim=HW_jssj();//èŽ·å¾—æ­¤æ¬¡é«˜ç”µå¹³æ—¶é—´
			if(Tim>=250) break;//ä¸æ˜¯æœ‰ç”¨çš„ä¿¡å·
			else  if(Tim>=200 && Tim<250)
			{
				Ok=1;//æ”¶åˆ°èµ·å§‹ä¿¡å·
			}
			if(Tim>=60 && Tim<90)
			{
				Data=1;//æ”¶åˆ°æ•°æ® 1
			}
			else if(Tim>=10 && Tim<50)
			{
				Data=0;//æ”¶åˆ°æ•°æ® 0
			}
			if(Ok==1)
			{
				hw_jsm<<=1;
				hw_jsm+=Data;
				if(Num>=32)
				{
					hw_jsbz=1;
					break;
				}
			}
			Num++;
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line10);
}



