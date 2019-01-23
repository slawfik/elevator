/*
 * EleComunicate.h
 *
 *  Created on: 3. 1. 2019
 *      Author: tomas
 */

#ifndef ELE_PROTOCOL_ELECOMUNICATE_H_
#define ELE_PROTOCOL_ELECOMUNICATE_H_

#include <stdio.h>
#include "../UART/L_BUFER.h"
#include "fsl_debug_console.h"
#include "fsl_pit.h"
#include "define.h"

/**
* activate_B_led_indicator - flag pre jednotlivé ledky 
* cabin_status - status kabíny
*/
typedef struct inside_elevator	{
	uint8_t activate_B_led_indicator[5];
	uint8_t cabin_status;
}INSIDE_ELEVATOR;

/**
* call_B_led_indicator - flag pre jednotlivé ledky hodnota 1, led bola stlačená
* disp_smer_pohybu - premenná smer pohibu používaná pre odosielanie na display
* poschodie - premená definujúca poschodie 0-P,1-1...
*/
typedef struct outside_elevator	{
	uint8_t call_B_led_indicator[5];
	uint8_t disp_smer_pohybu;
	uint8_t poschodie;
}OUTSIDE_ELEVATOR;

/**
* Adresy pre ledky vo vnútri kabíny
*/
enum adr_elevator_led_inside	{
	led4 = 0x24,
	led3 = 0x23,
	led2 = 0x22,
	led1 = 0x21,
	ledP = 0x20
};

class Ele_Comunicate {
public:
	Ele_Comunicate(uint8_t my_adr);
	virtual ~Ele_Comunicate();
	uint8_t pars_comand_button_function();
	uint8_t elevator_task();
	uint8_t isAck_paket();
	uint8_t read_Packet(L_BUFER* bufer);
	uint8_t lock_Elevator_Cabin();
	void reset_watchDog();
	void printOnTerminal(const char* text);
	void set_diplay_movment_none();

private:
	uint8_t my_addr;
	double poschodie;
	volatile uint8_t motor_status;
	uint8_t packet_Send[261];
	uint8_t packet_Receive[261];
	INSIDE_ELEVATOR inside_element;
	OUTSIDE_ELEVATOR outside_element;

	uint8_t turnled_ON(uint8_t adr);
	uint8_t turnled_OFF(uint8_t adr);
	uint8_t go_motor(const uint8_t speed);
	uint8_t set_display();
	uint8_t createPacket(const uint8_t dest,const uint8_t source,const uint8_t* data,const uint8_t data_len);
	uint8_t calculateCRC(const uint8_t* inputData,uint8_t dataLen);
	uint8_t unlock_Elevator_Cabin();
	void reset_emerigenci_break();
	void stop_motor();
	void get_elevator_position();
	void restart_watchDog();
	void getPoschodie();
	void Send();
};

#endif /* ELE_PROTOCOL_ELECOMUNICATE_H_ */
