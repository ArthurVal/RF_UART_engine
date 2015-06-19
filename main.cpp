
#include "RF_uartEngine_Tx.h"
#include <stdio.h>

int main(){

	RF_uartEngine_Tx TxUART;


	int i = 0;
	unsigned char text[500];
	msg_uart MSG;
	char dataToSend[200];

	dataToSend[0] = 0x31; //1
	dataToSend[1] = 0x32; //2
	dataToSend[2] = 0x33; //3
/*	dataToSend[3] = 0x34; //4
	dataToSend[4] = 0x35; //5
	dataToSend[5] = 0x36; //6
	dataToSend[6] = 0x37; //7
	dataToSend[7] = 0x38; //8
	dataToSend[8] = 0x39; //9
	dataToSend[9] = 0x00; //\0
*/

	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		text[i] = TxUART.sendSetParam(PHI_MAX, INT_16, dataToSend,3); //123
		//text[i] = TxUART.sendGetParam(ANGLE_PHI);
		//text[i] = TxUART.sendAnswerStartRF(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

  MSG = TxUART.getMsg();
	printf("-----------------------------------\n");
	printf("Message 1 send:\n");
	for(int j = 0 ; j < MSG.sizeMsg ; j++)
		printf("SendStartRF | CurrentMsg : %X | %X \n", text[j], MSG.currentMsg[j]);
	printf("-----------------------------------\n");

	i = 0;

	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(NBR_PTS, INT_16, dataToSend,2);
		//text[i] = TxUART.sendGetParam(ANGLE_PHI);
		//text[i] = TxUART.sendAnswerStartRF(false);
		text[i] = TxUART.sendAnswerMoveAngle(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

  MSG = TxUART.getMsg();
	printf("-----------------------------------\n");
	printf("Message 2 send:\n");
	for(int j = 0 ; j < MSG.sizeMsg ; j++)
		printf("SendStartRF | CurrentMsg : %X | %X \n", text[j], MSG.currentMsg[j]);

	printf("-----------------------------------\n");
	return 0;
}
