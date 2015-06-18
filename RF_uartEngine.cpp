#include "uartEngine.h"

uartEngine::uartEngine()
{
  stateMachine.state = STATE_INIT;

	stateMachine.functionCode = 0xFF;

	stateMachine.crc_msg = stateMachine.crc_compute = stateMachine.length = 0xFFFF;

	stateMachine.status = 0;
}


uartEngine::CRC_compute()
{
	stateMachine.crc_compute = 0xFFFF;
 
	for (int pos = 0; pos < (stateMachine.length + 2); ++pos) {
		// XOR byte into least sig. byte of crc
		stateMachine.crc_compute ^= (uint16_t)stateMachine.currentMsg[pos];          
 
		for (int i = 8; i != 0; i--) {    // Loop over each bit
			if ((stateMachine.crc_compute & 0x0001) != 0) {      // If the LSB is set
				stateMachine.crc_compute >>= 1;                    // Shift right and XOR 0xA001
				stateMachine.crc_compute ^= 0xA001;
			}
			else                            // Else LSB is not set
				stateMachine.crc_compute >>= 1;                    // Just shift right
		}
	}	
}

bool uartEngine::CRC_check()
{
	return (stateMachine.crc_compute == stateMachine.crc_msg);
}
