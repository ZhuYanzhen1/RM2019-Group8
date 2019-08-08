#include "User.h"
uint16_t Append_Cnt = 0;
void Send_Chassis_Data(Chassis_Data_t *chassis_Data)
{
	uint8_t cnt = 0;											//ѭ������
	uint8_t data[8];											//Ҫ���͵�����
	memset(data,0,8);											//��շ������ݻ���
	uint8_t buffer[sizeof(Chassis_Data_t)];		//��������	
	for(cnt = 0;cnt<8;cnt++)
	{
		chassis_Data->Motor_RPM[cnt] = (short)moto_chassis[cnt].speed_rpm;
	}
	chassis_Data->gyro = (long)((single_gyro.gyro*10000)/1);
	chassis_Data->angle = (long)((single_gyro.angle*10000)/1);
	memset(buffer,0,sizeof(Chassis_Data_t));		//���㻺��
	memcpy(buffer, chassis_Data,sizeof(Chassis_Data_t));//�Ȱѽṹ�����ݸ��Ƶ�������
	//һ֡һ֡��������

	memcpy(data, buffer,8);
	Send_Data(PID_CHASSIS_DATA_0,data);
	
	memcpy(data, buffer+8,8);
	Send_Data(PID_CHASSIS_DATA_1,data);
	
	data[0]=(chassis_Data->gyro>>24);
	data[1]=((chassis_Data->gyro&0x00ff0000)>>16);
	data[2]=((chassis_Data->gyro&0x0000ff00)>>8);
	data[3]=(chassis_Data->gyro&0x000000ff);
	data[4]=(chassis_Data->angle>>24);
	data[5]=((chassis_Data->angle&0x00ff0000)>>16);
	data[6]=((chassis_Data->angle&0x0000ff00)>>8);
	data[7]=(chassis_Data->angle&0x000000ff);
	Send_Data(PID_CHASSIS_DATA_2,data);
}
void Append_Data(uint8_t src,uint8_t *dst)
{
	if(Append_Cnt%8 == 0){
		*(dst+Append_Cnt/8) = src<<5;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 1){
		*(dst+(Append_Cnt-1)/8) |= src<<4;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 2){
		*(dst+(Append_Cnt-2)/8) |= src<<3;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 3){
		*(dst+(Append_Cnt-3)/8) |= src<<2;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 4){
		*(dst+(Append_Cnt-4)/8) |= src<<1;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 5){
		*(dst+(Append_Cnt-5)/8) |= src;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 6){
		*(dst+(Append_Cnt-6)/8) |= src>>1;
		*(dst+(Append_Cnt+2)/8) |= src<<7;
		Append_Cnt+=3;
	}
	else if(Append_Cnt%8 == 7){
		*(dst+(Append_Cnt-7)/8) |= src>>2;
		*(dst+(Append_Cnt+1)/8) |= src<<6;
		Append_Cnt+=3;
	}
}
uint8_t Cvt_Region_Occupy(uint8_t status, uint8_t belong)
{
	if(belong == 0)
	{
		return BELONG_NONE;
	}
	else if(belong == 1)
	{
		if(status == 1) return BELONG_RED_WEAK;
		if(status == 2) return BELONG_RED_STRONG;
	}
	else if(belong == 2)
	{
		if(status == 1) return BELONG_BLUE_WEAK;
		if(status == 2) return BELONG_BLUE_STRONG;
	}
	return BELONG_NONE;				//�����ܵ����λ��
}
void Cvt_Map_Data(Summer_Camp_Info_t *Summer_Camp_Info, Map_Data_t *Map_Data)
{
	uint8_t cnt = 0;					//������ʱ����
	//���Ǳ���������
	for(cnt=0;cnt<7;cnt++)
	{
		Map_Data->Castle_Energy[cnt] = Summer_Camp_Info->castle_energy[cnt].energy[0];
	}
	for(cnt=0;cnt<7;cnt++)
	{
		Map_Data->Castle_Energy[cnt+7] = Summer_Camp_Info->castle_energy[cnt].energy[1];
	}
	Append_Cnt = 0;						//���λ���ۼ�ֵ
	//ת����������
	for(cnt=0;cnt<7;cnt++)
	{
		uint8_t inner_cnt;
		for(inner_cnt=0;inner_cnt<9;inner_cnt++)
		{
			uint8_t status = Cvt_Region_Occupy(Summer_Camp_Info-> region_occupy[inner_cnt][cnt].status,\
			Summer_Camp_Info-> region_occupy[inner_cnt][cnt].belong);
			
			Append_Data(status,Map_Data->Region_Occupy);
		}
	}
	//���Ƴ���λ��
	Map_Data->Car_Location[0] = Summer_Camp_Info->car_location[0];
	Map_Data->Car_Location[1] = Summer_Camp_Info->car_location[1];
	//���ƻغ�ʣ��ʱ��
	Map_Data->Round_Remain_Time = Summer_Camp_Info->round_remain_tick;
	//����ʣ��غ�
	Map_Data->Round_Remain_Cnt = Summer_Camp_Info->round_remain_cnt;
	//����������Ӫ
	Map_Data->Team = Summer_Camp_Info->round_team;
	//����˫���÷�
	Map_Data->Score[0] = Summer_Camp_Info->realtime_score[0];
	Map_Data->Score[1] = Summer_Camp_Info->realtime_score[1];
}
void Send_Map_Data(Map_Data_t *map_data)
{
	uint8_t data[8];											//Ҫ���͵�����
	memset(data,0,8);											//��շ������ݻ���
	uint8_t buffer[sizeof(Map_Data_t)];		//��������	
	memset(buffer,0,sizeof(Map_Data_t));	
	memcpy(buffer, map_data,sizeof(Map_Data_t));//�Ȱѽṹ�����ݸ��Ƶ�������
	//һ֡һ֡��������
	memcpy(data, buffer,8);
	Send_Data(PID_MAP_DATA_0,data);
	memcpy(data, buffer+8,7);
	data[7] = data[6];
	data[6] = 0;
	Send_Data(PID_MAP_DATA_1,data);
	memcpy(data, buffer+15,8);
	Send_Data(PID_MAP_DATA_2,data);
	memcpy(data, buffer+23,8);
	Send_Data(PID_MAP_DATA_3,data);
	memcpy(data, buffer+31,8);
	Send_Data(PID_MAP_DATA_4,data);
	memcpy(data, buffer+39,6);
	Send_Data(PID_MAP_DATA_5,data);
}
void Send_Data(unsigned char pid, unsigned char data[])
{
	Comm_Pkt_t pkt;										//����ṹ��
	memset(&pkt,0,sizeof(Comm_Pkt_t));
	uint8_t cnt = 0;									//ѭ����ʱ����
	pkt.PKT_SOF =  PKT_SOF_DEFINE;		//���֡ͷ
	pkt.Replace_Locate = 0;
	//�������ݲ�ת������0xff����¼λ��
	for(cnt = 0;cnt<8;cnt++)
	{
		if(data[cnt] == 0xff)
		{
			//���������0xff�����滻��0�������
			pkt.Data[cnt] = 0x00;
			pkt.Replace_Locate = pkt.Replace_Locate | (1 << cnt);
		}
		else{
			pkt.Data[cnt] = data[cnt];
		}
	}
	pkt.PID = pid<<4 | ((~pid) & 0x0f);										//����pid,��ȡ��У��
	pkt.PKT_EOF = PKT_EOF_DEFINE;				//���֡β
	HAL_UART_Transmit(&USART2_Handler,(uint8_t *)&pkt,12,100);	//������֡
}
