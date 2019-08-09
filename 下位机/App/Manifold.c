#include "User.h"
#if ( USING_MANIFOLD == 1 )
Summer_Camp_Info_t Summer_Camp_Info;
Map_Data_t Map_Data;
Chassis_Data_t Chassis_Data;
void Communicate_Task(void * pvParameters)
{
	while(1)
	{
//		Cvt_Map_Data(&Summer_Camp_Info, &Map_Data);
//		Send_Map_Data(&Map_Data);
		Send_Chassis_Data(&Chassis_Data);
		vTaskDelay(100);
	}
}

#endif	//#if(USING_MANIFOLD == 1)
