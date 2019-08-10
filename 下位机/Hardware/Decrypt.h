#ifndef _DECRYPT_H_
#define _DECRYPT_H_

#define LOCATION 0x01
#define ANGLE 0x02

typedef struct
{
  float X_Possition;
	float Y_Possition;
	float Angle;
}Receive_Data_Package;
void Received_CallBack(unsigned char Receive_Buffer[]);
extern Receive_Data_Package Receive_Package;
#endif
