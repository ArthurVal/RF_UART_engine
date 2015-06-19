#ifndef RF_UARTENGINE_TX_H 
#define RF_UARTENGINE_TX_H 

#include "RF_uartEngine.h"
#include <stdio.h>
#include <stdlib.h>

class RF_uartEngine_Tx : public RF_uartEngine
{
	private:
		//Attributs

		//Methods
		char writeChar(); //State Machine update

	protected:
		//Attributs

		//Methods

	public:
		//Constructor
		RF_uartEngine_Tx();

		//Methods

    bool msgSent();

		char sendStartRF();
		char sendMoveAngle(unsigned char angleName);
		char sendSetParam(unsigned char paramName, unsigned char paramFormat, const char* paramValue, unsigned short paramValueSize);
/*		char sendSetMultiParam(unsigned int nParam, const unsigned char* paramName, const unsigned char* paramFormat, const const char* paramValue, unsigned short* paramValueSize);
*/
		char sendGetParam(unsigned char paramName);

/*		char sendAnswerStartRF(bool OK);
		char sendAnswerMoveAngle(bool OK);
		char sendAnswerGetParam(char paramName, char paramFormat, unsigned int paramLength, double* paramValue);

		char sendErrCRC();
		char sendErrUnknowMsg();
		char sendErrCarte();
*/


		msg_uart getMsg(){return stateMachine.msg;}

}; // class RF_uartEngine_Tx
#endif // RF_UARTENGINE_TX_H
