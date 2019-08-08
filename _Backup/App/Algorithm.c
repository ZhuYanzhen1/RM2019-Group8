#include "User.h"

void Control_Task(void *pvParameter)
{
	while(1)
	{
		Chassis_Victor_y.User = 1;
		vTaskDelay(10);
	}
}
