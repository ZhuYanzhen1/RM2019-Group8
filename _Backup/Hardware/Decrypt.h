#ifndef _DECRYPT_H_
#define _DECRYPT_H_
#define MAP_DATA0 0
#define MAP_DATA1 1
#define MAP_DATA2 2
#define MAP_DATA3 3
#define MAP_DATA4 4
#define MAP_DATA5 5
#define CHASSIS_DATA0 6
#define CHASSIS_DATA1 7
#define CHASSIS_DATA2 8
#define ACK_PACKAGE 9

#define MOTOR_DATA0 2
#define MOTOR_DATA1 3
//#define 
typedef struct
{
	short Chassis_Vx;
	short Chassis_Vy;
	short Chassis_Wz;
	unsigned char Mode_Now;

}Transmit_Data_Package;
typedef struct
{
    unsigned char Castle_Bule[7];   //The Number Of Every Castle Score Blue Side
    unsigned char Castle_Red[7];   //The Number Of Every Castle Score Red Side
    unsigned char Block[63];   //Every Block Status
    unsigned char Car_Blue[2]; //First Byte is X
    unsigned char Car_Red[2]; //First Byte is X 
    unsigned char Least_Time;  //How Many Time Have Left
    unsigned char Score_Blue;    //Blue Score Now
		unsigned char Score_Red;    //Red Score Now
    short Motor_Speed[8];//Eight Motor Speed (rpm)  from motor1~motor8
    long Gryo_Scope_Gryo; //Gryo Speed from sensor now
		long Gryo_Scope_Angle;//Angle Sumary from sensor now
}Receive_Data_Package;
typedef enum
{
	Block_None = 0,
	Blue_Weak = 1,
	Blue_Strong = 2,
	Red_Weak = 3,
	Red_Strong = 4,
}Block_Status;
void Received_CallBack(unsigned char Receive_Buffer[]);
#endif
