#ifndef __RANDOM_H_
#define __RANDOM_H_

#include "System_Config.h"
u8  Random_Init(void);			//RNG��ʼ�� 
u32 RNG_Get_RandomNum(void);//�õ������
int Get_RandomRange(int min,int max);//����[min,max]��Χ�������
#endif

