#include "User.h"

#if(USING_IIC_2 == 1)

void IIC2_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
    IIC2_SDA=1;
    IIC2_SCL=1;  
}
void SDA2_OUT()
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_0;
  GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
  GPIO_Initure.Pull=GPIO_PULLUP;          //����
  GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}
void SDA2_IN()
{
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_0;
  GPIO_Initure.Mode=GPIO_MODE_INPUT;
	GPIO_Initure.Pull=GPIO_PULLUP;          //����
  HAL_GPIO_Init(GPIOF,&GPIO_Initure);
}
//����IIC��ʼ�ź�
void IIC2_Start(void)
{
	SDA2_OUT();     //sda�����
	IIC2_SDA=1;	  	  
	IIC2_SCL=1;
	Delayus(4);
 	IIC2_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delayus(4);
	IIC2_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC2_Stop(void)
{
	SDA2_OUT();//sda�����
	IIC2_SCL=0;
	IIC2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	Delayus(4);
	IIC2_SCL=1; 
	Delayus(4);			
	IIC2_SDA=1;//����I2C���߽����ź�				   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC2_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA2_IN();      //SDA����Ϊ����  
	IIC2_SDA=1;Delayus(1);	   
	IIC2_SCL=1;Delayus(1);	 
	while(READ_SDA2)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC2_Stop();
			return 1;
		}
	}
	IIC2_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC2_Ack(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=0;
	Delayus(2);
	IIC2_SCL=1;
	Delayus(2);
	IIC2_SCL=0;
}
//������ACKӦ��		    
void IIC2_NAck(void)
{
	IIC2_SCL=0;
	SDA2_OUT();
	IIC2_SDA=1;
	Delayus(2);
	IIC2_SCL=1;
	Delayus(2);
	IIC2_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC2_Send_Byte(u8 txd)
{              
    u8 t;   
		SDA2_OUT(); 	    
    IIC2_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC2_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
				Delayus(2);   //��TEA5767��������ʱ���Ǳ����
				IIC2_SCL=1;
				Delayus(2); 
				IIC2_SCL=0;	
				Delayus(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC2_Read_Byte(unsigned char ack)
{
		unsigned char i,receive=0;
		SDA2_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
		{
        IIC2_SCL=0; 
        Delayus(2);
				IIC2_SCL=1;
        receive<<=1;
        if(READ_SDA2)
					receive++;   
				Delayus(1); 
    }					 
    if (!ack)
        IIC2_NAck();//����nACK
    else
        IIC2_Ack(); //����ACK   
    return receive;
}

#endif	//#if(USING_IIC_2 == 1)
