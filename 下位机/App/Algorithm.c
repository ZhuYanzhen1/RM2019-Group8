#include "User.h"

float Mult_Vector_X;
float Mult_Vector_Y;
float Mult_Vector_WZ;

float Last_CV_Vector_X;
float Last_CV_Vector_Y;
float Last_CV_Vector_Angle;

float Odom_Vector_X_Start_Val;
float Odom_Vector_Y_Start_Val;
float Odom_Vector_Angle_Start_Val;

uint8_t CV_Offline = 0;

void Control_Task(void *pvParameter)
{
	while(1)
	{
		
		if(Receive_Package.X_Possition != 0 && \
			Receive_Package.Y_Possition != 0 && \
			Receive_Package.Angle != 0)
		{
			Mult_Vector_X = Receive_Package.X_Possition;
			Mult_Vector_Y = Receive_Package.Y_Possition;
			Mult_Vector_WZ = Receive_Package.Angle;
			
			Last_CV_Vector_X = Receive_Package.X_Possition;
			Last_CV_Vector_Y = Receive_Package.Y_Possition;
			Last_CV_Vector_Angle = Receive_Package.Angle;
			
			CV_Offline = 0;
		}
		if( ( Receive_Package.X_Possition == 0 || \
			Receive_Package.Y_Possition == 0 || \
			Receive_Package.Angle == 0 ) && CV_Offline == 0 )
		{
			Odom_Vector_X_Start_Val = Odom_VectorO_X;
			Odom_Vector_Y_Start_Val = Odom_VectorO_Y;
			Odom_Vector_Angle_Start_Val = Odom_RotateO;
			
			CV_Offline = 1;
		}
		
		if(CV_Offline == 1)
		{
			Mult_Vector_X = Last_CV_Vector_X + (Odom_VectorO_X - Odom_Vector_X_Start_Val);
			Mult_Vector_Y = Last_CV_Vector_Y + (Odom_VectorO_Y - Odom_Vector_Y_Start_Val);
			Mult_Vector_WZ = Last_CV_Vector_Angle + (Odom_RotateO - Odom_Vector_Angle_Start_Val);
		}
		vTaskDelay(10);
	}
}
