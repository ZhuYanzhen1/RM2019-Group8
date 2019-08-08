#include "adc.h"
#include "User.h"

#if(USING_ADC == 1)

ADC_HandleTypeDef ADC1_Handler;//ADC���
void ADC1_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOAʱ��
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStructure.Mode=GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pin=GPIO_PIN_0|GPIO_PIN_1;
		GPIO_InitStructure.Pull=GPIO_NOPULL;
		GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
		GPIO_InitStructure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
		HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
		GPIO_InitStructure.Pin=GPIO_PIN_6;
		HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         //1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     //����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
}
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_3CYCLES;        //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
	  return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u16 Get_ADC(u32 ch)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<50;t++)
	{
		temp_val+=Get_Adc(ch);
		//Delayus(100);
	}
	return temp_val/50;
} 
//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
static u32 adcx;
static short result=0;
static double temperate=0;
short Get_Temprate(void)
{
	adcx=Get_ADC(ADC_CHANNEL_TEMPSENSOR);//��ȡ�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
	temperate=(float)adcx*(3.3/4096);		//��ѹֵ
	temperate=(temperate-0.76)/0.0025 + 25; //ת��Ϊ�¶�ֵ 
	result=temperate*=100;					//����100��.
	return result;
}

#endif	//#if(USING_ADC == 1)
