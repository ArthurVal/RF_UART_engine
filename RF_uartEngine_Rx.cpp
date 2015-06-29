
//include
#include "RF_uartEngine_Rx.h"

RF_uartEngine_Rx::RF_uartEngine_Rx()
: RF_uartEngine()
{
}

/*==================================================*/
/*==                  readChar                    ==*/
/*==================================================*/

bool RF_uartEngine_Rx::readChar(unsigned char inputUART)
{
	printf("BYTE RECEIVED: %X\n",inputUART);
	if(stateMachine.status != STATUS_BLOCKED){
		switch(stateMachine.state)
		{
			////////////////////////////////////
			//            STATE INIT          //
			////////////////////////////////////
			case STATE_INIT:
				if((inputUART == 0x3A) && (stateMachine.msgRead == true) && (stateMachine.status == STATUS_WAIT)){
					MSG_clear();
					stateMachine.state = STATE_FCT_BYTE;		
					stateMachine.status = STATUS_READING;
					*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;
					stateMachine.RX_msgRdy = false;
					stateMachine.msgRead = false;
				}else{
					stateMachine.state = STATE_INIT;		
					stateMachine.RX_msgRdy = false;
				}
			break;

			////////////////////////////////////
			//          STATE_FCT_BYTE        //
			////////////////////////////////////
			case STATE_FCT_BYTE:
				stateMachine.msg.ptrMsg = 1;
				*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = stateMachine.msg.functionCode = inputUART;
				stateMachine.state = STATE_LEN_1;	
				stateMachine.RX_msgRdy = false;
			break;

			////////////////////////////////////
			//          STATE_LEN             //
			////////////////////////////////////
			case STATE_LEN_1: //MSB
				stateMachine.msg.ptrMsg = 2;
				*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;
				stateMachine.msg.length = (unsigned short)inputUART;
				stateMachine.msg.length <<= 8;			
				stateMachine.state = STATE_LEN_2;	
				stateMachine.RX_msgRdy = false;

			break;

			case STATE_LEN_2: //LSB
				stateMachine.msg.ptrMsg = 3;
				*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;
				stateMachine.msg.length = stateMachine.msg.length | ((unsigned short)inputUART);

				stateMachine.msg.sizeMsg = stateMachine.msg.length + 8; //Datas + (Start - Func - LEN - LEN - CRC - CRC - STOP - STOP)
				stateMachine.msg.currentMsg = (unsigned char*)realloc(stateMachine.msg.currentMsg , sizeof(unsigned char) * (stateMachine.msg.sizeMsg));	

				stateMachine.msg.sizeData = stateMachine.msg.length; //Datas
				stateMachine.msg.Data = (unsigned char*)realloc(stateMachine.msg.Data , sizeof(unsigned char) * (stateMachine.msg.sizeData));	

				stateMachine.RX_msgRdy = false;

				switch(stateMachine.msg.functionCode){
					case FCT_START_RF:
						stateMachine.state = STATE_CRC_1;	
					break;
							case FCT_MOVE_PHI:
						stateMachine.state = STATE_CRC_1;	
					break;

					case FCT_MOVE_THETA:
						stateMachine.state = STATE_CRC_1;
					break;
	
					case FCT_SET_PARAM:
						stateMachine.state = STATE_SET_PARAM_1;
					break;
	
					case FCT_SET_M_PARAM:
						stateMachine.state = STATE_SET_MULTI_PARAM_1;
					break;
	
					case FCT_GET_PARAM:
						stateMachine.state = STATE_GET_PARAM_1;
					break;
	
					case FCT_ANS_START_RF:
						stateMachine.state = STATE_ACK_START_RF_1;
					break;
	
					case FCT_ANS_MOVE:
						stateMachine.state = STATE_ACK_MOVE_1;
					break;
	
					case FCT_ANS_GET_PARAM:
						stateMachine.state = STATE_ANS_GET_PARAM_1;
					break;
	
					case FCT_ERR_CRC:
						stateMachine.state = STATE_CRC_1;
					break;
	
					case FCT_ERR_MSG_UNKNOW:
						stateMachine.state = STATE_CRC_1;
					break;
	
					case FCT_ERR_CARTE:
						stateMachine.state = STATE_CRC_1;
					break;
	
					default:				
						stateMachine.state = STATE_ERR;		
						stateMachine.status = STATUS_ERR_FCT_UNKNOW;
					break;
				}
		break;

			////////////////////////////////////
			//           STATE_CRC            //
			////////////////////////////////////
		case STATE_CRC_1:
			stateMachine.msg.ptrMsg++;	
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;

			stateMachine.state = STATE_CRC_2;
			stateMachine.msg.crc_msg = (unsigned short)inputUART;
			stateMachine.msg.crc_msg <<= 8; //MSB

			stateMachine.RX_msgRdy = false;
		break;

		case STATE_CRC_2:
			stateMachine.msg.ptrMsg++;	
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;

			stateMachine.msg.crc_msg = stateMachine.msg.crc_msg | ((unsigned short)inputUART); //LSB


			CRC_compute();
			if(CRC_check())
				stateMachine.state = STATE_STOP_BYTE_1;	
			else{
				stateMachine.state = STATE_ERR;
				stateMachine.status = STATUS_ERR_CRC;
			}
			stateMachine.RX_msgRdy = false;
		break;

			////////////////////////////////////
			//          STATE_STOP_BYTE       //
			////////////////////////////////////
		case STATE_STOP_BYTE_1:			
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;
			if(inputUART == 0x0D){			
				stateMachine.state = STATE_STOP_BYTE_2;
			}else{				
				stateMachine.state = STATE_ERR;		
				stateMachine.status = STATUS_ERR_STOP_BYTE;
			}
			stateMachine.RX_msgRdy = false;
		break;

		case STATE_STOP_BYTE_2:
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = inputUART;
			if(inputUART == 0x0A){			
				stateMachine.state = STATE_INIT;
				stateMachine.status = STATUS_WAIT;
				stateMachine.RX_msgRdy = true;
				stateMachine.msgRead = false;
			}else{				
				stateMachine.state = STATE_INIT;		
				stateMachine.status = STATUS_ERR_STOP_BYTE;
				stateMachine.RX_msgRdy = true;
				stateMachine.msgRead = false;
			}
		break;

			////////////////////////////////////
			//         STATE_SET_PARAM        //
			////////////////////////////////////
		case STATE_SET_PARAM_1: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.msg.nbrParam = 1;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = stateMachine.msg.ID[0] = inputUART;
			stateMachine.state = STATE_SET_PARAM_N;
			stateMachine.RX_msgRdy = false;
		break;

		case STATE_SET_PARAM_N:
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = inputUART;

			if((stateMachine.msg.ptrMsg - 3) < stateMachine.msg.length){
				stateMachine.state = STATE_SET_PARAM_N;	
			}else{			
				stateMachine.state = STATE_CRC_1;				
			}
			stateMachine.RX_msgRdy = false;		
		break;

			////////////////////////////////////
			//     STATE_SET_MULTI_PARAM      //
			////////////////////////////////////
		case STATE_SET_MULTI_PARAM_1: //NB PARAMS
			stateMachine.msg.ptrMsg = 4;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = stateMachine.msg.nbrParam = inputUART;
			stateMachine.state = STATE_SET_MULTI_PARAM_M;
			stateMachine.RX_msgRdy = false;
		break;

		case STATE_SET_MULTI_PARAM_M: //IDs
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = stateMachine.msg.ID[stateMachine.msg.ptrMsg - 5] = inputUART;

			if((stateMachine.msg.ptrMsg - 4) < stateMachine.msg.nbrParam){
				stateMachine.state = STATE_SET_MULTI_PARAM_M;	
			}else{			
				stateMachine.state = STATE_SET_MULTI_PARAM_N;				
			}

			stateMachine.RX_msgRdy = false;		
		break;

		case STATE_SET_MULTI_PARAM_N: //DATAS
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = inputUART;

			if((stateMachine.msg.ptrMsg - 3) < stateMachine.msg.length){
				stateMachine.state = STATE_SET_MULTI_PARAM_N;	
			}else{			
				stateMachine.state = STATE_CRC_1;				
			}

			stateMachine.RX_msgRdy = false;	
		break;

			////////////////////////////////////
			//         STATE_GET_PARAM        //
			////////////////////////////////////
		case STATE_GET_PARAM_1: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.msg.nbrParam = 1;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = stateMachine.msg.ID[0] = inputUART;
			stateMachine.state = STATE_CRC_1;
			stateMachine.RX_msgRdy = false;
		break;

			////////////////////////////////////
			//        STATE_ACK_START_RF      //
			////////////////////////////////////
		case STATE_ACK_START_RF_1: //ACK
			stateMachine.msg.ptrMsg = 4;
			stateMachine.msg.nbrParam = 1;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = inputUART;
			stateMachine.state = STATE_CRC_1;
			stateMachine.RX_msgRdy = false;
		break;

			////////////////////////////////////
			//          STATE_ACK_MOVE        //
			////////////////////////////////////
		case STATE_ACK_MOVE_1: //ACK
			stateMachine.msg.ptrMsg = 4;
			stateMachine.msg.nbrParam = 1;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = inputUART;
			stateMachine.state = STATE_CRC_1;
			stateMachine.RX_msgRdy = false;
		break;

			////////////////////////////////////
			//      STATE_ANS_GET_PARAM       //
			////////////////////////////////////
		case STATE_ANS_GET_PARAM_1: //ID
			stateMachine.msg.ptrMsg = 4;
			stateMachine.msg.nbrParam = 1;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = stateMachine.msg.ID[0] = inputUART;
			stateMachine.state = STATE_ANS_GET_PARAM_N;
			stateMachine.RX_msgRdy = false;
		break;

		case STATE_ANS_GET_PARAM_N: //PARAM
			stateMachine.msg.ptrMsg++;
			*(stateMachine.msg.currentMsg + stateMachine.msg.ptrMsg) = *(stateMachine.msg.Data + stateMachine.msg.ptrMsg - 4) = inputUART;

			if((stateMachine.msg.ptrMsg - 3) < stateMachine.msg.length){
				stateMachine.state = STATE_ANS_GET_PARAM_N;	
			}else{			
				stateMachine.state = STATE_CRC_1;				
			}
			stateMachine.RX_msgRdy = false;	
		break;

			////////////////////////////////////
			//            STATE_ERR           //
			////////////////////////////////////
		case STATE_ERR: //Wait for stop byte 0x0D -> 0x0A
			stateMachine.state = STATE_INIT;
			if(stateMachine.status >= 0)
				stateMachine.status = STATUS_ERR_INTERNAL;

			stateMachine.RX_msgRdy = true;	
		break;



		}//switch state machine
	}//if stateMachine.status != STATUS_BLOCKED
	return stateMachine.RX_msgRdy;
}
