#include "User.h"
Receive_Data_Package Receive_Package;
void Received_CallBack(unsigned char Receive_Buffer[])
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
						uint8_t Temp_Register = 0x00;  //Which Contains the data haven't been Handle
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
                    case MAP_DATA0:
												Temp_Var = 0;
												while (1)
												{
													Receive_Package.Castle_Bule[Temp_Var] = Data_Buffer[Temp_Var];
													Temp_Var++;
													if (Temp_Var == 7)
														break;
												}
												Receive_Package.Castle_Red[0] = Data_Buffer[7];
												break;
										case MAP_DATA1:
												Temp_Var = 0;
												while (1)
												{
													Receive_Package.Castle_Red[Temp_Var + 1] = Data_Buffer[Temp_Var];
													Temp_Var++;
													if (Temp_Var == 6)
														break;
												}
												Receive_Package.Block[0] = (Data_Buffer[7] >> 5);
												Receive_Package.Block[1] = (((Data_Buffer[7] & 0x1C) >> 2));
												Temp_Register = Data_Buffer[7] & 0x03;
												break;
										case MAP_DATA2:
												Receive_Package.Block[2] = ((Temp_Register << 1) | Data_Buffer[0] >> 7);
												Receive_Package.Block[3] = ((Data_Buffer[0] & 0x70) >> 4);
												Receive_Package.Block[4] = ((Data_Buffer[0] & 0x0e) >> 1);
												Receive_Package.Block[5] = (((Data_Buffer[0] & 0x01) << 2) | (Data_Buffer[1] >> 6));
												Receive_Package.Block[6] = ((Data_Buffer[1] & 0x38) >> 3);
												Receive_Package.Block[7] = (Data_Buffer[1] & 0x07);   //Data_Buffer[1] All Handled
												Receive_Package.Block[8] = (Data_Buffer[2] >> 5);
												Receive_Package.Block[9] = ((Data_Buffer[2] & 0x1C) >> 2);
												Receive_Package.Block[10] = (((Data_Buffer[2] & 0x03) << 1) | Data_Buffer[3] >> 7);
												Receive_Package.Block[11] = ((Data_Buffer[3] & 0x70) >> 4);
												Receive_Package.Block[12] = ((Data_Buffer[3] & 0x0e) >> 1);
												Receive_Package.Block[13] = (((Data_Buffer[3] & 0x01) << 2) | (Data_Buffer[4] >> 6));
												Receive_Package.Block[14] = ((Data_Buffer[4] & 0x38) >> 3);
												Receive_Package.Block[15] = (Data_Buffer[4] & 0x07);   //Data_Buffer[4] All Handled
												Receive_Package.Block[16] = (Data_Buffer[5] >> 5);
												Receive_Package.Block[17] = ((Data_Buffer[5] & 0x1C) >> 2);
												Receive_Package.Block[18] = (((Data_Buffer[5] & 0x03) << 1) | Data_Buffer[6] >> 7);
												Receive_Package.Block[19] = ((Data_Buffer[6] & 0x70) >> 4);
												Receive_Package.Block[20] = ((Data_Buffer[6] & 0x0e) >> 1);
												Receive_Package.Block[21] = (((Data_Buffer[6] & 0x01) << 2) | (Data_Buffer[7] >> 6));
												Receive_Package.Block[22] = ((Data_Buffer[7] & 0x38) >> 3);
												Receive_Package.Block[23] = (Data_Buffer[7] & 0x07);   //Data_Buffer[7] All Handled
												break;
										case MAP_DATA3:
												Receive_Package.Block[24] = (Data_Buffer[0] >> 5);
												Receive_Package.Block[25] = ((Data_Buffer[0] & 0x1C) >> 2);
												Receive_Package.Block[26] = (((Data_Buffer[0] & 0x03) << 1) | Data_Buffer[1] >> 7);
												Receive_Package.Block[27] = ((Data_Buffer[1] & 0x70) >> 4);
												Receive_Package.Block[28] = ((Data_Buffer[1] & 0x0e) >> 1);
												Receive_Package.Block[29] = (((Data_Buffer[1] & 0x01) << 2) | (Data_Buffer[2] >> 6));
												Receive_Package.Block[30] = ((Data_Buffer[2] & 0x38) >> 3);
												Receive_Package.Block[31] = (Data_Buffer[2] & 0x07);   //Data_Buffer[2] All Handled
												Receive_Package.Block[32] = (Data_Buffer[3] >> 5);
												Receive_Package.Block[33] = ((Data_Buffer[3] & 0x1C) >> 2);
												Receive_Package.Block[34] = (((Data_Buffer[3] & 0x03) << 1) | Data_Buffer[4] >> 7);
												Receive_Package.Block[35] = ((Data_Buffer[4] & 0x70) >> 4);
												Receive_Package.Block[36] = ((Data_Buffer[4] & 0x0e) >> 1);
												Receive_Package.Block[37] = (((Data_Buffer[4] & 0x01) << 2) | (Data_Buffer[5] >> 6));
												Receive_Package.Block[38] = ((Data_Buffer[5] & 0x38) >> 3);
												Receive_Package.Block[39] = (Data_Buffer[5] & 0x07);   //Data_Buffer[5] All Handled
												Receive_Package.Block[40] = (Data_Buffer[6] >> 5);
												Receive_Package.Block[41] = ((Data_Buffer[6] & 0x1C) >> 2);
												Receive_Package.Block[42] = (((Data_Buffer[6] & 0x03) << 1) | Data_Buffer[7] >> 7);
												Receive_Package.Block[43] = ((Data_Buffer[7] & 0x70) >> 4);
												Receive_Package.Block[44] = ((Data_Buffer[7] & 0x0e) >> 1);
												Temp_Register = ((Data_Buffer[7] & 0x01) << 2);
												break;
										case MAP_DATA4:
												Receive_Package.Block[45] = (Temp_Register | (Data_Buffer[0] >> 6));
												Receive_Package.Block[46] = ((Data_Buffer[0] & 0x38) >> 3);
												Receive_Package.Block[47] = (Data_Buffer[0] & 0x07);   //Data_Buffer[0] All Handled
												Receive_Package.Block[48] = (Data_Buffer[1] >> 5);
												Receive_Package.Block[49] = ((Data_Buffer[1] & 0x1C) >> 2);
												Receive_Package.Block[50] = (((Data_Buffer[1] & 0x03) << 1) | Data_Buffer[2] >> 7);
												Receive_Package.Block[51] = ((Data_Buffer[2] & 0x70) >> 4);
												Receive_Package.Block[52] = ((Data_Buffer[2] & 0x0e) >> 1);
												Receive_Package.Block[53] = (((Data_Buffer[2] & 0x01) << 2) | (Data_Buffer[3] >> 6));
												Receive_Package.Block[54] = ((Data_Buffer[3] & 0x38) >> 3);
												Receive_Package.Block[55] = (Data_Buffer[3] & 0x07);   //Data_Buffer[3] All Handled
												Receive_Package.Block[56] = (Data_Buffer[4] >> 5);
												Receive_Package.Block[57] = ((Data_Buffer[4] & 0x1C) >> 2);
												Receive_Package.Block[58] = (((Data_Buffer[4] & 0x03) << 1) | Data_Buffer[5] >> 7);
												Receive_Package.Block[59] = ((Data_Buffer[5] & 0x70) >> 4);
												Receive_Package.Block[60] = ((Data_Buffer[5] & 0x0e) >> 1);
												Receive_Package.Block[61] = (((Data_Buffer[5] & 0x01) << 2) | (Data_Buffer[6] >> 6));
												Receive_Package.Block[62] = ((Data_Buffer[6] & 0x38) >> 3);
												Receive_Package.Car_Blue[0] = (Data_Buffer[7] >> 4);
												Receive_Package.Car_Blue[1] = (Data_Buffer[7] & 0x0F);
												break;
										case MAP_DATA5:
												Receive_Package.Car_Red[0] = (Data_Buffer[0] >> 4);
												Receive_Package.Car_Red[1] = (Data_Buffer[0] & 0x0F);
												Receive_Package.Least_Time = Data_Buffer[1];
												Receive_Package.Score_Blue = Data_Buffer[4];
												Receive_Package.Score_Red = Data_Buffer[5];
												break;
										case CHASSIS_DATA0:
												Temp_Var = 0;
												while (1)
												{
													Receive_Package.Motor_Speed[Temp_Var/2] = ((Data_Buffer[Temp_Var + 1] << 8) | Data_Buffer[Temp_Var]);
													Temp_Var += 2;
													if (Temp_Var == 8)
														break;
												}
												break;
										case CHASSIS_DATA1:
												Temp_Var = 0;
												while (1)
												{
													Receive_Package.Motor_Speed[(Temp_Var / 2) + 4] = ((Data_Buffer[Temp_Var + 1] << 8) | Data_Buffer[Temp_Var]);
													Temp_Var += 2;
													if (Temp_Var == 8)
														break;
												}
												break;
										case CHASSIS_DATA2:
												Receive_Package.Gryo_Scope_Gryo = ((Data_Buffer[0] << 24) | (Data_Buffer[1] << 16) | (Data_Buffer[2] << 8) | (Data_Buffer[3]));
												Receive_Package.Gryo_Scope_Angle = ((Data_Buffer[4] << 24) | (Data_Buffer[5] << 16) | (Data_Buffer[6] << 8) | (Data_Buffer[7]));
												break;
										case ACK_PACKAGE:
												//Here To Do The ACK Response
												break;
                }
        }
			}
}
