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

#define TRUE 1
#define FALSE 0
#define HIGH_MASK 0xf0
#define LOW_MASK 0x0f

#define ADR_MOTOR 0xf1
#define TERMINAL_ADDR 0xd0
#define ADR_EMERIGENCY_BREAK 0x0f
#define ADR_ELEVATOR_CABIN 0xf0
#define ADR_ELEVATOR_WATCHDOG_TIMER 0xfe
#define ADR_INSIDE_button4 0xb4
#define ADR_INSIDE_button3 0xb3
#define ADR_INSIDE_button2 0xb2
#define ADR_INSIDE_button1 0xb1
#define ADR_INSIDE_buttonP 0xb0
#define ADR_CALL_BUTTON4 0xc4
#define ADR_CALL_BUTTON3 0xc3
#define ADR_CALL_BUTTON2 0xc2
#define ADR_CALL_BUTTON1 0xc1
#define ADR_CALL_BUTTONP 0xc0
#define ADR_INSIDE_LED4 0x24
#define ADR_INSIDE_LED3 0x23
#define ADR_INSIDE_LED2 0x22
#define ADR_INSIDE_LED1 0x21
#define ADR_INSIDE_LEDP 0x20
#define ADR_OUTSIDE_LED4 0x14
#define ADR_OUTSIDE_LED3 0x13
#define ADR_OUTSIDE_LED2 0x12
#define ADR_OUTSIDE_LED1 0x11
#define ADR_OUTSIDE_LEDP 0x10
#define ADR_LIMIT_SWITCH4 0xe4
#define ADR_LIMIT_SWITCH3 0xe3
#define ADR_LIMIT_SWITCH2 0xe2
#define ADR_LIMIT_SWITCH1 0xe1
#define ADR_LIMIT_SWITCHP 0xe0
#define ADR_DISPLAY 0x30

#define MOTOR_GET_STATE 0x03
#define COM_DISPLAY_UP 0x01
#define COM_DISPLAY_DOWN 0x02
#define COM_DISPLAY_NONE 0x03
#define COM_MOTOR_STOP 0x01
#define COM_MOTOR_GO 0x02
#define CAB_UNLOCK 0
#define CAB_LOCK 1
#define LED_ON 0x01
#define LED_OFF 0x00
#define START_BYTE 0xA0
#define SOURCE_BYTE 0x02
#define COMAND_BYTE 0x04
#define WD_RESET_VALUE 0x03
#define DEACTIVATE_E_BREAK 0x00
#define ACTIVATE_E_BREAK 0x01
#define ELEVATOR_GO_DOW ~(2-1)
#define ELEVATOR_GO_UP	2
#define INSIDE_BUTON_HIGH_MASK 0x02
#define OUTSIDE_BUTON_HIGH_MASK 0x01
#define RED_LIMIT_SWITCH 0x02
#define YELOW_LIMIT_SWITCH 0x01
#define	POSCHODIE4 4
#define	POSCHODIE3 3
#define	POSCHODIE2 2
#define	POSCHODIE1 1
#define	POSCHODIEP 0

typedef struct inside_elevator	{
	uint8_t activate_B_led_indicator[5];
	uint8_t cabin_status;
}INSIDE_ELEVATOR;

typedef struct outside_elevator	{
	uint8_t call_B_led_indicator[5];
	uint8_t disp_smer_pohybu;
	uint8_t poschodie;
}OUTSIDE_ELEVATOR;

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

	uint8_t read_Packet(L_BUFER* bufer);
	uint8_t lock_Elevator_Cabin();
	uint8_t unlock_Elevator_Cabin();
	uint8_t createPacket(uint8_t dest,uint8_t source,uint8_t* data,uint8_t data_len);
	uint8_t isAck_paket();
	uint8_t turnled_ON(uint8_t adr);
	uint8_t turnled_OFF(uint8_t adr);
	uint8_t go_motor(uint8_t speed);
	uint8_t set_display();
	void stop_motor();
	void printOnTerminal(char* text);
	void reset_watchDog();
	void reset_emerigenci_break();
	void set_diplay_movment_none();
private:
	uint8_t my_addr;
	double poschodie;
	uint8_t task;
	uint8_t motor_status;
	uint8_t packet_Send[261];
	uint8_t packet_Receive[261];
	INSIDE_ELEVATOR inside_element;
	OUTSIDE_ELEVATOR outside_element;

	uint8_t calculateCRC(uint8_t* inputData,uint8_t dataLen);
	void get_elevator_position();
	void getPoschodie();
	void Send();
};

#endif /* ELE_PROTOCOL_ELECOMUNICATE_H_ */
