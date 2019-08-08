#ifndef _MYIIC_H
#define _MYIIC_H
#include "System_Config.h"
#define IIC2_SCL   PFout(1) //SCL
#define IIC2_SDA   PFout(0) //SDA
#define READ_SDA2  PFin(0)  //����SDA
void IIC2_Init(void);                //��ʼ��IIC2��IO��				 
void IIC2_Start(void);				//����IIC2��ʼ�ź�
void IIC2_Stop(void);	  			//����IIC2ֹͣ�ź�
void IIC2_Send_Byte(u8 txd);			//IIC2����һ���ֽ�
u8 IIC2_Read_Byte(unsigned char ack);//IIC2��ȡһ���ֽ�
u8 IIC2_Wait_Ack(void); 				//IIC2�ȴ�ACK�ź�
void IIC2_Ack(void);					//IIC2����ACK�ź�
void IIC2_NAck(void);				//IIC2������ACK�ź�
void IIC2_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC2_Read_One_Byte(u8 daddr,u8 addr);	 
#endif

