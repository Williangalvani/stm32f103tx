/*
 * usart.c
 *
 *  Created on: Apr 2, 2015
 *      Author: will
 */

#include "usart.h"




void Serial::init(int baudrate, int port) {
	{

		GPIO_InitTypeDef GPIO_InitStructure;

		USART_InitTypeDef USART_InitStructure;

		USART_ClockInitTypeDef USART_ClockInitStructure;

		//enable bus clocks

//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

		//Set USART1 Tx (PA.09) as AF push-pull

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		GPIO_Init(GPIOA, &GPIO_InitStructure);

		//Set USART1 Rx (PA.10) as input floating

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

		GPIO_Init(GPIOA, &GPIO_InitStructure);

		USART_ClockStructInit(&USART_ClockInitStructure);

		USART_ClockInit(USART1, &USART_ClockInitStructure);

		USART_InitStructure.USART_BaudRate = baudrate;

		USART_InitStructure.USART_WordLength = USART_WordLength_8b;

		USART_InitStructure.USART_StopBits = USART_StopBits_1;

		USART_InitStructure.USART_Parity = USART_Parity_No;

		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

		//Write USART1 parameters

		USART_Init(USART1, &USART_InitStructure);

		//Enable USART1

		USART_Cmd(USART1, ENABLE);

	}
}

void Serial::Usart1Put(uint8_t ch)
{
	  USART_SendData(USART1, (uint16_t) ch);
	  //Loop until the end of transmission
	  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	  {
	  }
}
uint8_t Serial::Usart1Get(void){
	 while ( USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (uint8_t)USART_ReceiveData(USART1);
}


