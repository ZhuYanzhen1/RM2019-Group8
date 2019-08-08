#include "dac.h"
#include "User.h"

#if(USING_DAC == 1)

DAC_HandleTypeDef DAC1_Handler;//DAC���
DAC_HandleTypeDef DAC2_Handler;//DAC���
void DAC1_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
    GPIO_Initure.Pin=GPIO_PIN_4;            //PA4
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    DAC_ChannelConfTypeDef DACCH1_Config;
    DAC1_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC1_Handler);                 //��ʼ��DAC
    DACCH1_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //��ʹ�ô�������
    DACCH1_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_1);//DACͨ��1����
    HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_1);  //����DACͨ��1
}
void DAC2_Init(void)
{
		GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_DAC_CLK_ENABLE();             //ʹ��DACʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
    GPIO_Initure.Pin=GPIO_PIN_5;            //PA5
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    DAC_ChannelConfTypeDef DACCH2_Config;
    DAC2_Handler.Instance=DAC;
    HAL_DAC_Init(&DAC2_Handler);                 //��ʼ��DAC
    DACCH2_Config.DAC_Trigger=DAC_TRIGGER_NONE;             //��ʹ�ô�������
    DACCH2_Config.DAC_OutputBuffer=DAC_OUTPUTBUFFER_DISABLE;//DAC1�������ر�
    HAL_DAC_ConfigChannel(&DAC2_Handler,&DACCH2_Config,DAC_CHANNEL_2);//DACͨ��1����
    HAL_DAC_Start(&DAC2_Handler,DAC_CHANNEL_2);  //����DACͨ��2
}
//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void DAC1_Set(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
    HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_1,DAC_ALIGN_12B_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}
void DAC2_Set(u16 vol)
{
	double temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
  HAL_DAC_SetValue(&DAC2_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}

#endif	//#if(USING_DAC == 1)
