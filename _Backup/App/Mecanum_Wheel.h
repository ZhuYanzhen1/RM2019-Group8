#ifndef _MECANUM_WHEEL_H_
#define _MECANUM_WHEEL_H
#define A 0.0125f
#define B 0.0125f
#define WHEEL_R 0.0075f
#define PI 3.14159265358979f
void Set_Speed(float speed_x,float speed_y,float speed_w);
void Speed_To_Wheel(float vx_set, float vy_set,float wz_set,float wheel_speed[4]);
#endif
