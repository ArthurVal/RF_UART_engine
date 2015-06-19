/*==========================================*/	
/*            State definition              */
/*==========================================*/	

#define STATE_INIT 0		//START BYTE (0x3A)

#define STATE_FCT_BYTE 2		//FCT CODE

#define STATE_STOP_BYTE_1 3		//CR (0x0D)
#define STATE_STOP_BYTE_2 4		//LN (0x0A)

#define STATE_START_RF_1 5		//LENGTH
#define STATE_START_RF_2 6		//LENGTH

#define STATE_MOVE_PHI_1 7		//LENGTH
#define STATE_MOVE_PHI_2 8		//LENGTH

#define STATE_MOVE_THETA_1 9		//LENGTH
#define STATE_MOVE_THETA_2 10		//LENGTH

#define STATE_SET_PARAM_1 11		//LENGTH
#define STATE_SET_PARAM_2 12		//LENGTH
#define STATE_SET_PARAM_3 13		//ID
#define STATE_SET_PARAM_N 14		//PARAM (SIZE = LEN)

#define STATE_SET_MULTI_PARAM_1 15		//LENGTH
#define STATE_SET_MULTI_PARAM_2 16		//LENGTH
#define STATE_SET_MULTI_PARAM_3 17		//Nbr
#define STATE_SET_MULTI_PARAM_M 18		//ID
#define STATE_SET_MULTI_PARAM_N 19		//PARAM

#define STATE_GET_PARAM_1 20		//LENGTH
#define STATE_GET_PARAM_2 21		//LENGTH
#define STATE_GET_PARAM_3 22		//ID

#define STATE_ACK_START_RF_1 23		//LENGTH
#define STATE_ACK_START_RF_2 24		//LENGTH
#define STATE_ACK_START_RF_3 25		//VALUE (0x00 = OK or 0x01 = FAIL)

#define STATE_ACK_MOVE_1 26		//LENGTH
#define STATE_ACK_MOVE_2 27		//LENGTH
#define STATE_ACK_MOVE_3 28		//VALUE (0x00 = OK or 0x01 = FAIL)

#define STATE_ANS_GET_PARAM_1 29		//LENGTH
#define STATE_ANS_GET_PARAM_2 30		//LENGTH
#define STATE_ANS_GET_PARAM_3 31		//ID
#define STATE_ANS_GET_PARAM_N 33		//PARAM

#define STATE_ERR_CRC_1 34		//LENGTH
#define STATE_ERR_CRC_2 35		//LENGTH

#define STATE_ERR_FCT_UNKNOW_1 36		//LENGTH
#define STATE_ERR_FCT_UNKNOW_2 37		//LENGTH

#define STATE_ERR_CARTE_1 38		//LENGTH
#define STATE_ERR_CARTE_2 39		//LENGTH

#define STATE_CRC_1 40
#define STATE_CRC_2 41

		//Error states
#define STATE_ERR 100


/*==========================================*/	
/*        Function Code definition          */
/*==========================================*/	

#define FCT_START_RF 0
#define FCT_MOVE_PHI 1
#define FCT_MOVE_THETA 2
#define FCT_SET_PARAM  3
#define FCT_SET_M_PARAM  4

#define FCT_GET_PARAM 16

#define FCT_ANS_START_RF 32
#define FCT_ANS_MOVE 33
#define FCT_ANS_GET_PARAM 34

#define FCT_ERR_CRC 240
#define FCT_ERR_MSG_UNKNOW 241
#define FCT_ERR_CARTE 242

/*==========================================*/	
/*             ID definition                */
/*==========================================*/

#define ID_THETA_MAX 0x00
#define ID_THETA_MIN 0x01
#define ID_PHI_MAX 0x02
#define ID_PHI_MIN 0x03
#define ID_ACQ_TIME 0x04
#define ID_NBR_PTS 0x05
#define ID_ANGLE_PHI 0x06
#define ID_ANGLE_THETA 0x07
#define ID_INTENSITY 0x08

#define ID_ASCII 0x00
#define ID_INT_8 0x10
#define ID_INT_16 0x20
#define ID_INT_32 0x30
#define ID_INT_64 0x40
#define ID_FLOAT 0x50
#define ID_DOUBLE 0x60

/*==========================================*/	
/*           STATUS definition              */
/*==========================================*/

#define STATUS_WAIT 0
#define STATUS_WRITING 1
#define STATUS_ERR_START_BYTE 2
#define STATUS_ERR_STOP_BYTE 3
#define STATUS_ERR_FCT_UNKNOW 4
#define STATUS_ERR_LEN 5
#define STATUS_ERR_ACK 6
#define STATUS_ERR_STATE 7

/*==========================================*/	
/*    angleName & paramName definition      */
/*==========================================*/

#define THETA_MAX 0x00
#define THETA_MIN 0x01
#define PHI_MAX 0x02
#define PHI_MIN 0x03
#define ACQ_TIME 0x04
#define NBR_PTS 0x05
#define ANGLE_PHI 0x06
#define ANGLE_THETA 0x07
#define INTENSITY 0x08

/*==========================================*/	
/*         paramFormat definition           */
/*==========================================*/

#define ASCII 0x00
#define INT_8 0x10
#define INT_16 0x20
#define INT_32 0x30
#define INT_64 0x40
#define FLOAT 0x50
#define DOUBLE 0x60

/*==========================================*/	
/*           Struct definition              */
/*==========================================*/	

#include <stdint.h>

typedef struct {

	unsigned int ptrMsg;
	unsigned int sizeMsg;
	unsigned char* currentMsg; 

	unsigned char functionCode;
	unsigned short length;

	unsigned char nbrParam;
	unsigned char ID[256];

		//Everything after Length in the msg
	unsigned int sizeData; 
	unsigned char* Data;

	unsigned short crc_msg;
	unsigned short crc_compute;

} msg_uart; 

typedef struct {
	unsigned int state;	

	bool RX_msgRdy;
	bool TX_msgEnd;

	msg_uart msg;

	int status; 
		
} state_machine; 



















