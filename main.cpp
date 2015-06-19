
#include "RF_uartEngine_Tx.h"
#include <stdio.h>

int main(){

	RF_uartEngine_Tx TxUART;


	int i = 0;
	unsigned char text[500];

	char dataToSend[200];

	dataToSend[0] = 0x31;
	dataToSend[1] = 0x32;
/*	dataToSend[2] = 0x33;
	dataToSend[3] = 0x34;
	dataToSend[4] = 0x35;
	dataToSend[5] = 0x36;
	dataToSend[6] = 0x37;
	dataToSend[7] = 0x38;
	dataToSend[8] = 0x39;
	dataToSend[9] = 0x00;*/

	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(NBR_PTS, INT_16, dataToSend,2);
		text[i] = TxUART.sendGetParam(ANGLE_PHI);
		i++;
	}while(!TxUART.msgSent());
 
  msg_uart MSG = TxUART.getMsg();

	for(int j = 0 ; j < MSG.sizeMsg ; j++)
		printf("SendStartRF | CurrentMsg : %X | %X \n", text[j], MSG.currentMsg[j]);

	return 0;
}
