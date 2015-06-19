
//include
#include "RF_uartEngine_Tx.h"

RF_uartEngine_Tx::RF_uartEngine_Tx()
: RF_uartEngine()
{
	stateMachine.state = STATE_INIT;
}


bool RF_uartEngine_Tx::msgSent()
{
	return stateMachine.TX_msgEnd;	
}

/*==================================================*/
/*==                sendStartRF                   ==*/
/*==================================================*/

char RF_uartEngine_Tx::sendStartRF()
{
	if(stateMachine.status == STATUS_WAIT){
		//Message creation
		MSG_clear();

			//Set function code
		stateMachine.msg.functionCode = FCT_START_RF;

			//Set length
		stateMachine.msg.length = 0x0000;

			//Create the msg buffer
		stateMachine.msg.sizeMsg = (stateMachine.msg.length + 8);
		stateMachine.msg.currentMsg = (unsigned char*)malloc(sizeof(unsigned char) * stateMachine.msg.sizeMsg);

			//Setup the msg
		*(stateMachine.msg.currentMsg) = 0x3A;
		*(stateMachine.msg.currentMsg + 1) = stateMachine.msg.functionCode;
		*(stateMachine.msg.currentMsg + 2) = (stateMachine.msg.length >> 8); //MSB
		*(stateMachine.msg.currentMsg + 3) = (stateMachine.msg.length & 0x00FF); //LSB
		CRC_compute();
		*(stateMachine.msg.currentMsg + 4) = (unsigned char)(stateMachine.msg.crc_compute >> 8); //MSB
		*(stateMachine.msg.currentMsg + 5) = (unsigned char)(stateMachine.msg.crc_compute & 0x00FF); //LSB
		*(stateMachine.msg.currentMsg + 6) = 0x0D;
		*(stateMachine.msg.currentMsg + 7) = 0x0A;
	}
	return writeChar();	
}

/*==================================================*/
/*==               sendMoveAngle                  ==*/
/*==================================================*/

char RF_uartEngine_Tx::sendMoveAngle(unsigned char angleName)
{
	if(stateMachine.status == STATUS_WAIT){
		//Message creation
		MSG_clear();

			//Set function code
		if(angleName == ANGLE_PHI)
			stateMachine.msg.functionCode = FCT_MOVE_PHI;
		else
			if(angleName == ANGLE_THETA)
				stateMachine.msg.functionCode = FCT_MOVE_THETA;
			else{
					//Not good angle name (send start byte = 0x00) 
				stateMachine.msg.sizeMsg = 1;
				stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * stateMachine.msg.sizeMsg);				
				*(stateMachine.msg.currentMsg) = 0x00;
				return writeChar();	
			}

			//Set length
		stateMachine.msg.length = 0x0000;

			//Create the msg buffer
		stateMachine.msg.sizeMsg = (stateMachine.msg.length + 8);
		stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * stateMachine.msg.sizeMsg);

			//Setup the msg
		*(stateMachine.msg.currentMsg) = 0x3A;
		*(stateMachine.msg.currentMsg + 1) = stateMachine.msg.functionCode;
		*(stateMachine.msg.currentMsg + 2) = (stateMachine.msg.length >> 8); //MSB
		*(stateMachine.msg.currentMsg + 3) = (stateMachine.msg.length & 0x00FF); //LSB
		CRC_compute();
		*(stateMachine.msg.currentMsg + 4) = (unsigned char)(stateMachine.msg.crc_compute >> 8); //MSB
		*(stateMachine.msg.currentMsg + 5) = (unsigned char)(stateMachine.msg.crc_compute & 0x00FF); //LSB
		*(stateMachine.msg.currentMsg + 6) = 0x0D;
		*(stateMachine.msg.currentMsg + 7) = 0x0A;
	}
	return writeChar();	
}

/*==================================================*/
/*==               sendSetParam                   ==*/
/*==================================================*/

char RF_uartEngine_Tx::sendSetParam(unsigned char paramName, unsigned char paramFormat, const char* paramValue, unsigned short paramValueSize)
{
	if(stateMachine.status == STATUS_WAIT){
		//Message creation
		MSG_clear();

		char charValue;

		short shortValue;

		int intValue;
		unsigned int maskIntValue = 0xFF000000;

		long longValue;
		unsigned long maskLongValue = 0xFF00000000000000;

		float floatValue;
		unsigned int maskFloatValue = 0xFF000000;

		double doubleValue;
		unsigned long maskDoubleValue = 0xFF00000000000000;

			//Set function code
		stateMachine.msg.functionCode = FCT_SET_PARAM;

			//Set ID & nbrParam
		stateMachine.msg.nbrParam = 1;	
		stateMachine.msg.ID[0] = paramFormat | paramName;
	
			//Set length & sizeData & Data
		switch(paramFormat){
			case ASCII:				
				stateMachine.msg.length = stateMachine.msg.sizeData = paramValueSize + 1;
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				for(int i = 0 ; i < stateMachine.msg.sizeData  ; i++){
					*(stateMachine.msg.Data + i + 1) = *(paramValue + i);
				}
			break;	
	
			case INT_8:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(char);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				charValue = (char) atoi(paramValue);
				*(stateMachine.msg.Data + 1) = charValue;
			break;	
	
			case INT_16:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(short);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				(*stateMachine.msg.Data) = stateMachine.msg.ID[0];
				shortValue = (short) atoi(paramValue);
				*(stateMachine.msg.Data + 1) = (unsigned char)((shortValue & 0xFF00) >> 8); //MSB
				*(stateMachine.msg.Data + 2) = (unsigned char)(shortValue & 0x00FF); //LSB

			break;	
	
			case INT_32:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(int);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				intValue =  atoi(paramValue);
				for(int i = 0 ; i < sizeof(intValue); ++i){
					*(stateMachine.msg.Data + i + 1) = (unsigned char)((intValue & maskIntValue) >> (24 - i*8));
					maskIntValue >>= 8;
				}
				/*
				*(stateMachine.msg.Data + 1) = (unsigned char)((intValue & 0xFF000000) >> 24); //MSB
				*(stateMachine.msg.Data + 2) = (unsigned char)((intValue & 0x00FF0000) >> 16);
				*(stateMachine.msg.Data + 3) = (unsigned char)((intValue & 0x0000FF00) >> 8); 
				*(stateMachine.msg.Data + 4) = (unsigned char) (intValue & 0x000000FF); //LSB
				*/
			break;	
	
			case INT_64:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(long);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				longValue =  atol(paramValue);
				for(int i = 0 ; i < sizeof(longValue); ++i){
					*(stateMachine.msg.Data + i + 1) = (unsigned char)((intValue & maskLongValue) >> (56 - i*8));
					maskLongValue >>= 8;
				}
			break;	
	
/*			case FLOAT:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(float);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				floatValue =  strtof(paramValue, NULL);
				for(int i = 0 ; i < sizeof(floatValue); ++i){
					*(stateMachine.msg.Data + i + 1) = (unsigned char)((floatValue & maskFloatValue) >> (24 - i*8));
					maskFloatValue >>= 8;
				}

			break;	
*/	
/*			case DOUBLE:
				stateMachine.msg.length = stateMachine.msg.sizeData = sizeof(char) + sizeof(double);
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * stateMachine.msg.sizeData);
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				doubleValue =  strtod(paramValue, NULL);
				for(int i = 0 ; i < sizeof(doubleValue); ++i){
					*(stateMachine.msg.Data + i + 1) = (unsigned char)((doubleValue & maskDoubleValue) >> (56 - i*8));
					maskDoubleValue >>= 8;
				}
			break;
*/
			default:						
				stateMachine.msg.length = stateMachine.msg.sizeData = paramValueSize + 1;
				*(stateMachine.msg.Data) = stateMachine.msg.ID[0];
				for(int i = 0 ; i < stateMachine.msg.sizeData  ; i++){
					*(stateMachine.msg.Data + i + 1) = *(paramValue + i);
				}
			break;
		}

			//Create the msg buffer
		stateMachine.msg.sizeMsg = (stateMachine.msg.length + 8);
		stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * (stateMachine.msg.sizeMsg));

			//Setup the msg
		*(stateMachine.msg.currentMsg) = 0x3A;
		*(stateMachine.msg.currentMsg + 1) = stateMachine.msg.functionCode;
		*(stateMachine.msg.currentMsg + 2) = (stateMachine.msg.length >> 8); //MSB
		*(stateMachine.msg.currentMsg + 3) = (stateMachine.msg.length & 0x00FF); //LSB
		
		for(int i = 0 ; i < (stateMachine.msg.length)  ; ++i){
			*(stateMachine.msg.currentMsg + 4 + i) = *(stateMachine.msg.Data + i);
		}		
		CRC_compute();
		*(stateMachine.msg.currentMsg + 4 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute >> 8); //MSB
		*(stateMachine.msg.currentMsg + 5 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute & 0x00FF); //LSB
		*(stateMachine.msg.currentMsg + 6 + stateMachine.msg.length) = 0x0D;
		*(stateMachine.msg.currentMsg + 7 + stateMachine.msg.length) = 0x0A;
	}
	return writeChar();	
}

/*==================================================*/
/*==            sendSetMultiParam                 ==*/
/*==================================================*/

//TODO: LA FLEMME ~_~

/*==================================================*/
/*==               sendGetParam                   ==*/
/*==================================================*/

char RF_uartEngine_Tx::sendGetParam(unsigned char paramName)
{
	if(stateMachine.status == STATUS_WAIT){
		//Message creation
		MSG_clear();

			//Set function code
		stateMachine.msg.functionCode = FCT_GET_PARAM;

			//Set ID & nbrParam
		stateMachine.msg.nbrParam = 0;	
		stateMachine.msg.ID[0] = paramName;
	
			//Set length & sizeData & Data		
		stateMachine.msg.length = stateMachine.msg.sizeData = 1;
		*(stateMachine.msg.Data) = stateMachine.msg.ID[0];

			//Create the msg buffer
		stateMachine.msg.sizeMsg = (stateMachine.msg.length + 8);
		stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * (stateMachine.msg.sizeMsg));

			//Setup the msg
		*(stateMachine.msg.currentMsg) = 0x3A;
		*(stateMachine.msg.currentMsg + 1) = stateMachine.msg.functionCode;
		*(stateMachine.msg.currentMsg + 2) = (stateMachine.msg.length >> 8); //MSB
		*(stateMachine.msg.currentMsg + 3) = (stateMachine.msg.length & 0x00FF); //LSB
		
		for(int i = 0 ; i < (stateMachine.msg.length)  ; ++i){
			*(stateMachine.msg.currentMsg + 4 + i) = *(stateMachine.msg.Data + i);
		}		
		CRC_compute();
		*(stateMachine.msg.currentMsg + 4 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute >> 8); //MSB
		*(stateMachine.msg.currentMsg + 5 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute & 0x00FF); //LSB
		*(stateMachine.msg.currentMsg + 6 + stateMachine.msg.length) = 0x0D;
		*(stateMachine.msg.currentMsg + 7 + stateMachine.msg.length) = 0x0A;
	}
	return writeChar();	
}

/*==================================================*/
/*==            sendAnswerStartRF                 ==*/
/*==================================================*/

char RF_uartEngine_Tx::sendAnswerStartRF(bool OK)
{
	if(stateMachine.status == STATUS_WAIT){
		//Message creation
		MSG_clear();

			//Set function code
		stateMachine.msg.functionCode = FCT_ANS_START_RF;
	
			//Set length & sizeData & Data		
		stateMachine.msg.length = stateMachine.msg.sizeData = 1;
		*(stateMachine.msg.Data) = (char)OK;

			//Create the msg buffer
		stateMachine.msg.sizeMsg = (stateMachine.msg.length + 8);
		stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * (stateMachine.msg.sizeMsg));

			//Setup the msg
		*(stateMachine.msg.currentMsg) = 0x3A;
		*(stateMachine.msg.currentMsg + 1) = stateMachine.msg.functionCode;
		*(stateMachine.msg.currentMsg + 2) = (stateMachine.msg.length >> 8); //MSB
		*(stateMachine.msg.currentMsg + 3) = (stateMachine.msg.length & 0x00FF); //LSB
		
		for(int i = 0 ; i < (stateMachine.msg.length)  ; ++i){
			*(stateMachine.msg.currentMsg + 4 + i) = *(stateMachine.msg.Data + i);
		}		
		CRC_compute();
		*(stateMachine.msg.currentMsg + 4 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute >> 8); //MSB
		*(stateMachine.msg.currentMsg + 5 + stateMachine.msg.length) = (unsigned char)(stateMachine.msg.crc_compute & 0x00FF); //LSB
		*(stateMachine.msg.currentMsg + 6 + stateMachine.msg.length) = 0x0D;
		*(stateMachine.msg.currentMsg + 7 + stateMachine.msg.length) = 0x0A;
	}
	return writeChar();	
}




/*==================================================*/
/*==                 writeChar                    ==*/
/*==================================================*/

char RF_uartEngine_Tx::writeChar()
{
	switch(stateMachine.state)
	{
			////////////////////////////////////
			//            STATE INIT          //
			////////////////////////////////////
		case STATE_INIT:

				/*-DEBUG PURPOSE-*/
		printf("------------------------------------\n");
		printf("DEBUG:\n");
		printf("Function code = %X\n", stateMachine.msg.functionCode);
		printf("Length =        %X\n", stateMachine.msg.length);
		//printf("ID =            %X\n", stateMachine.msg.ID[0]);
		printf("CRC =           %X\n", stateMachine.msg.crc_compute);

		printf("MSG:\n");
		if(stateMachine.msg.currentMsg != NULL)
			for(int i = 0; i<stateMachine.msg.sizeMsg ; ++i){
				printf("                %X\n", *(stateMachine.msg.currentMsg + i));				
			}
		else
			printf("No MSG\n");

			printf("Data:\n");
		if(stateMachine.msg.Data != NULL)
			for(int i = 0; i<stateMachine.msg.sizeData ; ++i){
				printf("                %X\n", *(stateMachine.msg.Data + i));				
			}
		else
			printf("No Data\n");
		printf("------------------------------------\n");



			stateMachine.msg.ptrMsg = 0;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x3A){
				stateMachine.state = STATE_FCT_BYTE;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ERR;		
				stateMachine.status = STATUS_ERR_START_BYTE;
				stateMachine.TX_msgEnd = true;			
				return 0x00;
			}
		break;


			////////////////////////////////////
			//          STATE_FCT_BYTE        //
			////////////////////////////////////
		case STATE_FCT_BYTE:
			stateMachine.msg.ptrMsg = 1;
			switch(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg)){
				case FCT_START_RF:
					stateMachine.state = STATE_START_RF_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_MOVE_PHI:
					stateMachine.state = STATE_MOVE_PHI_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_MOVE_THETA:
					stateMachine.state = STATE_MOVE_THETA_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_SET_PARAM:
					stateMachine.state = STATE_SET_PARAM_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_SET_M_PARAM:
					stateMachine.state = STATE_SET_MULTI_PARAM_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_GET_PARAM:
					stateMachine.state = STATE_GET_PARAM_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ANS_START_RF:
					stateMachine.state = STATE_ACK_START_RF_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ANS_MOVE:
					stateMachine.state = STATE_ACK_MOVE_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ANS_GET_PARAM:
					stateMachine.state = STATE_ANS_GET_PARAM_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ERR_CRC:
					stateMachine.state = STATE_ERR_CRC_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ERR_MSG_UNKNOW:
					stateMachine.state = STATE_ERR_FCT_UNKNOW_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				case FCT_ERR_CARTE:
					stateMachine.state = STATE_ERR_CARTE_1;	
					return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
				break;

				default:				
					stateMachine.state = STATE_ERR;		
					stateMachine.status = STATUS_ERR_FCT_UNKNOW;
					stateMachine.TX_msgEnd = true;	
					return 0xFF;
				break;
			}
		break;


			////////////////////////////////////
			//           STATE_CRC            //
			////////////////////////////////////
		case STATE_CRC_1:
			stateMachine.msg.ptrMsg++;
			stateMachine.state = STATE_CRC_2;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_CRC_2:
			stateMachine.msg.ptrMsg++;
			stateMachine.state = STATE_STOP_BYTE_1;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;


			////////////////////////////////////
			//          STATE_STOP_BYTE       //
			////////////////////////////////////
		case STATE_STOP_BYTE_1:
			stateMachine.msg.ptrMsg++;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x0D){
				stateMachine.state = STATE_STOP_BYTE_2;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_STOP_BYTE_2;		
				stateMachine.status = STATUS_ERR_STOP_BYTE;
				//stateMachine.TX_msgEnd = true;			
				return 0x0D;
			}
		break;

		case STATE_STOP_BYTE_2:
			stateMachine.msg.ptrMsg++;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x0A){
				if(stateMachine.status == STATUS_WRITING){
					stateMachine.state = STATE_INIT;		
					stateMachine.status = STATUS_WAIT;
				}else{
					stateMachine.state = STATE_ERR;					
				}
				stateMachine.TX_msgEnd = true;				
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ERR;		
				stateMachine.status = STATUS_ERR_STOP_BYTE;
				stateMachine.TX_msgEnd = true;			
				return 0x0A;
			}
		break;


			////////////////////////////////////
			//          STATE_START_RF        //
			////////////////////////////////////
		case STATE_START_RF_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_START_RF_2;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_START_RF_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_START_RF_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;



			////////////////////////////////////
			//          STATE_MOVE_PHI        //
			////////////////////////////////////
		case STATE_MOVE_PHI_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_MOVE_PHI_2;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_MOVE_PHI_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_MOVE_PHI_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

			////////////////////////////////////
			//        STATE_MOVE_THETA        //
			////////////////////////////////////
		case STATE_MOVE_THETA_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_MOVE_THETA_2;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_MOVE_THETA_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_MOVE_THETA_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;		
				stateMachine.status = STATUS_WRITING;		
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;


			////////////////////////////////////
			//         STATE_SET_PARAM        //
			////////////////////////////////////
		case STATE_SET_PARAM_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			stateMachine.state = STATE_SET_PARAM_2;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_PARAM_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			stateMachine.state = STATE_SET_PARAM_3;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_PARAM_3: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.state = STATE_SET_PARAM_N;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_PARAM_N:
			stateMachine.msg.ptrMsg++;
			if(stateMachine.msg.ptrMsg < stateMachine.msg.length + 3)
				stateMachine.state = STATE_SET_PARAM_N;
			else
				stateMachine.state = STATE_CRC_1;

			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;


			////////////////////////////////////
			//     STATE_SET_MULTI_PARAM      //
			////////////////////////////////////
		case STATE_SET_MULTI_PARAM_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			stateMachine.state = STATE_SET_MULTI_PARAM_2;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_MULTI_PARAM_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			stateMachine.state = STATE_SET_MULTI_PARAM_3;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_MULTI_PARAM_3: //NB PARAM
			stateMachine.msg.ptrMsg = 4;
			stateMachine.state = STATE_SET_MULTI_PARAM_M;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_SET_MULTI_PARAM_M: //IDs
			stateMachine.msg.ptrMsg++;
			if(stateMachine.msg.ptrMsg < stateMachine.msg.nbrParam + 4)
				stateMachine.state = STATE_SET_MULTI_PARAM_M;
			else
				stateMachine.state = STATE_SET_MULTI_PARAM_N;

		break;

		case STATE_SET_MULTI_PARAM_N:
			stateMachine.msg.ptrMsg++;
			if(stateMachine.msg.ptrMsg < stateMachine.msg.length + 3)
				stateMachine.state = STATE_SET_MULTI_PARAM_N;
			else
				stateMachine.state = STATE_CRC_1;
		break;


			////////////////////////////////////
			//         STATE_GET_PARAM        //
			////////////////////////////////////
		case STATE_GET_PARAM_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_GET_PARAM_2;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_GET_PARAM_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_GET_PARAM_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x01){
				stateMachine.state = STATE_GET_PARAM_3;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_GET_PARAM_3;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x01;
			}
		break;

		case STATE_GET_PARAM_3: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.state = STATE_CRC_1;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;


			////////////////////////////////////
			//        STATE_ACK_START_RF      //
			////////////////////////////////////
		case STATE_ACK_START_RF_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_ACK_START_RF_2;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ACK_START_RF_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_ACK_START_RF_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x01){
				stateMachine.state = STATE_ACK_START_RF_3;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ACK_START_RF_3;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x01;
			}
		break;

		case STATE_ACK_START_RF_3: //ACK
			stateMachine.msg.ptrMsg = 4;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) <= 0x01){
				stateMachine.state = STATE_CRC_1;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;
				stateMachine.status = STATUS_ERR_ACK;
				return 0x01; //return not OK
			}
		break;


			////////////////////////////////////
			//          STATE_ACK_MOVE        //
			////////////////////////////////////
		case STATE_ACK_MOVE_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_ACK_MOVE_2;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ACK_MOVE_2;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_ACK_MOVE_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x01){
				stateMachine.state = STATE_ACK_MOVE_3;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ACK_MOVE_3;
				stateMachine.status = STATUS_ERR_LEN;
				return 0x01;
			}
		break;

		case STATE_ACK_MOVE_3: //ACK
			stateMachine.msg.ptrMsg = 4;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) <= 0x01){
				stateMachine.state = STATE_CRC_1;
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;
				stateMachine.status = STATUS_ERR_ACK;
				return 0x01; //return not OK
			}
		break;


			////////////////////////////////////
			//      STATE_ANS_GET_PARAM       //
			////////////////////////////////////
		case STATE_ANS_GET_PARAM_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			stateMachine.state = STATE_ANS_GET_PARAM_2;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_ANS_GET_PARAM_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			stateMachine.state = STATE_ANS_GET_PARAM_3;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_ANS_GET_PARAM_3: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.state = STATE_ANS_GET_PARAM_N;		
			return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
		break;

		case STATE_ANS_GET_PARAM_N: //PARAM
			stateMachine.msg.ptrMsg++;
			if(stateMachine.msg.ptrMsg < stateMachine.msg.length + 3)
				stateMachine.state = STATE_ANS_GET_PARAM_N;
			else
				stateMachine.state = STATE_CRC_1;
		break;


			////////////////////////////////////
			//         STATE_ERR_CRC          //
			////////////////////////////////////
		case STATE_ERR_CRC_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_ERR_CRC_2;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ERR_CRC_2;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_ERR_CRC_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;


			////////////////////////////////////
			//      STATE_ERR_FCT_UNKNOW      //
			////////////////////////////////////
		case STATE_ERR_FCT_UNKNOW_1: //LEN
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_ERR_FCT_UNKNOW_2;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ERR_FCT_UNKNOW_2;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_ERR_FCT_UNKNOW_2: //LEN
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;


			////////////////////////////////////
			//        STATE_ERR_CARTE         //
			////////////////////////////////////
		case STATE_ERR_CARTE_1:
			stateMachine.msg.ptrMsg = 2;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_ERR_CARTE_2;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_ERR_CARTE_2;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

		case STATE_ERR_CARTE_2:
			stateMachine.msg.ptrMsg = 3;
			if(*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) == 0x00){
				stateMachine.state = STATE_CRC_1;	
				return *(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg);
			}else{				
				stateMachine.state = STATE_CRC_1;		
				stateMachine.status = STATUS_ERR_LEN;
				return 0x00;
			}
		break;

			////////////////////////////////////
			//           STATE_ERR            //
			////////////////////////////////////
		case STATE_ERR:
			if(stateMachine.status == STATUS_WAIT)
				stateMachine.state = STATE_INIT;	

			return 0xFF;
				
		break;


			////////////////////////////////////
			//             DEFAULT            //
			////////////////////////////////////
		default:
			stateMachine.state = STATE_ERR;	
			stateMachine.status = STATUS_ERR_STATE;	
			return 0xFF;
	}
}



