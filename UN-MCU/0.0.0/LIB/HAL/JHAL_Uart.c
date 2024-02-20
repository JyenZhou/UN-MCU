#include "JHAL_Uart.h"
OS_WEAK JHAL_UART *   __JHAL_uartGetPrintfJUart(void)
	
{
	// 请调用JHAL_uartRegistPrintf进行绑定要重定向的串口
	JHAL_error("__JHAL_uartGetPrintfJUart");
	return NULL;
}


#ifndef  UartExistRedirect

OS_WEAK int fputc (int ch,FILE * f)
{
 	// 请实现重定向  若使用软件库或不需要显示实现的 请使用宏定义屏蔽
	  JHAL_error("fputc");
    return 0;
}

 
OS_WEAK int fgetc (FILE * f)
{
   JHAL_error("fgetc");
    return 0;
}

#endif


