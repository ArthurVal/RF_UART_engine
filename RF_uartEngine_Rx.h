#ifndef RF_UARTENGINE_RX_H 
#define RF_UARTENGINE_RX_H 

#include "RF_uartEngine.h"
#include <stdio.h>
#include <stdlib.h>

class RF_uartEngine_Rx : public RF_uartEngine
{
	private:
		//Attrib
		
		//Methods

	protected:
		//Attributs

		//Methods

	public:
		//Constructor
		RF_uartEngine_Rx();

		//Methods
		bool readChar(unsigned char);	
		bool msgIsReady();
		

}; // class RF_uartEngine_Rx
#endif // RF_UARTENGINE_RX_H
