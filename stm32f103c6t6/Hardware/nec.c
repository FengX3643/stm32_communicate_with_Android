#include "stm32f10x.h" 
#include "nec.h"
#include "Delay.h"
void NECInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIO 时钟，下面以 PA10 为例
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO 时钟
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// 以上拉输入模式为例
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);	// 把 10 号引脚的中断配给 GPIOA，即 PA10
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;		// 选择中断线
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		// 表示开放该条中断线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// 中断模式，事件模式为 EXTI_Mode_EVENT，唔不过这是个枚举类型，难道不能一起开吗
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// 以上升沿触发为例
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// 分组方式 2 即抢占和响应优先级各占 2 位 (0 ~ 3)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		// PA10 刚好可以举例一下这个 EXTI15_10，如前所述，是归到一起去了
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// 设置开放该中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// 抢占优先级，数值范围要注意符合前面分组的规范
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		// 响应优先级，数值范围要注意符合前面分组的规范
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	
}
u8 HW_jssj(void)
	{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)//楂樼數骞�
	{
	t++;
	Delay_us(20);
	if(t>=250) return t;//瓒呮椂婧㈠嚭
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
			Tim=HW_jssj();//鑾峰緱姝ゆ楂樼數骞虫椂闂�
			if(Tim>=250) break;//涓嶆槸鏈夌敤鐨勪俊鍙�
			else  if(Tim>=200 && Tim<250)
			{
				Ok=1;//鏀跺埌璧峰淇″彿
			}
			if(Tim>=60 && Tim<90)
			{
				Data=1;//鏀跺埌鏁版嵁 1
			}
			else if(Tim>=10 && Tim<50)
			{
				Data=0;//鏀跺埌鏁版嵁 0
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



