#include "../JHAL_LowPower.h"
 




/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: Stop模式 内存数据保留，基本外设开启
 
  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

 void enterLowPowerByStopMode(void)
{
    SPM_EnableLVD(DISABLE);
    // SPM_EnablePLL(DISABLE);//好像关了这个程序就死掉了 可能是我没开这个
    SPM_EnableXOSC(DISABLE);//外部晶振关闭
    SysStop();
    SPM_EnableLVD(ENABLE);
    //  SPM_EnablePLL(ENABLE);
    SPM_EnableXOSC(ENABLE);//外部晶振关闭
}

