#include "..\JHAL_Flash.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		Flash默认通用HAL驱动  注意封装内部已经做了地址偏移   传进去的00地址就是falsh所在首地址
 		另外要注意大小不要超出  当然这里面也有防错机制

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
#define FLASH_WAITETIME 50000       //FLASH等待超时时间 


#define FlashStartAddr  ((uint32_t)0x08000000)
#if defined MM32G0001

//扇区大小1K
#define FlashPageSize 1024

#define FlashPageNumber 16
#define FlashMaxSize  FlashPageSize*FlashPageNumber




#else
#error 未定义flash大小
#endif



#define FlashEndAddr (FlashStartAddr+FlashMaxSize)

 
 
/*******************************************************************************
*@ 函数功能或简介: 擦出指定扇区区间的Flash数据
  * @输入参数:StartPage 起始地址 (必须扇区对齐)
							EndPage 结束地址  (会自动对齐扇区)

  * @返 回 值: 扇区擦出状态
  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

bool JHAL_flashErasePage(uint32_t startPageAddr, uint32_t endAddr)
{
    JHAL_disableInterrupts();
    FLASH_Unlock();
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

    //判断开始地址是否按照扇区对齐 否则会出错
    while((startPageAddr%FlashPageSize)!=0);
    //地址超出
    while(endAddr>FlashEndAddr);
    for (u32 i = startPageAddr; i <= endAddr; i += FlashPageSize)
    {
        FLASH_ErasePage(i);
        if(FLASH_COMPLETE!= FLASH_WaitForLastOperation(FLASH_WAITETIME))
        {
            return false;
        }

        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }

    FLASH_Lock();
    JHAL_enableInterrupts();


    return true;

}




/**
  * 描述  flash 程序区写多个字节数据
  * 输入  address： 指定flash地址
  *  p_FlashBuffer: 写入数据的指针
  *    leng：数据的长度
  * 返回  无。
  * 注：写入前要先擦除地址所在的页 //多出的不足8个字节按四个直接处理  因为是读数据所以后面指针超出没事  但是下次写入地址要隔开8
  */
bool JHAL_flashWriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng)//地址必须为被8整除
{


    JHAL_disableInterrupts();
    FLASH_Unlock();
    for(u16 i=0; i<leng; i++) //一页1024byte，缓冲一次写64bit=8byte，128个缓冲块
    {
        FLASH_ProgramWord(address, *p_FlashBuffer);
        if(FLASH_COMPLETE!= FLASH_WaitForLastOperation(FLASH_WAITETIME))
        {
            return false;
        }
        FLASH_ClearFlag(FLASH_FLAG_EOP);
        p_FlashBuffer+=4;
        address+=4;
        FLASH_ClearFlag(FLASH_FLAG_EOP);
    }
    JHAL_enableInterrupts();
    return true;
} 


