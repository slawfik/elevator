/*
 * EleComunicate.cpp
 *
 *  Created on: 3. 1. 2019
 *      Author: tomas
 */

#include <ele_protocol/EleComunicate.h>
//#include "fsl_pit.h"

Ele_Comunicate::Ele_Comunicate(uint8_t my_adr)  {
	// TODO Auto-generated constructor stub
	my_addr = my_adr;
	poschodie = 0;
	outside_element.disp_smer_pohybu = COM_DISPLAY_NONE;
	outside_element.poschodie = 4.5;
	for(int i=0;i<5;i++)	{
		inside_element.activate_B_led_indicator[i] = 0;
		outside_element.call_B_led_indicator[i] = 0;
	}
	memset(this->packet_Send,0,sizeof(uint8_t)*261);
}

Ele_Comunicate::~Ele_Comunicate() {
	// TODO Auto-generated destructor stub
}

const uint8_t crcArray[] = { 	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
					157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
					35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
					190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
					70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
					219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
					101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
					248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
					140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
					17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
					175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
					50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
					202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
					87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
					233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
					116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53 };

uint8_t Ele_Comunicate::calculateCRC(uint8_t* inputData,uint8_t dataLen)	{
	uint8_t CRC_value = 0;
	uint8_t index = 0;
	for(uint8_t i = 0;i<dataLen;i++)	{
		index = CRC_value ^ inputData[i];
		CRC_value = crcArray[index];
	}

	return CRC_value;
}

uint8_t Ele_Comunicate::createPacket(uint8_t dest,uint8_t source,uint8_t* data,uint8_t data_len)	{
	uint8_t calc[255];
	calc[0] = dest;
	calc[1] = source;

	this->packet_Send[0] = START_BYTE;
	this->packet_Send[1] = dest;
	this->packet_Send[2] = source;
	this->packet_Send[3] = data_len;
	for(uint8_t i = 0; i<data_len;i++)	{
		this->packet_Send[i+4] = data[i];
		calc[i+2] = data[i];
	}
	this->packet_Send[4+data_len] = this->calculateCRC(calc, data_len+2);
	this->packet_Send[data_len +5] = '\0';
	return 0;
}

uint8_t Ele_Comunicate::read_Packet(L_BUFER* bufer)	{
	uint8_t ch;
	memset(packet_Receive,'_',261);
	if(bufer->read_from_Buff(&packet_Receive[0]))	{
		if(packet_Receive[0] == START_BYTE)	{
			while(bufer->read_from_Buff(&packet_Receive[1]) != 1);
			while(bufer->read_from_Buff(&packet_Receive[2]) != 1);
			while(bufer->read_from_Buff(&packet_Receive[3]) != 1);
			for(uint8_t i = 0;i<=packet_Receive[3];i++)	{
				while(bufer->read_from_Buff(&packet_Receive[i+4]) != 1);
			}
			//while(bufer->read_from_Buff(&packet_Receive[packet_Receive[3]+4]) != 1);
			return 1;
		} else {
			bufer->read_from_Buff(&ch);
			return 0;
		}
	}
	return 0;
}

void Ele_Comunicate::reset_watchDog()	{
	createPacket(ADR_ELEVATOR_WATCHDOG_TIMER,my_addr, (uint8_t*) WD_RESET_VALUE, 1);
	Send();
}

void Ele_Comunicate::Send()		{
	for(int i=0;i<packet_Send[3]+5;i++)	{
		PUTCHAR(packet_Send[i]);
	}
}

uint8_t Ele_Comunicate::lock_Elevator_Cabin()	{
	createPacket(ADR_ELEVATOR_CABIN,my_addr,(uint8_t*) CAB_LOCK, 1);
	Send();
	inside_element.cabin_status = CAB_LOCK;
	return (uint8_t) CAB_LOCK;
}

uint8_t Ele_Comunicate::unlock_Elevator_Cabin()		{
	createPacket(ADR_ELEVATOR_CABIN,my_addr, (uint8_t*) CAB_UNLOCK, 1);
	Send();
	inside_element.cabin_status = CAB_UNLOCK;
	return (uint8_t) CAB_UNLOCK;
}

uint8_t Ele_Comunicate::isAck_paket()	{
	if(packet_Receive[0] == START_BYTE && packet_Receive[1] == 0 && packet_Receive[2] == 0 && packet_Receive[3] == 0/* && packet_Receive[4] == 0*/)		{
		return TRUE;
	}	else {
		return FALSE;
	}
}

void Ele_Comunicate::printOnTerminal(char* text)	{
	createPacket(TERMINAL_ADDR, my_addr, (uint8_t*) text,strlen(text));
	Send();
}

uint8_t Ele_Comunicate::turnled_ON(uint8_t adr)	{
	createPacket(adr, my_addr,(uint8_t*) LED_ON, 1);
	Send();
	if(((adr & HIGH_MASK)>>4) == INSIDE_BUTON_HIGH_MASK)	{	//B
		inside_element.activate_B_led_indicator[adr & LOW_MASK] = LED_ON;
	} else if (((adr & HIGH_MASK)>>4) == OUTSIDE_BUTON_HIGH_MASK)	{	//C
		outside_element.call_B_led_indicator[adr & LOW_MASK] = LED_ON;
	}
	return 0;
}

uint8_t Ele_Comunicate::turnled_OFF(uint8_t adr)	{
	createPacket(adr, my_addr,(uint8_t*) LED_OFF, 1);
	Send();
	if(((adr & HIGH_MASK)>>4) == INSIDE_BUTON_HIGH_MASK)	{	//B
		inside_element.activate_B_led_indicator[adr & LOW_MASK] = LED_OFF;
	} else if (((adr & HIGH_MASK)>>4) == OUTSIDE_BUTON_HIGH_MASK)	{	//C
		outside_element.call_B_led_indicator[adr & LOW_MASK] = LED_OFF;
	}
	return 0;
}

uint8_t Ele_Comunicate::go_motor(uint8_t smer)		{
	uint8_t data[5];
	data[0] = COM_MOTOR_GO;
	if(smer == (uint8_t)ELEVATOR_GO_DOW)		{
		data[1] = 0xff;
		data[2] = 0xff;
		data[3] = 0xff;
	} else {
		data[1] = 0x00;
		data[2] = 0x00;
		data[3] = 0x00;
	}
	data[4] = smer;
	createPacket(ADR_MOTOR, my_addr, data, 5);
	Send();
	return 0;
}

void Ele_Comunicate::stop_motor()	{
	uint8_t stop_comand = 0x01;
	createPacket(ADR_MOTOR, my_addr,&stop_comand, 1);//(uint8_t*) COM_MOTOR_STOP
	Send();
}

void Ele_Comunicate::reset_emerigenci_break()	{
	createPacket(ADR_EMERIGENCY_BREAK, my_addr,DEACTIVATE_E_BREAK ,1);
	Send();
}

double Ele_Comunicate::get_elevator_default_position()	{
	double* motor_count;
	//uint8_t com = 0x03;
	createPacket(ADR_MOTOR, my_addr,(uint8_t*)MOTOR_GET_STATE, 1);
	Send();
	while(!read_Packet(&l_buffer) || packet_Receive[2] != ADR_MOTOR);
	motor_count = (double*)(&packet_Receive[4]);
	return *motor_count;
}

uint8_t Ele_Comunicate::elevator_task()	{
	if(outside_element.disp_smer_pohybu == COM_DISPLAY_NONE)	{
		for(int i = 0;i<5;i++)	{

		}
	} else if (outside_element.disp_smer_pohybu == COM_DISPLAY_DOWN) {

	} else {	 //COM_DISPLAY_UP

	}

	return 0;
}

uint8_t Ele_Comunicate::pars_comand_limitSwitch_function()	{
	switch(packet_Receive[2])	{
		case ADR_LIMIT_SWITCH1:
			if(inside_element.activate_B_led_indicator[1] || outside_element.call_B_led_indicator[1])	{
				if(packet_Receive[COMAND_BYTE] == 0x01)	{
					stop_motor();
				}	else if (packet_Receive[COMAND_BYTE] == 0x02) {
					//open cabine and close
					turnled_OFF((uint8_t) ADR_OUTSIDE_LED1);
					turnled_OFF((uint8_t) ADR_INSIDE_LED1);
					unlock_Elevator_Cabin();
					PIT_StartTimer(PIT, kPIT_Chnl_1);
				}
			} //else status display o poschodí
			return 1;
		case ADR_LIMIT_SWITCH2:
			if(inside_element.activate_B_led_indicator[2] || outside_element.call_B_led_indicator[2])	{
				if(packet_Receive[COMAND_BYTE] == 0x01)	{
					stop_motor();
				}	else if (packet_Receive[COMAND_BYTE] == 0x02) {
					//open cabine and close
					turnled_OFF((uint8_t) ADR_OUTSIDE_LED2);
					turnled_OFF((uint8_t) ADR_INSIDE_LED2);
					unlock_Elevator_Cabin();
					PIT_StartTimer(PIT, kPIT_Chnl_1);
				}
			}
			return 1;
		case ADR_LIMIT_SWITCH3:
			if(inside_element.activate_B_led_indicator[3] || outside_element.call_B_led_indicator[3])	{
				if(packet_Receive[COMAND_BYTE] == 0x01)	{
					stop_motor();
				}	else if (packet_Receive[COMAND_BYTE] == 0x02) {
					//open cabine and close
					turnled_OFF((uint8_t) ADR_OUTSIDE_LED3);
					turnled_OFF((uint8_t) ADR_INSIDE_LED3);
					unlock_Elevator_Cabin();
					PIT_StartTimer(PIT, kPIT_Chnl_1);
				}
			}
			return 1;
		case ADR_LIMIT_SWITCH4:
			if(inside_element.activate_B_led_indicator[4] || outside_element.call_B_led_indicator[4])	{
				if(packet_Receive[COMAND_BYTE] == 0x01)	{
					stop_motor();
				}	else if (packet_Receive[COMAND_BYTE] == 0x02) {
					//open cabine and close
					turnled_OFF((uint8_t) ADR_OUTSIDE_LED4);
					turnled_OFF((uint8_t) ADR_INSIDE_LED4);
					unlock_Elevator_Cabin();
					PIT_StartTimer(PIT, kPIT_Chnl_1);
				}
			}
			return 1;
		case ADR_LIMIT_SWITCHP:
			if(inside_element.activate_B_led_indicator[0] || outside_element.call_B_led_indicator[0])	{
				if(packet_Receive[COMAND_BYTE] == 0x01)	{
					stop_motor();
				}	else if (packet_Receive[COMAND_BYTE] == 0x02) {
					//open cabine and close
					turnled_OFF((uint8_t) ADR_OUTSIDE_LEDP);
					turnled_OFF((uint8_t) ADR_INSIDE_LEDP);
					unlock_Elevator_Cabin();
					PIT_StartTimer(PIT, kPIT_Chnl_1);
				}
			}
			return 1;
	}
	return 0;
}

uint8_t Ele_Comunicate::pars_comand_button_function()	{
	switch(packet_Receive[SOURCE_BYTE])	{
		case ADR_INSIDE_button1:
			turnled_ON((uint8_t) ADR_INSIDE_LED1);
			outside_element.poschodie = get_elevator_default_position();
			return 1;
		case ADR_INSIDE_button2:
			turnled_ON((uint8_t) ADR_INSIDE_LED2);
			return 1;
		case ADR_INSIDE_button3:
			turnled_ON((uint8_t) ADR_INSIDE_LED3);
			return 1;
		case ADR_INSIDE_button4:
			turnled_ON((uint8_t) ADR_INSIDE_LED4);
			return 1;
		case ADR_INSIDE_buttonP:
			turnled_ON((uint8_t) ADR_INSIDE_LEDP);
			stop_motor();
			return 1;
		case ADR_CALL_BUTTON1:
			turnled_ON((uint8_t) ADR_OUTSIDE_LED1);
			return 1;
		case ADR_CALL_BUTTON2:
			turnled_ON((uint8_t) ADR_OUTSIDE_LED2);
			return 1;
		case ADR_CALL_BUTTON3:
			turnled_ON((uint8_t) ADR_OUTSIDE_LED3);
			stop_motor();
			return 1;
		case ADR_CALL_BUTTON4:
			turnled_ON((uint8_t) ADR_OUTSIDE_LED4);
			go_motor((uint8_t)ELEVATOR_GO_UP);
			return 1;
		case ADR_CALL_BUTTONP:
			turnled_ON((uint8_t) ADR_OUTSIDE_LEDP);
			go_motor((uint8_t)ELEVATOR_GO_DOW);
			return 1;
		case TERMINAL_ADDR:
			reset_emerigenci_break();
			return 1;
	}
	return 0;
}