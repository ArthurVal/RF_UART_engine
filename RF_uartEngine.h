#ifndef RF_UARTENGINE_H 
#define RF_UARTENGINE_H

//include
#include "RF_stateMachine.h"
#include <stdlib.h>

class RF_uartEngine
{
	private:
		//Attributs

		//Methods

	protected:
		//Attributs
		state_machine stateMachine;

		//Methods
		void MSG_clear();
		void CRC_compute();
		bool CRC_check();

	public:
		//Constructor & Destructor
		RF_uartEngine();
		inline ~RF_uartEngine() {free(stateMachine.msg.currentMsg); free(stateMachine.msg.Data);};
		//Attributs

		//Methods
		inline state_machine getStateMachine(){return stateMachine;}

}; // class RF_uartEngine
#endif // RF_UARTENGINE_H
