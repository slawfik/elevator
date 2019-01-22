 
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

/*
 * @brief   Application entry point.
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

    //uint8_t znak = 0;
    Ele_Comunicate elevator(0x01);
    pit_init(&elevator);

    /* Force the counter to be placed into memory. */
   //char i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
    	if(elevator.read_Packet(&l_buffer) && !elevator.isAck_paket())	{
    		elevator.pars_comand_button_function();
    	} else {
    		elevator.elevator_task();
    	}


    		/*if(elevator.isAck_paket() == FALSE && i == 0)	{
    			elevator.pars_comand_main_function();
    			i = elevator.lock_Elevator_Cabin();
    			elevator.printOnTerminal("L_Ahoj!\n");
    		} else {
				if(elevator.isAck_paket() == FALSE && i == 1){
					elevator.pars_comand_main_function();
					i = elevator.unlock_Elevator_Cabin();
					elevator.printOnTerminal("U_Ahoj!\n");
				}
    		}*/
    	//	for(int u=0;u<0x00FFFFFF;u++);
       /* for(int u=0;u<0x0000FFFF;u++);
        i = l_buffer.read_from_Buff(&znak);
        if(i)	{
        	PUTCHAR(znak);
        }*/
	}
    return 0 ;
}
