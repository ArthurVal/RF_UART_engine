#include "RF_uartEngine.h"

RF_uartEngine::RF_uartEngine()
{
  stateMachine.state = STATE_INIT;

	stateMachine.msg.ptrMsg = 0;
	stateMachine.msg.sizeMsg = 1;
	stateMachine.msg.currentMsg = (unsigned char*)malloc(sizeof(unsigned char) * stateMachine.msg.sizeMsg);
;

	stateMachine.msg.functionCode = 0xFF;
	stateMachine.msg.length = 0xFFFF;

	stateMachine.msg.sizeData = 1;
	stateMachine.msg.Data = (unsigned char*)malloc(sizeof(unsigned char) * stateMachine.msg.sizeData );

	stateMachine.msg.crc_msg = stateMachine.msg.crc_compute = 0xFFFF;

	stateMachine.status = STATUS_WAIT;
}

/*==================================================*/
/*==                 MSG_clear                    ==*/
/*==================================================*/

void RF_uartEngine::MSG_clear()
{
  stateMachine.msg.ptrMsg = 0;

	free(stateMachine.msg.currentMsg);
	free(stateMachine.msg.Data);

	stateMachine.msg.functionCode = stateMachine.msg.functionCode = 0xFF;

	stateMachine.msg.length = stateMachine.msg.length = 0xFFFF;

	stateMachine.msg.nbrParam = 0x00;

	for(int i = 0 ; i < 256 ; ++i){
		stateMachine.msg.ID[i] = 0x00;
	}	

	stateMachine.msg.crc_msg = stateMachine.msg.crc_compute = 0xFFFF;

	stateMachine.RX_msgRdy = false;
	stateMachine.TX_msgEnd = false;

}

/*==================================================*/
/*==                CRC_compute                   ==*/
/*==================================================*/

void RF_uartEngine::CRC_compute()
{
	stateMachine.msg.crc_compute = 0xFFFF;
 
	for (int pos = 0; pos < (stateMachine.msg.length + 4); ++pos) {
		// XOR byte into least sig. byte of crc
		stateMachine.msg.crc_compute ^= (unsigned short)stateMachine.msg.currentMsg[pos];     
 
		for (int i = 8; i != 0; i--) {    // Loop over each bit
			if ((stateMachine.msg.crc_compute & 0x0001) != 0) {      // If the LSB is set
				stateMachine.msg.crc_compute >>= 1;                    // Shift right and XOR 0xA001
				stateMachine.msg.crc_compute ^= 0xA001;
			}
			else                            // Else LSB is not set
				stateMachine.msg.crc_compute >>= 1;                    // Just shift right
		}
	}	
}

/*==================================================*/
/*==                 CRC_check                    ==*/
/*==================================================*/

bool RF_uartEngine::CRC_check()
{
	return (stateMachine.msg.crc_compute == stateMachine.msg.crc_msg);
}
