#ifndef _ODOMETER_H_
#define _ODOMETER_H_
extern float Odom_VectorO_X , Odom_VectorO_Y , Odom_RotateO ,VO_X,VO_Y;
extern float Des_Vx , Des_Vy , Des_Wz , L_Des_Vx , L_Des_Vy , L_Des_Wz;;
void Odometer_Summary(void *pvParameters);
void Mecanum_To_Chassis(float Wheel[4],float *Vx,float *Vy,float *Wz);
#endif
