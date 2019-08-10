#include "User.h"

uint8_t Process = PROCESS_FIND_SOF;

uint16_t Data_Length = 0;
uint8_t Seq_Num = 0;
uint8_t CRC8 = 0;
uint8_t CMD_ID = 0;
uint8_t Recv_Cnt = 0;
uint8_t Buffer[sizeof(Summer_Camp_Info_t)];
uint8_t CRC16 = 0;

void Referee_Decode(uint8_t data)
{
	switch(Process)
	{
		case PROCESS_FIND_SOF:
		{
			if(data == SOF){
				Process = PROCESS_DATA_LENGTH_LOW;
			}else{
				Process = PROCESS_FIND_SOF;
				Recv_Cnt = 0;
			}
		}break;
		
		case PROCESS_DATA_LENGTH_LOW:
		{
			Data_Length = 0;
			Data_Length = Data_Length | data;
			Process = PROCESS_DATA_LENGTH_HIGH;
		}break;
		
		case PROCESS_DATA_LENGTH_HIGH:
		{
			Data_Length = Data_Length | data<<8;
			if(Data_Length == sizeof(Summer_Camp_Info))
			{
				Process = PROCESS_DATA_SEQ_NUM;
			}
			else
			{
				Process = PROCESS_FIND_SOF;
			}
		}break;
		
		case PROCESS_DATA_SEQ_NUM:
		{
			Seq_Num = 0;
			Seq_Num = data;
			Process = PROCESS_CRC8;
		}break;
		
		case PROCESS_CRC8:
		{
			CRC8 = 0;
			CRC8 = data;
			Process = PROCESS_CMD_ID_LOW;
		}break;
		
		case PROCESS_CMD_ID_LOW:
		{
			CMD_ID = 0;
			CMD_ID = CMD_ID | data;
			Process = PROCESS_CMD_ID_HIGH;
		}break;
		
		case PROCESS_CMD_ID_HIGH:
		{
			CMD_ID = CMD_ID | data<<8;
			
			if(CMD_ID != 0x01)
			{
				Process = PROCESS_FIND_SOF;
				Recv_Cnt = 0;
			}
			else
			{
				Process = PROCESS_DATA;
				Recv_Cnt = 0;
			}
		}break;
		
		case PROCESS_DATA:
		{
			if((Recv_Cnt+1)<=Data_Length)
			{
				Buffer[Recv_Cnt] = data;
				Recv_Cnt++;
			}
			if((Recv_Cnt)==Data_Length)
			{
				Recv_Cnt = 0;
				Process = PROCESS_CRC16_LOW;
			}
		}break;
		
		case PROCESS_CRC16_LOW:
		{
			CRC16 = 0;
			CRC16 = CRC16 | data;
			Process = PROCESS_CRC16_HIGH;

		}break;
		
		case PROCESS_CRC16_HIGH:
		{
			CRC16 = CRC16 | data<<8;
			Process = PROCESS_FINISH;
		}break;
		
		case PROCESS_FINISH:
		{
			memcpy(&Summer_Camp_Info,Buffer,sizeof(Summer_Camp_Info));
			Process = PROCESS_FIND_SOF;
		}break;
	}
}
