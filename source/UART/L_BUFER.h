/*
 * LBUFER.h
 *
 *  Created on: 17. 1. 2019
 *      Author: tomas
 */

#ifndef UART_L_BUFER_H_
#define UART_L_BUFER_H_

#include <stdio.h>

class L_BUFER {
private:
	uint8_t cread, cwrite;
	uint8_t* bufer;
	uint8_t* start;
	uint8_t* write;
	uint8_t* read;
	uint8_t size;
public:
	L_BUFER(uint8_t* buf,uint8_t p_size);
	virtual ~L_BUFER();
	uint8_t writ_to_Buff(uint8_t i_znak);
	uint8_t read_from_Buff(uint8_t* o_znak);
};


extern L_BUFER l_buffer;
#endif /* UART_L_BUFER_H_ */
