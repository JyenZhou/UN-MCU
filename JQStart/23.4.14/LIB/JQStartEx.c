#include "JFML.h"

#ifdef USE_HAL_DRIVER
#include  "HAL/STM32/stm32f1xx_hal_wwdg.c"
#endif








//96λUID�൱��3��32bitλ����  ÿ8bitλ�����൱��һ���ַ�  ���������Ҫ4*3+1 =13���ַ��ռ� Ϊ�˼��� ��λuid�ȴ� 
void uid2string(char* string,int buffSize )
{

    snprintf(string, buffSize, "%X%X%X",  JHAL_uidGetLow(), JHAL_uidGetMiddle(),JHAL_uidGetHigh());
 
 
}


























