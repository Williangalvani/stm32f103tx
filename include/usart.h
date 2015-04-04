/*
 * uart.h
 *
 *  Created on: Apr 2, 2015
 *      Author: will
 */

#include "stm32f10x_usart.h"

#ifndef UART_H_
#define UART_H_
#define RECV_BUFFER_SIZE 16

class Serial {
public:
	Serial() {

	}


	unsigned char usart_recv_buffer[RECV_BUFFER_SIZE]; //! Ring-Buffer de recebimento de USART2.
	int usart_rb_in = 0;//! Index do Ring-Buffer para recebimento na USART2.
	bool usart_available_flag = 0;//! Flag de recebimento de USART2.

	void init(int baudrate,int port);


	void Usart1Put(uint8_t ch);
	uint8_t Usart1Get(void);

private:


};

#endif /* UART_H_ */
