/*
 * pittimer.cpp
 *
 *  Created on: 18. 1. 2019
 *      Author: Slavkovský
 */

#include <pit_timer/pittimer.h>
//#include "fsl_pit.h"

Ele_Comunicate* _elevator;

void pit_init(Ele_Comunicate* pa_elevator)	{
	_elevator = pa_elevator;

	pit_config_t pitConfig;
	PIT_GetDefaultConfig(&pitConfig);
	/*PIT pre watch dog*/
	PIT_Init(PIT, &pitConfig);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(PIT_TIMER_WD, CLOCK_GetFreq(kCLOCK_BusClk)));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	/*PIT pre odomknutie a zamknutie kabíny*/
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_1, USEC_TO_COUNT(PIT_ELEVATO_CABINE, CLOCK_GetFreq(kCLOCK_BusClk)));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_1, kPIT_TimerInterruptEnable);

	EnableIRQ(PIT_IRQn);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
	//PIT_StartTimer(PIT, kPIT_Chnl_1);
}

extern "C" void PIT_IRQHandler(void)
{
	/* Clear interrupt flag.*/
	if(PIT->CHANNEL[kPIT_Chnl_0].TFLG)	{
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		_elevator->reset_watchDog();
	}	else if (PIT->CHANNEL[kPIT_Chnl_1].TFLG) {
		PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
		PIT_StopTimer(PIT,kPIT_Chnl_1 );
		_elevator->lock_Elevator_Cabin();
		_elevator->set_diplay_movment_none();
	}


}
