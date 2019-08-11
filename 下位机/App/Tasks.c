#include "User.h"
#if(USING_TASK == 1)

void Auto_Mode(void);
void Manual_Mode(void);


void Renew_PID(void)         //更新PID值
{
	PID_Renew(&PID1,moto_chassis[0].speed_rpm);
	PID_Renew(&PID2,moto_chassis[1].speed_rpm);
	PID_Renew(&PID3,moto_chassis[2].speed_rpm);
	PID_Renew(&PID4,moto_chassis[3].speed_rpm);
	PID_Renew(&PID5,moto_chassis[4].speed_rpm);
	PID_Renew(&PID6,moto_chassis[5].speed_rpm);
	PID_Renew(&PID7,moto_chassis[6].speed_rpm);
	PID_Renew(&PID8,moto_chassis[7].speed_rpm);
	PID_Renew(&Ball2_Angle,moto_chassis[7].total_angle);
	PID_Renew(&Ball1_Angle,moto_chassis[6].total_angle);
	
//	Chassis_Vy.Collect[1] = Chassis_Vy.Collect[0];
//	Chassis_Vy.Collect[0] = VO_Y;
//	Chassis_Vx.Collect[1] = Chassis_Vx.Collect[0];
//	Chassis_Vx.Collect[0] = VO_X;
//	Chassis_Wz.Collect[1] = Chassis_Wz.Collect[0];
//	Chassis_Wz.Collect[0] = single_gyro.angle;
//	Chassis_Victor_y.Collect[1] = Chassis_Victor_y.Collect[0];
//	Chassis_Victor_y.Collect[0] = VectorO_Y;
}
void PID_Init(void)         //初始化PID值
{
	Set_PID(&PID1,M2006_SPEED);
	Set_PID(&PID2,M2006_SPEED);
	Set_PID(&PID3,M2006_SPEED);
	Set_PID(&PID4,M2006_SPEED);
	Set_PID(&PID5,M3510_SPEED);
	Set_PID(&PID6,M3510_SPEED);
	Set_PID(&PID7,M2006_SPEED);
	Set_PID(&PID8,M2006_SPEED);
	Set_PID(&Ball2_Angle,CASCADE_ANGLE);
	Set_PID(&Ball1_Angle,CASCADE_ANGLE);
	
	Chassis_Vx.Kp=1;
	Chassis_Vy.Kp=1;
	Chassis_Vy.Maxinum=2;
	Chassis_Vy.Minium=-2;
	Chassis_Vx.Maxinum=2;
	Chassis_Vx.Minium=-2;

	Chassis_Wz.Kp=1.7;
	Chassis_Wz.Ki=0;
	Chassis_Wz.Kd=5;
	Chassis_Wz.I_Sum_Max=20;
	Chassis_Wz.Maxinum=30;
	Chassis_Wz.Minium=-30;
	
	Chassis_Victor_y.Kp=1;
	Chassis_Victor_y.Maxinum=0.5;
	Chassis_Victor_y.Minium=-0.5;

	Chassis_Victor_x.Kp=1;
	Chassis_Victor_x.Maxinum=0.5;
	Chassis_Victor_x.Minium=-0.5;
}
void Remote_Control(void* pvParameters)              //周期处理遥控器接收值
{
	while(1)
	{
		if(Remote.sw1==Left_L&&Remote.sw2==Right_L)
		{
			Auto_Mode();
		}
		else
		{
			Manual_Mode();
		}
		vTaskDelay(5);

	}
}


void Auto_Mode(void)
{
	
}

void Manual_Mode(void)
{
	Chassis_Vx.User = -1*(Remote.ch1/330.0f);
	Chassis_Vy.User = Remote.ch2/330.0f;
	Chassis_Wz.User = Remote.ch3/16.0f;
	if(Remote.sw1==Left_M&&Remote.sw2==Right_L)
	{
		//装球
		Ball_Reload(1);
		Ball_Reload(2);
		Ball_Reload(3);
		Ball_Reload(4);
	}
	else if(Remote.sw1==Left_M&&Remote.sw2==Right_M)
	{
		//放球
		Ball_Place(1);
		Ball_Place(2);
		Ball_Place(3);
		Ball_Place(4);
	}
	if(Remote.sw1==Left_H&&Remote.sw2==Right_H)
	{
		//放杯
	}
}

#endif	//#if(USING_TASK == 1)
