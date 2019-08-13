#include "User.h"
#if(USING_TASK == 1)
unsigned long Total_Angle_Sum=0;
void Auto_Mode(void);
void Manual_Mode(void);


void Renew_PID(void)         //更新PID值
{
	PID_Renew(&PID1,moto_chassis[0].speed_rpm);
	PID_Renew(&PID2,moto_chassis[1].speed_rpm);
	PID_Renew(&PID3,moto_chassis[2].speed_rpm);
	PID_Renew(&PID4,moto_chassis[3].speed_rpm);
	PID_Renew(&PID5,moto_chassis[4].speed_rpm);
	PID_Renew(&Ball_Angle,moto_chassis[4].total_angle);
	
	if(Receive_Package2.Start_Flag==1)
	{
		PID_Renew(&Chassis_Wz,Mult_Vector_WZ);
		PID_Renew(&Chassis_Victor_y,Mult_Vector_Y);
		PID_Renew(&Chassis_Victor_x,Mult_Vector_X);
	}
}
void PID_Init(void)         //初始化PID值
{
	Set_PID(&PID1,M2006_SPEED);
	Set_PID(&PID2,M2006_SPEED);
	Set_PID(&PID3,M2006_SPEED);
	Set_PID(&PID4,M2006_SPEED);
	Set_PID(&PID5,M2006_SPEED);
	Set_PID(&Ball_Angle,CASCADE_ANGLE);
	
	Chassis_Vx.Kp=1;
	Chassis_Vy.Kp=1;
	Chassis_Vy.Maxinum=2;
	Chassis_Vy.Minium=-2;
	Chassis_Vx.Maxinum=2;
	Chassis_Vx.Minium=-2;

	Chassis_Wz.Kp=1.7;
	Chassis_Wz.Ki=0;
	Chassis_Wz.Kd=5;
	Chassis_Wz.I_Sum_Max=0;
	Chassis_Wz.Maxinum=30;
	Chassis_Wz.Minium=-30;
	
	Chassis_Victor_y.Kp=1;
	Chassis_Victor_y.Maxinum=1.5;
	Chassis_Victor_y.Minium=-1.5;

	Chassis_Victor_x.Kp=1;
	Chassis_Victor_x.Maxinum=1.5;
	Chassis_Victor_x.Minium=-1.5;
}


void Remote_Control(void* pvParameters)              //周期处理遥控器接收值
{
	while(1)
	{
		
		if(Remote.sw1==Left_H&&Remote.sw2==Right_H)
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

uint8_t last_cnt;
void Auto_Mode(void)
{
	if(Receive_Package2.Ball_Cnt != last_cnt)
	{
		last_cnt = Receive_Package2.Ball_Cnt;
		Ball_Reload(1);
		Ball_Reload(2);
		Ball_Reload(3);
		Ball_Reload(4);
		vTaskDelay(500);
		Ball_Place(1);
		Ball_Place(2);
		Ball_Place(3);
		Ball_Place(4);
	}
}

void Manual_Mode(void)
{
	if(Remote.sw1==Left_L&&Remote.sw2==Right_L)
	{
		Chassis_Vx.User = -1*(Remote.ch1/330.0f);
		Chassis_Vy.User = Remote.ch2/330.0f;
		Chassis_Wz.User = Remote.ch3/16.0f;
	}
	if(Remote.ch1==-660&&Remote.ch2==-660&&Remote.ch3==660&&Remote.ch4==-660)  //下内八放球
	{
		//装球
		Ball_Reload(1);
		Ball_Reload(2);
		Ball_Reload(3);
		Ball_Reload(4);
		vTaskDelay(500);
		Ball_Place(1);
		Ball_Place(2);
		Ball_Place(3);
		Ball_Place(4);
		while(1)
		{
			if(Remote.ch1==0&&Remote.ch2==0&&Remote.ch3==0&&Remote.ch4==0)
				break;
		}
	}
	else if(Remote.ch1==660&&Remote.ch2==-660&&Remote.ch3==-660&&Remote.ch4==-660)  //下外八放杯
	{
		//放杯
		Total_Angle_Sum+=49152;
		Ball_Angle.User=Total_Angle_Sum;
		vTaskDelay(500);
		Total_Angle_Sum+=49152;
		Ball_Angle.User=Total_Angle_Sum;
		while(1)
		{
			if(Remote.ch1==0&&Remote.ch2==0&&Remote.ch3==0&&Remote.ch4==0)
				break;
		}
	}
}

#endif	//#if(USING_TASK == 1)
