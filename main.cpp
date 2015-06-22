
#include "RF_uartEngine_Tx.h"
#include "RF_uartEngine_Rx.h"
#include <stdio.h>

int main(){

	RF_uartEngine_Tx TxUART;
	RF_uartEngine_Rx RxUART;


	int i = 0;
	unsigned char text[500];
	bool msgReceive;
	int RxStatus = 0;
	msg_uart MSG_RX, MSG_TX;
	char dataToSend[200];

	dataToSend[0] = 0x31; //1
	dataToSend[1] = 0x32; //2
	dataToSend[2] = 0x33; //3
	dataToSend[3] = 0x34; //4
	dataToSend[4] = 0x35; //5
	dataToSend[5] = 0x36; //6
	dataToSend[6] = 0x37; //7
	dataToSend[7] = 0x38; //8
	dataToSend[8] = 0x39; //9
	dataToSend[9] = 0x00; //\0
/**/
	char* dataToSendTestValue[20];
	unsigned char dataToSendTestName[20];
	unsigned char dataToSendTestFormat[20];
	unsigned short dataToSendTestSize[20];

	dataToSendTestValue[0] = "123456789";
	dataToSendTestName[0] = NBR_PTS;
	dataToSendTestFormat[0] = ASCII;
	dataToSendTestSize[0] = 9;


	dataToSendTestValue[1] = "65535";
	dataToSendTestName[1] = NBR_PTS;
	dataToSendTestFormat[1] = INT_64;
	dataToSendTestSize[1] = 5;

	dataToSendTestValue[2] = "651981116519116516198";
	dataToSendTestName[2] = NBR_PTS;
	dataToSendTestFormat[2] = ASCII;
	dataToSendTestSize[2] = 21;





			//TX LOOP
	do{
		text[i] = TxUART.sendStartRF();
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;

	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n", RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 1 send: sendStartRF()\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");



/*------------------------------------------------------------------------------------*/

	i = 0;
			//TX LOOP
	do{
		//text[i] = TxUART.sendStartRF();
		text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(NBR_PTS, INT_16, dataToSend,2);
		//text[i] = TxUART.sendGetParam(ANGLE_PHI);
		//text[i] = TxUART.sendAnswerStartRF(false);
		//text[i] = TxUART.sendAnswerMoveAngle(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 2 send: sendMoveAngle(ANGLE_PHI)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");

/*------------------------------------------------------------------------------------*/


	i = 0;
			//TX LOOP
	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		text[i] = TxUART.sendSetParam(ANGLE_PHI, ASCII, dataToSend,10);
		//text[i] = TxUART.sendGetParam(ANGLE_PHI);
		//text[i] = TxUART.sendAnswerStartRF(false);
		//text[i] = TxUART.sendAnswerMoveAngle(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 3 send: sendSetParam(ANGLE_PHI, ASCII, dataToSend,10)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");











/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(NBR_PTS, INT_16, dataToSend,2);
		text[i] = TxUART.sendGetParam(ANGLE_PHI);
		//text[i] = TxUART.sendAnswerStartRF(false);
		//text[i] = TxUART.sendAnswerMoveAngle(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}


	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n", RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 4 send: sendGetParam(ANGLE_PHI)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");



/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		//text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(NBR_PTS, INT_16, dataToSend,2);
		//text[i] = TxUART.sendGetParam(ANGLE_PHI);
		text[i] = TxUART.sendAnswerStartRF(false);
		//text[i] = TxUART.sendAnswerMoveAngle(true);
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n", RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 5 send: sendAnswerStartRF(false)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");






/*------------------------------------------------------------------------------------*/

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

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n", RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 6 send: sendAnswerMoveAngle(true)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");




/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		text[i] = TxUART.sendErrCRC();
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 7 send: sendErrCRC()\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");


/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		text[i] = TxUART.sendErrUnknowMsg();
		i++;
	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 8 send: sendErrUnknowMsg()\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");



/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		text[i] = TxUART.sendErrCarte();
		i++;

	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 9 send: sendErrCarte()\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");

/*------------------------------------------------------------------------------------*/

	i = 0;
	do{
		text[i] = TxUART.sendSetMultiParam(3,
		                                   dataToSendTestName,
		                                   dataToSendTestFormat,
		                                   dataToSendTestValue,
		                                   dataToSendTestSize);
		printf("TX: %X\n",text[i]);
		i++;

	}while(!TxUART.msgSent() && !TxUART.transmitError());
 
	if(TxUART.transmitError()){
		printf("-----------------------------------\n");
		printf("---   TRANSMIT ERROR DETECTED   ---\n");
		printf("-----------------------------------\n");
		return -1;
	}

	i = 0;
			//RX LOOP
	do{
		msgReceive = RxUART.readChar(text[i]);
		printf("RX: %X\n",text[i]);
		i++;
		if(i == 100)
			msgReceive = true;
	}while(!msgReceive);
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n",RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}



  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("-----------------------------------\n");
	printf("Message 10 send: sendSetMultiParam(\n3,\n NBR_PTS / NBR_PTS / NBR_PTS,\n ASCII / INT_64 / ASCII,\n 123456789 / 65535 / 651981116519116516198,\n 9 / 5 / 21)\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");
	printf("Nb Param => TX: %X || RX: %X\n", MSG_TX.nbrParam, MSG_RX.nbrParam);
	printf("ID: ===============\n");
	for(int j = 0 ; j < MSG_TX.nbrParam ; j++)
		printf("            TX: %X || RX: %X\n", MSG_TX.ID[j], MSG_RX.ID[j]);
	printf("ID: ===============\n");

	return 0;
}
