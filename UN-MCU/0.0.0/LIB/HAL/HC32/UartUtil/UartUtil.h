#ifndef __UART_UTIL_H__
#define __UART_UTIL_H__

#include "JUTIL.h"


#include "uart.h"
#include "bt.h"
#include "lpm.h"

//�ڲ���������ʱ������󳤶� �ڶ�������ʱ���Ƕ��ĳ���
#define Uart1_ReceivingLength 9

void uartUtilinit(void);
extern unsigned char   ReceiveData_Uart1[Uart1_ReceivingLength];
extern unsigned char volatile RecEnd_Uart1;

void uart1SendData(uint8 * data,uint16 length);
#ifdef __cplusplus
extern "C" {
#endif
#endif
