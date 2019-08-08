#include "User.h"
#if(USING_TASK == 1)

void Renew_PID(void)         //更新PID值
{
	PID1.Collect[1] = PID1.Collect[0];
	PID1.Collect[0] = moto_chassis[0].speed_rpm;
	PID2.Collect[1] = PID2.Collect[0];
	PID2.Collect[0] = moto_chassis[1].speed_rpm;
	PID3.Collect[1] = PID3.Collect[0];
	PID3.Collect[0] = moto_chassis[2].speed_rpm;
	PID4.Collect[1] = PID4.Collect[0];
	PID4.Collect[0] = moto_chassis[3].speed_rpm;
	Chassis_Vy.Collect[1] = Chassis_Vy.Collect[0];
	Chassis_Vy.Collect[0] = VO_Y;
	Chassis_Vx.Collect[1] = Chassis_Vx.Collect[0];
	Chassis_Vx.Collect[0] = VO_X;
	Chassis_Wz.Collect[1] = Chassis_Wz.Collect[0];
	Chassis_Wz.Collect[0] = single_gyro.angle;
	Chassis_Victor_y.Collect[1] = Chassis_Victor_y.Collect[0];
	Chassis_Victor_y.Collect[0] = VectorO_Y;
}
void PID_Init(void)         //初始化PID值
{
	PID1.Kp=3;
	PID1.Ki=0.1;
	PID1.Kd=0.1;
	PID1.I_Sum_Max=5000;
	PID1.Maxinum=10000;
	PID1.Minium=-10000;
	
	PID2.Kp=3;
	PID2.Ki=0.1;
	PID2.Kd=0.1;
	PID2.I_Sum_Max=5000;
	PID2.Maxinum=10000;
	PID2.Minium=-10000;
	
	PID3.Kp=3;
	PID3.Ki=0.1;
	PID3.Kd=0.1;
	PID3.I_Sum_Max=5000;
	PID3.Maxinum=10000;
	PID3.Minium=-10000;
	
	PID4.Kp=3;
	PID4.Ki=0.1;
	PID4.Kd=0.1;
	PID4.I_Sum_Max=5000;
	PID4.Maxinum=10000;
	PID4.Minium=-10000;
	
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
			Chassis_Vx.User = Remote.ch1/330.0f;
			Chassis_Vy.User = Remote.ch2/330.0f;
			Chassis_Wz.User = Remote.ch3/16.0f;
		}
		else if(Remote.sw1 == Left_H && Remote.sw2 == Right_H)
		{
			Chassis_Victor_y.User = 0.6;
			Chassis_Victor_x.User = 0.6;
		}
		else
		{
			Chassis_Vx.User = 0;
			Chassis_Vy.User = 0;
			Chassis_Wz.User = 0;
		}
		vTaskDelay(5);
	}
}

#endif	//#if(USING_TASK == 1)
