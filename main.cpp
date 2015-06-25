
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
	dataToSendTestFormat[1] = ASCII;
	dataToSendTestSize[1] = 5;

	dataToSendTestValue[2] = "651981116519116516198";
	dataToSendTestName[2] = ANGLE_PHI;
	dataToSendTestFormat[2] = ASCII;
	dataToSendTestSize[2] = 21;

	dataToSendTestValue[3] = "98564468";
	dataToSendTestName[3] = ANGLE_THETA;
	dataToSendTestFormat[3] = INT_32;
	dataToSendTestSize[3] = 8;

	dataToSendTestValue[4] = "651981116519116516198";
	dataToSendTestName[4] = INTENSITY;
	dataToSendTestFormat[4] = ASCII;
	dataToSendTestSize[4] = 21;

			//TX LOOP
	do{
		text[i] = TxUART.sendStartRF();
		//text[i] = TxUART.sendMoveAngle(ANGLE_PHI);
		//text[i] = TxUART.sendSetParam(dataToSendTestName[0] ,dataToSendTestFormat[0] ,dataToSendTestValue[0] ,dataToSendTestSize[0]);
		text[i] = TxUART.sendSetMultiParam(5, dataToSendTestName ,dataToSendTestFormat ,dataToSendTestValue ,dataToSendTestSize);
		//text[i] = TxUART.sendGetParam(dataToSendTestName[4]);
		//text[i] = TxUART.sendAnswerStartRF(true);
		//text[i] = TxUART.sendAnswerMoveAngle(false);
		//text[i] = TxUART.sendAnswerGetParam(dataToSendTestName[4] ,dataToSendTestFormat[4] ,dataToSendTestValue[4] ,dataToSendTestSize[4]);
		//text[i] = TxUART.sendErrCRC();
		//text[i] = TxUART.sendErrUnknowMsg();
		//text[i] = TxUART.sendErrCarte();

		//printf("BYTE TRANSMIT: %X\n", text[i]);

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
		//Insert Error
	//text[0] = 0x3A;
	//text[1] = 0x3B;
	//text[2] = 0x00;
	//text[3] = 0x04;
	//text[4] = 0x3A;
	//text[5] = 0x3B;
	//text[6] = 0x00;
	//text[7] = 0x04;

	do{
		//printf("BYTE RECEIVED: %X\n",text[i]);
		msgReceive = RxUART.readChar(text[i]);
		i++;
	}while((!msgReceive) && (i<20));
	RxStatus = RxUART.getStatus();
 	if(RxStatus < 0 ){
		printf("-----------------------------------\n");
		printf("---    RECEIVE ERROR DETECTED   ---\n");
		printf("---         CODE : %d           ---\n", RxStatus);
		printf("-----------------------------------\n");
		return -1;
	}else{
		printf("-----------------------------------\n");
		printf("---      RX Status : %d          ---\n", RxStatus);	
		printf("-----------------------------------\n");	
	}

  MSG_RX = RxUART.getMsg();
  MSG_TX = TxUART.getMsg();

	printf("===================================\n");
	printf("Message:\n");
	printf("-----------------------------------\n");
	for(int j = 0 ; j < MSG_RX.sizeMsg ; j++)
		printf("TX_MSG | text[] | RX_MSG : %X | %X | %X\n", MSG_TX.currentMsg[j], text[j], MSG_RX.currentMsg[j]);
	printf("-----------------------------------\n");
	printf("Details:\n");
	printf(" -> HEADER:\n");
	printf("Function Code => TX: %X |-| RX: %X\n", MSG_TX.functionCode, MSG_RX.functionCode);
	printf("Length        => TX: %X |-| RX: %X\n", MSG_TX.length, MSG_RX.length);
	printf(" -> DATA:\n");

	printf("Nb Param      => TX: %X |-| RX: %X\n", MSG_TX.nbrParam, MSG_RX.nbrParam);
	if((MSG_TX.nbrParam > 0) && (MSG_RX.nbrParam > 0)){
		printf("IDs:\n");
		for(int j = 0 ; (j < MSG_RX.nbrParam) && (j < MSG_TX.nbrParam) ; j++)
			printf("              -- TX: %X || RX: %X\n", MSG_TX.ID[j], MSG_RX.ID[j]);
	}

	if((MSG_TX.sizeData > 0) && (MSG_RX.sizeData > 0)){
		printf("Datas:\n");
		for(int j = 0 ; (j < MSG_TX.sizeData) && (j < MSG_RX.sizeData) ; j++)
			printf("              -- TX: %X || RX: %X\n", MSG_TX.Data[j], MSG_RX.Data[j]);
	}
	printf(" -> CRC:\n");
	printf("CRC (MSG)     => TX: %X |-| RX: %X\n", MSG_TX.crc_msg, MSG_RX.crc_msg);
	printf("CRC (CPT)     => TX: %X |-| RX: %X\n", MSG_TX.crc_compute, MSG_RX.crc_compute);
	printf("===================================\n");

	return 0;
}
