#ifndef _PID_H_
#define _PID_H_
typedef struct
{
	float Kp;   //����ϵ��
	float Ki;   //����ϵ��
	float Kd;   //΢��ϵ��
	float Offset;  //ƫ����
	float Collect[2];  //ǰ���βɼ���������,Collect[0]Ϊ���һ�βɼ�������
	float I_Sum;       //�������ۼ�ֵ
	float User;        //�û��趨ֵ
	float Result;      //������
	float Maxinum;     //������ֵ
	float Minium;      //�����Сֵ
	float I_Sum_Max;   //I�����ֵ
}PID_Structure;
void PID_Calculate(void* pvParameters);
void Base_Speed_Task(void* pvParameters);
float PID_Get_Result(PID_Structure* PID_Handler);
extern PID_Structure PID1;
extern PID_Structure PID2;
extern PID_Structure PID3;
extern PID_Structure PID4;
extern PID_Structure PID5;
extern PID_Structure PID6;
extern PID_Structure PID7;
extern PID_Structure PID8;
extern PID_Structure Chassis_Wz;
extern PID_Structure Chassis_Vx;
extern PID_Structure Chassis_Vy;
extern PID_Structure Chassis_Victor_y;
extern PID_Structure Chassis_Victor_x;
#endif
