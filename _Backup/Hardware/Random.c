#include "Random.h"
#include "User.h"

#if(USING_RANDOM == 1)

RNG_HandleTypeDef RNG_Handler;  //RNG���
u8 Random_Init(void)
{
    u16 retry=0;
	  u16 Counter=0;
	  __HAL_RCC_RNG_CLK_ENABLE();//ʹ��RNGʱ��
    RNG_Handler.Instance=RNG;
    HAL_RNG_Init(&RNG_Handler);//��ʼ��RNG
    while(__HAL_RNG_GET_FLAG(&RNG_Handler,RNG_FLAG_DRDY)==RESET&&retry<10000)//�ȴ�RNG׼������
    {
        retry++;
				Counter=1800;
        while(Counter--);
    }
    if(retry>=10000) return 1;//���������������������
    return 0;
}
u32 RNG_Get_RandomNum(void)
{
    return HAL_RNG_GetRandomNumber(&RNG_Handler);
}
//����[min,max]��Χ�������
int Get_RandomRange(int min,int max)
{ 
   return HAL_RNG_GetRandomNumber(&RNG_Handler)%(max-min+1) +min;
}

#endif	//#if(USING_RANDOM == 1)
