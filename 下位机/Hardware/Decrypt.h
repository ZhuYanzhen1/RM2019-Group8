#ifndef _DECRYPT_H_
#define _DECRYPT_H_

#define LOCATION 0x01
#define ANGLE 0x02

typedef struct
{
  float X_Possition;
	float Y_Possition;
	float Angle;
}USART3_Receive_Data_Package;

typedef struct
{
  unsigned char Ball_Cnt;
	unsigned char Cup_Cnt;
}USART2_Receive_Data_Package;
#define BALL_CUP_CNT 				0x00
void USART3_Received_CallBack(unsigned char Receive_Buffer[]);
void USART2_Received_CallBack(unsigned char Receive_Buffer[]);
extern USART2_Receive_Data_Package Receive_Package2;
extern USART3_Receive_Data_Package Receive_Package3;
#endif
