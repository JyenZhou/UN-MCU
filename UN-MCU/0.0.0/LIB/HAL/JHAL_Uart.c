#include "JHAL_Uart.h"
OS_WEAK JHAL_UART *   __JHAL_uartGetPrintfJUart(void)
	
{
	// �����JHAL_uartRegistPrintf���а�Ҫ�ض���Ĵ���
	JHAL_error("__JHAL_uartGetPrintfJUart");
	return NULL;
}


#ifndef  UartExistRedirect

OS_WEAK int fputc (int ch,FILE * f)
{
 	// ��ʵ���ض���  ��ʹ����������Ҫ��ʾʵ�ֵ� ��ʹ�ú궨������
	  JHAL_error("fputc");
    return 0;
}

 
OS_WEAK int fgetc (FILE * f)
{
   JHAL_error("fgetc");
    return 0;
}

#endif


