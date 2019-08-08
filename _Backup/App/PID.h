#ifndef _PID_H_
#define _PID_H_
typedef struct
{
	float Kp;   //比例系数
	float Ki;   //积分系数
	float Kd;   //微分系数
	float Offset;  //偏移量
	float Collect[2];  //前两次采集到的数据,Collect[0]为最近一次采集的数据
	float I_Sum;       //积分项累加值
	float User;        //用户设定值
	float Result;      //运算结果
	float Maxinum;     //输出最大值
	float Minium;      //输出最小值
	float I_Sum_Max;   //I项最大值
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
