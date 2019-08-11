#ifndef _MACHINE_ARM_H_
#define _MACHINE_ARM_H_

#define PWM_CH1_OPEN				0.5f
#define PWM_CH2_OPEN				0.5f
#define PWM_CH3_OPEN				0.5f
#define PWM_CH4_OPEN				0.5f

#define PWM_CH1_CLOSE				1.2f
#define PWM_CH2_CLOSE				1.2f
#define PWM_CH3_CLOSE				1.2f
#define PWM_CH4_CLOSE				1.2f


void Ball_Place(unsigned char num);//放球
void Ball_Reload(unsigned char num);//补弹
void Cup_Press(void);	//按杯子
void Cup_Release(void);	//松开杯子

#endif
