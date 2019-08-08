#include "User.h"
TaskHandle_t LED_Task_Handle;					//LED��˸���
TaskHandle_t PID_Calculate_Task;			//����PID���
TaskHandle_t Remote_Control_Task;			//ң�����ݴ�����
TaskHandle_t Base_Speed_Task_Handler; //���̿��ƾ��
TaskHandle_t Communicate_Task_Handler;//��λ��ͨ�ž��
TaskHandle_t Odometer_Task_Handler;		//��̼��ۼӾ��
TaskHandle_t Control_Task_Handler;    //�����㷨���
void User_Code(void)
{
	Power1_CTRL=1;   //��1�ŵ�Դ����
	Power2_CTRL=1;   //��2�ŵ�Դ����
	Power3_CTRL=1;   //��3�ŵ�Դ����
	Power4_CTRL=1;   //��4�ŵ�Դ����
	xTaskCreate((TaskFunction_t)LED_Flash,        //LED��˸����
							(const char *)"LED_TASK",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)1,
							(TaskHandle_t *)LED_Task_Handle);
	xTaskCreate((TaskFunction_t)PID_Calculate,    //���̵��PID�ջ�
							(const char *)"PID_Calculate",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)PID_Calculate_Task);
	xTaskCreate((TaskFunction_t)Remote_Control,   //ң�������ڴ���
							(const char *)"Remote_Control",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Remote_Control_Task);
	xTaskCreate((TaskFunction_t)Base_Speed_Task,	//����PID�ջ�
							(const char *)"Base_Speed_Task",
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Base_Speed_Task_Handler);
	xTaskCreate((TaskFunction_t)Communicate_Task,
							(const char *)"Communicate_Task",   //����ͨ��
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)2,
							(TaskHandle_t *)Communicate_Task_Handler);
	xTaskCreate((TaskFunction_t)Odometer_Summary,
							(const char *)"Odometer_Task",   //��̼��ۼ�
							(uint16_t)128,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)Odometer_Task_Handler);
	xTaskCreate((TaskFunction_t)Control_Task,
							(const char *)"Control_Task",   //�����㷨
							(uint16_t)512,
							(void *)NULL,
							(UBaseType_t)3,
							(TaskHandle_t *)Control_Task_Handler);
}
