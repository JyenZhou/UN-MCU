#ifndef __TCP_DEMO_H
#define __TCP_DEMO_H
#include "HALL_MyBSP/BoardLoadingPeripheral/W5500/W5500/types.h"

extern uint16 W5500_tcp_server_port;
void do_tcp_server(void);//TCP Server�ػ���ʾ����
void do_tcp_client(void);//TCP Clinet�ػ���ʾ����

extern uint8 isLogin;
#endif 

