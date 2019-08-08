#ifndef _ENCRYPT_H_
#define _ENCRYPT_H_
#include "mytype.h"
#define PKT_SOF_DEFINE 0xff
#define PKT_EOF_DEFINE 0xff


#define BELONG_NONE							0
#define BELONG_BLUE_WEAK				1
#define BELONG_BLUE_STRONG			2
#define BELONG_RED_WEAK					3
#define BELONG_RED_STRONG				4

//��λ�����ְ�id1
//��λ���ش���id0

#define PID_MAP_DATA_0			0
#define PID_MAP_DATA_1			1
#define PID_MAP_DATA_2			2
#define PID_MAP_DATA_3			3
#define PID_MAP_DATA_4			4
#define PID_MAP_DATA_5			5

#define PID_CHASSIS_DATA_0		6
#define PID_CHASSIS_DATA_1		7
#define PID_CHASSIS_DATA_2		8

#define PID_ACK		9
#define PID_RESERVE_DATA		10
#define PID_HAND_SHAKE		11

#define HAND_SHAKE_DATA		[0xa5,0x5a,0xa5,0x5a,0xa5,0x5a,0xa5]

typedef struct
{
  struct castle_energy
  {
    uint8_t energy[2];
  }castle_energy[7];//���ҷ����еģ��ٵз����е�

  struct region_occupy
  {
    uint8_t status : 2; // 0 = no one, 1 = weak, 2 = strong
    uint8_t belong : 2; // 0 = no one, 1 = red, 2 = blue
    uint8_t have_robot : 2;
    uint8_t resv : 2;
  }region_occupy[9][7];

  uint8_t car_location[2];
  uint8_t round_remain_tick;
  uint8_t round_remain_cnt;
  uint8_t round_team;
  int16_t realtime_score[2];
} Summer_Camp_Info_t;

#pragma pack(2)

typedef struct
{
	short Motor_RPM[8];
	long gyro;
	long angle;
}Chassis_Data_t;

typedef struct
{
	unsigned char Castle_Energy[14];	//�ﱤ����
	unsigned char Region_Occupy[24];	//����״̬
	unsigned char Car_Location[2];		//��������λ��
	unsigned char Round_Remain_Time;	//�غ�ʣ��ʱ��
	unsigned char Round_Remain_Cnt;		//ʣ��غ�
	unsigned char Team;							//������Ӫ
	unsigned char Score[2];						//����ʵʱ�÷�
}Map_Data_t;


typedef struct
{
	unsigned char PKT_SOF;
	unsigned char PID;
	unsigned char Data[8];
	unsigned char Replace_Locate;
	unsigned char PKT_EOF;
}Comm_Pkt_t;

void Send_Chassis_Data(Chassis_Data_t *Chassis_Data);
void Append_Data(uint8_t src,uint8_t *dst);
uint8_t Cvt_Region_Occupy(uint8_t status, uint8_t belong);
void Cvt_Map_Data(Summer_Camp_Info_t *Summer_Camp_Info, Map_Data_t *Map_Data);
void Send_Map_Data(Map_Data_t *map_data);
void Send_Data(unsigned char pid, unsigned char data[8]);
#endif
