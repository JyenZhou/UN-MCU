#include "JFML.h"

#ifdef USE_HAL_DRIVER
#include  "HAL/STM32/stm32f1xx_hal_wwdg.c"
#endif








//96位UID相当于3个32bit位变量  每8bit位变量相当于一个字符  所以最大需要4*3+1 =13个字符空间 为了兼容 低位uid先存 
void uid2string(char* string,int buffSize )
{

    snprintf(string, buffSize, "%X%X%X",  JHAL_uidGetLow(), JHAL_uidGetMiddle(),JHAL_uidGetHigh());
 
 
}


























