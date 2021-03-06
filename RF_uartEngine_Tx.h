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
		void createCurrentMsg(); //Create the message

	protected:
		//Attributs

		//Methods

	public:
		//Constructor
		RF_uartEngine_Tx();

		//Methods
    bool msgSent();
    bool transmitError();

		void clearMsg();

		char sendStartRF();
		char sendMoveAngle(unsigned char angleName);

		char sendSetParam(unsigned char paramName, 
		                  unsigned char paramFormat, 
		                  const char* paramValue, 
		                  unsigned short paramValueSize);

		char sendSetMultiParam(unsigned int nParam,
		                       const unsigned char* paramName, 
		                       const unsigned char* paramFormat, 
		                       char** paramValue, 
		                       unsigned short* paramValueSize);

		char sendGetParam(unsigned char paramName);

		char sendAnswerStartRF(bool OK);
		char sendAnswerMoveAngle(bool OK);
		char sendAnswerGetParam(unsigned char paramName, unsigned char paramFormat, const char* paramValue, unsigned short paramValueSize);

		char sendErrCRC();
		char sendErrUnknowMsg();
		char sendErrCarte();

}; // class RF_uartEngine_Tx
#endif // RF_UARTENGINE_TX_H
