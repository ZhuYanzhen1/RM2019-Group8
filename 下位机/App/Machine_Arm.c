#include "User.h"

#if(USING_MACHINE_ARM == 1)

void Ball_Place(unsigned char num)		//·ÅÇò
{
	if(num == 1) PWM_SetDuty(&htim5,TIM_CHANNEL_1,(double)PWM_CH1_OPEN/20.0);
	if(num == 2) PWM_SetDuty(&htim5,TIM_CHANNEL_2,(double)PWM_CH2_OPEN/20.0);
	if(num == 3) PWM_SetDuty(&htim5,TIM_CHANNEL_3,(double)PWM_CH3_OPEN/20.0);
	if(num == 4) PWM_SetDuty(&htim5,TIM_CHANNEL_4,(double)PWM_CH4_OPEN/20.0);
}


void Ball_Reload(unsigned char num)		//ÖØ×°Çò
{
	if(num == 1) PWM_SetDuty(&htim5,TIM_CHANNEL_1,(double)PWM_CH1_CLOSE/20.0);
	if(num == 2) PWM_SetDuty(&htim5,TIM_CHANNEL_2,(double)PWM_CH2_CLOSE/20.0);
	if(num == 3) PWM_SetDuty(&htim5,TIM_CHANNEL_3,(double)PWM_CH3_CLOSE/20.0);
	if(num == 4) PWM_SetDuty(&htim5,TIM_CHANNEL_4,(double)PWM_CH4_CLOSE/20.0);
}

void Cup_Press(void)
{
	PWM_SetDuty(&htim4,TIM_CHANNEL_1,(double)0.6/20.0);
}

void Cup_Release(void)
{
	PWM_SetDuty(&htim4,TIM_CHANNEL_1,(double)1.2/20.0);
}

#endif	//#if(USING_MACHINE_ARM == 1)
