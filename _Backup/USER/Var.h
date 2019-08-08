#ifndef _VAR_H_
#define _VAR_H_
#define PID_Time 5               //PID����ʱ��5ms/�� 200Hz
#define USART3_Priority 1
#define USART3_Receve_Length 2  //USART3���ջ�������С
#define UART_RX_DMA_SIZE (1024)		//����1��DMA2����
#define DBUS_MAX_LEN     (50)			//ң������󳤶�
#define DBUS_BUFLEN      (18)    //ң�����ַ�����
#define USART2_Priority 0	     //����2�����ж����ȼ�
#define USART2_Receve_Length 2 //USART2���ջ�������С
#define CAN1_Priority 0				//CAN1�����ж����ȼ�
#define CAN2_Priority 0				//CAN2�����ж����ȼ�
#define Flash_Base_Address 0x8180000  //�û���дFlash����ַ
#define PWM_FREQUENCE 50						//PWMƵ��
#define IN_CCM __attribute__((at(0x10000000)))  //��CCM�ڴ�������
#endif
