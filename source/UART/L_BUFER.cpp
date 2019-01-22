/*
 * LBUFER.cpp
 *
 *  Created on: 17. 1. 2019
 *      Author: tomas
 */

#include <UART/L_BUFER.h>
#include <fsl_clock.h>
#include <fsl_lpsci.h>
#include <assert.h>

/*##########Init UART f###########*/
lpsci_config_t user_config;

void initUart()	{
	LPSCI_GetDefaultConfig(&user_config);
	user_config.baudRate_Bps = 57600U;
	user_config.enableTx = true;
	user_config.enableRx = true;

	LPSCI_Init(UART0, &user_config, CLOCK_GetFreq(kCLOCK_PllFllSelClk));
	LPSCI_DisableInterrupts(UART0, kLPSCI_AllInterruptsEnable);

	/* Enable LPSCI RX IRQ if previously enabled. */
	LPSCI_EnableInterrupts(UART0, kLPSCI_RxDataRegFullInterruptEnable);

	/* Enable interrupt in NVIC. */
	EnableIRQ(UART0_IRQn);
}

int __sys_write(int iFileHandle, char *pcBuffer, int iLength) {
	// Write String to BUFFER
	//int size = bufferWrite(&cBuffer, (uint8_t *) pcBuffer, iLength);
	/*for(int i=0; i<iLength; i++){
		l_buffer
		vBuffer->bufferWrite(pcBuffer[i]);
	}*/
	//l_buffer.read_from_Buff((uint8_t*)pcBuffer);
	//int size = vBuffer->getSize();
	LPSCI_EnableInterrupts(UART0,
			kLPSCI_TxDataRegEmptyInterruptEnable
					| kLPSCI_RxOverrunInterruptEnable); // kLPSCI_TransmissionCompleteInterruptEnable | kLPSCI_RxOverrunInterruptEnable);
	return iLength;
}

extern "C" void UART0_IRQHandler(void) {
	UART0_Type *base = UART0;
	uint8_t send_ch;

	/* Send data register empty and the interrupt is enabled. */
	if ((base->S1 & kLPSCI_TxDataRegEmptyFlag)) {
		//int c = bufferRead(&cBuffer, &pcBuffer, 1);
		int c = 0;//l_buffer.read_from_Buff(&send_ch);
		if (c > 0) {
//			LPSCI_WriteBlocking(UART0, &send_ch, 1);
			UART0->D = send_ch;
		} else {
			/* Disable TX register empty interrupt. */
//			base->C2 &= ~UART0_C2_TIE_MASK;
			LPSCI_DisableInterrupts(base, kLPSCI_TxDataRegEmptyInterruptEnable);
		}
		LPSCI_ClearStatusFlags(base, kLPSCI_TxDataRegEmptyFlag);
	}
	/* If RX overrun. */
	if (UART0_S1_OR_MASK & base->S1) {
		while (UART0_S1_RDRF_MASK & base->S1) {
			(void) base->D;
		}

		LPSCI_ClearStatusFlags(base, kLPSCI_RxOverrunFlag);
	}

	/* Receive data register full */
	if ((UART0_S1_RDRF_MASK & base->S1) && (UART0_C2_RIE_MASK & base->C2)) {
		uint8_t rxData;
		//static uint8_t size = 0;
		rxData = base->D;

		l_buffer.writ_to_Buff(rxData);

		send_ch = 0;
	}
}
/*#########END init UART f ############*/

L_BUFER::L_BUFER(uint8_t* buf,uint8_t p_size) {
	// TODO Auto-generated constructor stub
	bzero(buf,p_size);
	cread = 0;
	cwrite = 0;
	bufer = buf;
	read = buf;
	write = buf;
	start = buf;
	size = p_size;

	initUart();
}

L_BUFER::~L_BUFER() {
	// TODO Auto-generated destructor stub
}

//uint8_t debug[50];
//int i =0;
uint8_t L_BUFER::writ_to_Buff(uint8_t i_znak)	{
	//i++;
	if(cwrite == size-1)	{
		if(cread != 0)	{
			bufer[cwrite] = i_znak;
			//*write = i_znak;
			//write = start;
					//debug[cwrite] = i_znak;
			cwrite = 0;
		}
	} else {
		if(cwrite+1 != cread)	{
			bufer[cwrite] = i_znak;
			//*write = i_znak;
					//debug[cwrite] = i_znak;
			//write++;
			cwrite++;
		}
	}
/*	if(*(write-1) == 0xFF)	{
		i_znak = cwrite +1;
	}	else if (bufer[cwrite-1] == 'B') {
		i_znak = cwrite +1;
	}	else if (i == 14) {
		i_znak = cwrite +1;
	}	else if (i == 20) {
		i_znak = cwrite +1;
	}*/
	return 0;
}

uint8_t L_BUFER::read_from_Buff(uint8_t* o_znak)	{
	if(cread != cwrite)	{
		*o_znak = bufer[cread];
		 bufer[cread] = '\0';
		if(cread == size-1)	{
			cread = 0;
		}	else {
			cread++;
		}
		return 1;
	}	else {
		return 0;
	}
}
