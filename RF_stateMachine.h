/*==========================================*/	
/*            State definition              */
/*==========================================*/	

#define STATE_INIT 0		//START BYTE (0x3A)

#define STATE_FCT_BYTE 2		//FCT CODE

#define STATE_STOP_BYTE_1 3		//LENGTH
#define STATE_STOP_BYTE_2 4		//LENGTH

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
#define STATE_ANS_GET_PARAM_3 31		//Nbr
#define STATE_ANS_GET_PARAM_M 32		//ID
#define STATE_ANS_GET_PARAM_N 33		//PARAM

#define STATE_ERR_CRC_1 34		//LENGTH
#define STATE_ERR_CRC_2 35		//LENGTH

#define STATE_ERR_FCT_UNKNOW_1 36		//LENGTH
#define STATE_ERR_FCT_UNKNOW_2 37		//LENGTH

#define STATE_ERR_CARTE_1 38		//LENGTH
#define STATE_ERR_CARTE_2 39		//LENGTH

		//Error states
#define STATE_ERR 100


/*==========================================*/	
/*        Function Code definition          */
/*==========================================*/	

#define FCT_START_RF 0x00
#define FCT_MOVE_PHI 0x01
#define FCT_MOVE_THETA 0x02
#define FCT_SET_PARAM 0x03
#define FCT_SET_M_PARAM 0x04

#define FCT_GET_PARAM 0x10

#define FCT_ANS_START_RF 0x20
#define FCT_ANS_MOVE 0x21
#define FCT_ANS_GET_PARAM 0x22

#define FCT_ERR_CRC 0xF0
#define FCT_ERR_MSG_UNKNOW 0xF1
#define FCT_ERR_CARTE 0xF2

/*==========================================*/	
/*           Struct definition              */
/*==========================================*/	


typedef struct {
	unsigned int state;	

	bool RX_msgRdy;
	bool TX_msgSend;

	char currentChar;
	char currentMsg[];

	char functionCode;
	uint16_t length;

  char ID[];
  char Data[];	

	uint16_t crc_msg;
	uint16_t crc_compute;

	int status; 
		
} state_machine; 



















