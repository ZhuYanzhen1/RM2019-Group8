#include "User.h"
TaskHandle_t Init_Task_Hanlder;
void Init_Task(void* pvParameters)
{
	taskENTER_CRITICAL();
	u8 num=1;
	oled_init();										//��ʼ��OLED
	oled_LOGO();										//OLED��ʾRM��LOGO
	Delayms(300);
	oled_clear(Pen_Clear);
	oled_printf(num,Pen_Write,"OLED Init Success");  //1
	LED_Init();											//��ʼ��LED
	num++;
	oled_printf(num,Pen_Write,"LED Init Success");   //2
	Random_Init();									//��ʼ��Ӳ�������
	num++;
	oled_printf(num,Pen_Write,"RNG Init Success");   //3
	USART2_init(115200);							//��ʼ������2(��������)��������115200
	num++;
	oled_printf(num,Pen_Write,"USART2 Init Success");//4
	ADC1_Init();										//��ʼ��ADC
	num=1;
	oled_clear(Pen_Clear);
	oled_printf(num,Pen_Write,"ADC1 Init Success");  //1
	DAC1_Init();										//��ʼ��DAC1
	num++;
	oled_printf(num,Pen_Write,"DAC1 Init Success");  //2
	DAC2_Init();										//��ʼ��DAC2
	num++;
	oled_printf(num,Pen_Write,"DAC2 Init Success");  //3
	IIC2_Init();										//��ʼ��IIC2
	num++;
	oled_printf(num,Pen_Write,"IIC2 Init Success");  //4
	num=1;
	oled_clear(Pen_Clear);
	Key_Init();											//��ʼ���û�����
	oled_printf(num,Pen_Write,"Key Init Success");   //1
	num++;
	oled_printf(num,Pen_Write,"Malloc Init Success"); //2
	Beep_Startup();									//��ʼ��������
	num++;
	oled_printf(num,Pen_Write,"Beep Init Success");  //3
	num++;
	MX_CAN1_Init();									//��ʼ��CAN1
	oled_printf(num,Pen_Write,"CAN1 Init Success");  //4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	MX_CAN2_Init();									//��ʼ��CAN2
	oled_printf(num,Pen_Write,"CAN2 Init Success");  //1
	num++;
	PWM_Init();											//��ʼ��PWM
	oled_printf(num,Pen_Write,"PWM Init Success");   //2
	num++;
	USART3_Init(115200);						//��ʼ�����㽻����
	oled_printf(num,Pen_Write,"USART3 Init Success");
	num++;
	Laser_Init();										//��ʼ������
	oled_printf(num,Pen_Write,"Laser Init Success");//4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	Remote_Init();									//��ʼ��ң����
	oled_printf(num,Pen_Write,"Remote Init Success"); //1
	num++;
	Power_Init();										//��ʼ������
	oled_printf(num,Pen_Write,"Power Init Success"); //2
	num=1;
	oled_clear(Pen_Clear);
	SPI4_Init();										//��ʼ��SPI4
	oled_printf(num,Pen_Write,"SPI4 Init Success");  //3
	num++;
	Motor_Init();										//��ʼ�����
	oled_printf(num,Pen_Write,"Motor Init Success"); //4
	num++;
	num=1;
	oled_clear(Pen_Clear);
	USART2_Receve_Handler.Buffer_Counter=-1;
	USART3_Receve_Handler.Buffer_Counter=-1;
	PID_Init();                     //��ʼ��PID����
	User_Code();										//�����û�����
	taskEXIT_CRITICAL();
	vTaskDelete(Init_Task_Hanlder);
}
int main(void)
{
	HAL_Init();                     //��ʼ��HAL��    
	Stm32_Clock_Init(360,12,2,8);   //����ʱ��,180Mhz
	xTaskCreate((TaskFunction_t)Init_Task,
							(const char *)"Init_Task",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Init_Task_Hanlder);
	vTaskStartScheduler();
}
