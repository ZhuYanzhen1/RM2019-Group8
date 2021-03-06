#include "User.h"
USART2_Receive_Data_Package Receive_Package2;
USART3_Receive_Data_Package Receive_Package3;
void USART3_Received_CallBack(unsigned char Receive_Buffer[])
{
		unsigned char Buffer_Data_Counter=0;
		unsigned char Buffer_Flag=0;
		while(1)
		{
			if(Buffer_Flag == 0 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
				Buffer_Flag = 1;
			else if(Buffer_Flag == 1 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
				break;
				Buffer_Data_Counter++;
		}
		Buffer_Data_Counter++;
		if(Buffer_Data_Counter == 12)   //if the package had been broken
    {
				long Temp_X,Temp_Y;
        uint8_t Data_Buffer[8];
        uint8_t PID_Bit = Receive_Buffer[1]>>4;  //Get The PID Bit
        if(PID_Bit == ((~(Receive_Buffer[1] & 0x0f)) & 0x0f) )  //PID Verify Success
        {
            uint8_t Temp_Var = 0x00 ;
            while(1)     //Memory Copy
            {
                Data_Buffer[Temp_Var] = Receive_Buffer[2+Temp_Var];
                Temp_Var++;
                if(Temp_Var == 8)
                    break;
            }
            if(Receive_Buffer[10] != 0x00)   //Some Byte had been replace
            {
                uint8_t Temp_Filter = 0x00;
								Temp_Var = 0;
                while(1)
                {
                    if(((Receive_Buffer[10] & (Temp_Filter|(0x01<<Temp_Var)))>>Temp_Var) == 1)  //This Byte Need To Adjust
                        Data_Buffer[Temp_Var] = 0xff;    //Adjust to 0xff
                    Temp_Var++;
                    if(Temp_Var == 8)
                        break;
                }
						}
						
            switch(PID_Bit)
            {
                case LOCATION:
										Temp_X = ((Data_Buffer[0] << 24)|(Data_Buffer[1] << 16)|(Data_Buffer[2] << 8)|Data_Buffer[3]);
										Temp_Y = ((Data_Buffer[4] << 24)|(Data_Buffer[5] << 16)|(Data_Buffer[6] << 8)|Data_Buffer[7]);
									  Receive_Package3.X_Possition = (Temp_X/10000.0f);
										Receive_Package3.Y_Possition = (Temp_Y/10000.0f);
										break;
								case ANGLE:
										Temp_X = ((Data_Buffer[0] << 24)|(Data_Buffer[1] << 16)|(Data_Buffer[2] << 8)|Data_Buffer[3]);
									  Receive_Package3.Angle = (Temp_X/10000.0f);
										break;
            }
        }
			}
}


void USART2_Received_CallBack(unsigned char Receive_Buffer[])
{
		unsigned char Buffer_Data_Counter=0;
		unsigned char Buffer_Flag=0;
		while(1)
		{
			if(Buffer_Flag == 0 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
				Buffer_Flag = 1;
			else if(Buffer_Flag == 1 && Receive_Buffer[Buffer_Data_Counter] == 0xff)
				break;
				Buffer_Data_Counter++;
		}
		Buffer_Data_Counter++;
		if(Buffer_Data_Counter == 12)   //if the package had been broken
    {
        uint8_t Data_Buffer[8];
        uint8_t PID_Bit = Receive_Buffer[1]>>4;  //Get The PID Bit
        if(PID_Bit == ((~(Receive_Buffer[1] & 0x0f)) & 0x0f) )  //PID Verify Success
        {
            uint8_t Temp_Var = 0x00 ;
            while(1)     //Memory Copy
            {
                Data_Buffer[Temp_Var] = Receive_Buffer[2+Temp_Var];
                Temp_Var++;
                if(Temp_Var == 8)
                    break;
            }
            if(Receive_Buffer[10] != 0x00)   //Some Byte had been replace
            {
                uint8_t Temp_Filter = 0x00;
								Temp_Var = 0;
                while(1)
                {
                    if(((Receive_Buffer[10] & (Temp_Filter|(0x01<<Temp_Var)))>>Temp_Var) == 1)  //This Byte Need To Adjust
                        Data_Buffer[Temp_Var] = 0xff;    //Adjust to 0xff
                    Temp_Var++;
                    if(Temp_Var == 8)
                        break;
                }
						}
						
            switch(PID_Bit)
            {
                case POS_BALL_CNT:
										Receive_Package2.Ball_Cnt = Data_Buffer[0];
										Receive_Package2.Cup_Cnt = Data_Buffer[1];
										Receive_Package2.Start_Flag=Data_Buffer[2];
										
										Receive_Package2.X_Possition=((Data_Buffer[4]<<8)|Data_Buffer[5])/1000.0f;
										Receive_Package2.Y_Possition=((Data_Buffer[6]<<8)|Data_Buffer[7])/1000.0f;
								
										break;
            }
        }
			}
}
