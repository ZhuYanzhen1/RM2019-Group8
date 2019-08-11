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

void Arm_Lift(void);  //��е��̧��
void Arm_Drop(void);	//��е���½�
void Hand_Grab(void);	//ץ��ץȡ
void Hand_Open(void);	//ץ���ſ�
void Hand_Drop(void);	//ץ�ַ����
void Ball_Place(unsigned char num);//����
void Ball_Reload(unsigned char num);//����
#endif
