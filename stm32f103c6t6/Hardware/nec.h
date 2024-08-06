#ifndef __NEC_H
#define __NEC_H

#define IRED	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
#include "Delay.h"
void NECInit(void);
u8 HW_jssj(void);
extern u8 hw_jsbz;
extern u8 hw_jsm;
#endif
