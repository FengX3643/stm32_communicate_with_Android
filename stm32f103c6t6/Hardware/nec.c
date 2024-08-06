#include "stm32f10x.h" 
#include "nec.h"
#include "Delay.h"
void NECInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIO ʱ�ӣ������� PA10 Ϊ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// AFIO ʱ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		// ����������ģʽΪ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource10);	// �� 10 �����ŵ��ж���� GPIOA���� PA10
	
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;		// ѡ���ж���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;		// ��ʾ���Ÿ����ж���
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	// �ж�ģʽ���¼�ģʽΪ EXTI_Mode_EVENT���������Ǹ�ö�����ͣ��ѵ�����һ����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	// �������ش���Ϊ��
	EXTI_Init(&EXTI_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	// ���鷽ʽ 2 ����ռ����Ӧ���ȼ���ռ 2 λ (0 ~ 3)
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		// PA10 �պÿ��Ծ���һ����� EXTI15_10����ǰ�������ǹ鵽һ��ȥ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			// ���ÿ��Ÿ��ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	// ��ռ���ȼ�����ֵ��ΧҪע�����ǰ�����Ĺ淶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		// ��Ӧ���ȼ�����ֵ��ΧҪע�����ǰ�����Ĺ淶
	NVIC_Init(&NVIC_InitStructure);
	
	
	
	
}
u8 HW_jssj(void)
	{
	u8 t=0;
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)==1)//高电平
	{
	t++;
	Delay_us(20);
	if(t>=250) return t;//超时溢出
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
			Tim=HW_jssj();//获得此次高电平时间
			if(Tim>=250) break;//不是有用的信号
			else  if(Tim>=200 && Tim<250)
			{
				Ok=1;//收到起始信号
			}
			if(Tim>=60 && Tim<90)
			{
				Data=1;//收到数据 1
			}
			else if(Tim>=10 && Tim<50)
			{
				Data=0;//收到数据 0
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



