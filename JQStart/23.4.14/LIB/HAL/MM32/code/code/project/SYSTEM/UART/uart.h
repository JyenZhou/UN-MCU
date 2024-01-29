#ifndef __UART_H
#define __UART_H

#include  "main.h"

void Uart_Init(uint32_t bound);

void Send_Data_To_UART0(uint8_t dat);

extern uint8_t	 recv_byte[USART_RX_LEN];
extern uint8_t	 recv_byte_temp[USART_RX_LEN];
extern uint8_t	 recv_count;
extern uint8_t	 recv_timeout;

#endif
