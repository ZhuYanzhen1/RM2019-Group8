#ifndef __ADC_H
#define __ADC_H
#include "System_Config.h"

void ADC1_Init(void); 				//ADCͨ����ʼ��
u16 Get_ADC(u32 ch);//�õ�ĳ��ͨ����������������ƽ��ֵ
short Get_Temprate(void);    //��ȡCPU�ں��¶�
#endif 
