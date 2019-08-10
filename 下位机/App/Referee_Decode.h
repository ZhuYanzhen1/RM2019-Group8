#ifndef __REFEREE_DECODE_H__
#define __REFEREE_DECODE_H__

#define PROCESS_FIND_SOF 							0
#define PROCESS_DATA_LENGTH_LOW 			1
#define PROCESS_DATA_LENGTH_HIGH 			2
#define PROCESS_DATA_SEQ_NUM					3
#define PROCESS_CRC8		 							4

#define PROCESS_CMD_ID_LOW		 				5
#define PROCESS_CMD_ID_HIGH		 				6
#define PROCESS_DATA		 							7
#define PROCESS_CRC16_LOW							8
#define PROCESS_CRC16_HIGH						9
#define PROCESS_FINISH								10

#define SOF 													0xA5

void Referee_Decode(unsigned char data);


#endif //	#ifndef __REFEREE_DECODE_H__
