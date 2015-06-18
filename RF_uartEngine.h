#ifndef RF_UARTENGINE_H 
#define RF_UARTENGINE_H

//include
#include "RF_stateMachine.h"

class RF_uartEngine
{
	private:
		//Attributs

		//Methods

	protected:
		//Attributs
		state_machine stateMachine;

		//Methods
		CRC_compute();
		bool CRC_check();

	public:
		//Attributs

		//Methods
		inline state_machine getStateMachine(){return stateMachine;}

}; // class RF_uartEngine
#endif // RF_UARTENGINE_H
