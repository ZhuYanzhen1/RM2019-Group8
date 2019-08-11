#ifndef _MACHINE_ARM_H_
#define _MACHINE_ARM_H_

#define PWM_CH1_OPEN				0.5f
#define PWM_CH2_OPEN				0.5f
#define PWM_CH3_OPEN				0.5f
#define PWM_CH4_OPEN				0.5f

#define PWM_CH1_CLOSE				0.9f
#define PWM_CH2_CLOSE				0.9f
#define PWM_CH3_CLOSE				0.9f
#define PWM_CH4_CLOSE				0.9f

void Arm_Lift(void);  //机械臂抬升
void Arm_Drop(void);	//机械臂下降
void Hand_Grab(void);	//抓手抓取
void Hand_Open(void);	//抓手张开
void Hand_Drop(void);	//抓手放物块
void Ball_Place(unsigned char num);//放球
void Ball_Reload(unsigned char num);//补弹
#endif
