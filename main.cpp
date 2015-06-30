
#include "RF_uartEngine_Tx.h"
#include "RF_uartEngine_Rx.h"
#include <stdio.h>
#include <cstring>
#include <SerialStream.h>

int main(){

	RF_uartEngine_Tx TxUART;
	RF_uartEngine_Rx RxUART;

	int i = 0;
	int choice = 0;
	char* stringInput;

	unsigned char text[500];

	bool msgReceive;
	int RxStatus = 0;

	msg_uart MSG_RX, MSG_TX;

	char* dataToSendTestValue[20];
	unsigned char dataToSendTestName[20];
	unsigned char dataToSendTestFormat[20];
	unsigned short dataToSendTestSize[20];

	dataToSendTestValue[0] = "21";
	dataToSendTestName[0] = ANGLE_PHI;
	dataToSendTestFormat[0] = ASCII;
	dataToSendTestSize[0] = 2;


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

	using namespace LibSerial ;
	//
	// Create a SerialStream instance.
	//
	SerialStream my_serial_stream ;
	//
	// Open the serial port for communication.
	//
	my_serial_stream.Open( "/dev/ttyACM0" );
	my_serial_stream.SetBaudRate(SerialStreamBuf::BAUD_57600);
	//
	// Use 8 bit wide characters.
	//
	my_serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
	//
	// Use one stop bit.
	//
	my_serial_stream.SetNumOfStopBits(1) ;


	printf("=======================\n");
	printf("SERIAL PORT INITIALIZED\n");
	printf("=======================\n");
	printf("\n");



	while(choice != 9){

		printf("======================\n");
		printf("Choose Msg to send: \n");
		printf("\t- 1: Move Angle Phi\n");
		printf("\t- 9: END\n");
		choice = 0;
		while((choice != 1) && (choice != 9)){			
			scanf("%d", &choice);
			printf("\n");
		};
		//printf("CHOICE = %d\n", choice);
	
		switch(choice){
			case 1:
				printf("Enter value of phi in degree: ");
				scanf("%s",stringInput);
				dataToSendTestValue[0] = stringInput;
  			dataToSendTestSize[0] = (short)strlen(stringInput);
 
				printf("\nSET ANGLE PHI VALUE = %s / SIZE = %d : ",dataToSendTestValue[0],dataToSendTestSize[0]);
				do{
					my_serial_stream << TxUART.sendSetParam(dataToSendTestName[0] ,dataToSendTestFormat[0] ,dataToSendTestValue[0] ,dataToSendTestSize[0]);
				}while(!TxUART.msgSent() && !TxUART.transmitError());
 				if(TxUART.transmitError()){
					printf("\n-----------------------------------\n");
					printf("---   TRANSMIT ERROR DETECTED   ---\n");
					printf("-----------------------------------\n");
					my_serial_stream.Close();
					return -1;
				}
				printf("SENT\n");
				printf("MOVE TO ANGLE: ");
				do{
					my_serial_stream << TxUART.sendMoveAngle(ANGLE_PHI);
				}while(!TxUART.msgSent() && !TxUART.transmitError());
 				if(TxUART.transmitError()){
					printf("\n-----------------------------------\n");
					printf("---   TRANSMIT ERROR DETECTED   ---\n");
					printf("-----------------------------------\n");
					my_serial_stream.Close();
					return -1;
				}
				printf("SENT\n\n");

				/*printf("Waiting for Arduino to answer ...\n");
				do{					
					my_serial_stream >> next_char;
					printf("BYTE RECEIVED: %X\n",next_char);
					msgReceive = RxUART.readChar(next_char);
				}while((!msgReceive));
				RxStatus = RxUART.getStatus();
			  MSG_RX = RxUART.getMsg();
 				if(RxStatus < 0 ){
					printf("-----------------------------------\n");
					printf("---    RECEIVE ERROR DETECTED   ---\n");
					printf("---         CODE : %d           ---\n", RxStatus);
					printf("-----------------------------------\n");
					my_serial_stream.Close();
					return -1;
				}else{
					printf("- RX Msg Received :\n");
					printf("---> Status : %d\n", RxStatus);	
					printf("---> Function Code : %X\n\n\n", MSG_RX.functionCode);	
				}*/
			break;
			default:
				printf("ENDING PROGRAM\n");
		}

	}




	my_serial_stream.Close();
	return 0;
}
