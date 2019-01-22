/*
 * pittimer.h
 *
 *  Created on: 18. 1. 2019
 *      Author: Slavkovský
 */

#ifndef PIT_TIMER_PITTIMER_H_
#define PIT_TIMER_PITTIMER_H_

#include <ele_protocol/EleComunicate.h>		//odtial je includnutý aj "fsl_pit.h"

#define PIT_TIMER_WD 80000
#define PIT_ELEVATO_CABINE 2000000

void pit_init(Ele_Comunicate* pa_elevator);

#endif /* PIT_TIMER_PITTIMER_H_ */
