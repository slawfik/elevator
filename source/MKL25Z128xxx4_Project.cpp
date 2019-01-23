 
/**
 * @file    MKL25Z128xxx4_Project.cpp
 * @brief   Application entry point.
 */

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include <ele_protocol/EleComunicate.h>
#include "pit_timer/pittimer.h"

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/**
 * @file    MKL25Z128xxx4_Project.cpp
 * @param
 */

uint8_t buffer[50];
L_BUFER l_buffer(buffer,50);

int main(void) {

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();

    Ele_Comunicate elevator(0x01);
    pit_init(&elevator);

    while(1) {
    	if(elevator.read_Packet(&l_buffer) && !elevator.isAck_paket())	{
    		elevator.pars_comand_button_function();
    	} else {
    		elevator.elevator_task();
    	}
	}
    return 0 ;
}
