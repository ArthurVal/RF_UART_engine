#ifndef RF_UARTENGINE_H 
#define RF_UARTENGINE_H

//include
#include "RF_stateMachine.h"
#include <stdlib.h>
#include <stdio.h>

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

		//Methods
			//Block the state Machine if true. Unlock it if false.
		void lockStateMachine(bool lock);
	
		inline msg_uart getMsg()               {stateMachine.msgRead = true;		                         
		                                       return stateMachine.msg;
														               }
		inline state_machine getStateMachine() {return stateMachine;}
		int getStatus();

}; // class RF_uartEngine
#endif // RF_UARTENGINE_H
