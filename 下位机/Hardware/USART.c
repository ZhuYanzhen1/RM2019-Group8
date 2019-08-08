#include "User.h"

#if(USING_USART == 1)

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
USART_Receve USART2_Receve_Handler;
USART_Receve USART3_Receve_Handler;
UART_HandleTypeDef USART3_Handler; //UART3���
UART_HandleTypeDef USART2_Handler; //UART2���
u8 aRxBuffer3;
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0);//ѭ������,ֱ���������   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif 
u8 aRxBuffer;
extern u8 aRxBuffer3;
void USART2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;		//����
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PD5,PD6
	USART2_Handler.Instance=USART2;					    //USART2
	USART2_Handler.Init.BaudRate=bound;				    //������
	USART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	USART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	USART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	USART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART2_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART2_Handler);
	HAL_NVIC_EnableIRQ(USART2_IRQn);		//ʹ��USART2�ж�ͨ��
	HAL_NVIC_SetPriority(USART2_IRQn,USART2_Priority,0);	//��ռ���ȼ�0�������ȼ�0
	HAL_UART_Receive_IT(&USART2_Handler,&aRxBuffer, 1);
}
unsigned char Rcv_Flg=0; //��Ϊ1���Ѿ��յ���֡ͷ
unsigned char USART3_Rcv_Buffer[15];
unsigned char RX_Counter=0x01;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)//����Ǵ���2
	{
		
	}
	if(huart->Instance==USART3)//����Ǵ���3
	{
		if(aRxBuffer3 == 0xff && Rcv_Flg == 1)
		{
			Rcv_Flg = 0;//�����շ�
			RX_Counter=1;
			USART3_Rcv_Buffer[11]=0xff;
			Received_CallBack((unsigned char *)&USART3_Rcv_Buffer);
		}
		else if(aRxBuffer3 == 0xff && Rcv_Flg == 0)
		{
			Rcv_Flg = 1;
			RX_Counter=1;
			memset(&USART3_Rcv_Buffer,0x00,sizeof(USART3_Rcv_Buffer));
			USART3_Rcv_Buffer[0]=0xff;
		}
		else if(Rcv_Flg == 1)
		{
			USART3_Rcv_Buffer[RX_Counter] = aRxBuffer3;
			RX_Counter++;
		}
	}
}
void USART2_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	HAL_UART_IRQHandler(&USART2_Handler);	//����HAL���жϴ����ú���
	timeout=0;
    while (HAL_UART_GetState(&USART2_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
	timeout=0;
	while(HAL_UART_Receive_IT(&USART2_Handler, &aRxBuffer, 1) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}
} 
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  __HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  hdma_usart1_rx.Instance = DMA2_Stream2;
  hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
  hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
  hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
  hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  HAL_DMA_Init(&hdma_usart1_rx);
  __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);
  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 100000;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_EVEN;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance==USART1)
  {
    __HAL_RCC_USART1_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7|GPIO_PIN_6);
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
}
void USART3_Init(unsigned long baud)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOD_CLK_ENABLE();			//ʹ��GPIODʱ��
	__HAL_RCC_USART3_CLK_ENABLE();			//ʹ��USART3ʱ��
	GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;	 		//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;		//����
	GPIO_Initure.Alternate=GPIO_AF7_USART3;	//����ΪUSART3
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);	   	//��ʼ��PD8,PD9
	USART3_Handler.Instance=USART3;					    //USART2
	USART3_Handler.Init.BaudRate=baud;				    //������
	USART3_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART3_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	USART3_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	USART3_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	USART3_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	USART3_Handler.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&USART3_Handler);
	HAL_NVIC_EnableIRQ(USART3_IRQn);		//ʹ��USART2�ж�ͨ��
	HAL_NVIC_SetPriority(USART3_IRQn,USART3_Priority,0);	//��ռ���ȼ�0�������ȼ�0
	HAL_UART_Receive_IT(&USART3_Handler,&aRxBuffer3, 1);
}
void USART3_IRQHandler(void)
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	HAL_UART_IRQHandler(&USART3_Handler);	//����HAL���жϴ����ú���
	timeout=0;
    while (HAL_UART_GetState(&USART3_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
	timeout=0;
	while(HAL_UART_Receive_IT(&USART3_Handler, &aRxBuffer3, 1) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}
}

#endif	//#if(USING_USART == 1)
