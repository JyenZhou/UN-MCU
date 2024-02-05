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


#define FlashStartAddr FLASH_BASE
#if defined STM32F103xE   || defined STM32F103xB

//扇区大小1K
#define FlashPageSize 0x400
//256K xe的大小是256-512 定义小的兼容性好 一般来说够用了  如果超出就再加个具体定义的宏定义
#define FlashPageNumber 256
#define FlashMaxSize  (*((uint16_t *)FLASH_SIZE_DATA_REGISTER))


#elif defined (STM32F407xx)
//16K
#define FlashPageSize 0x4000
#define FlashPageNumber 10
#define FlashMaxSize  (*((uint16_t *)0x1FFF7A22))

#else
#error 未定义flash大小
#endif



#define FlashEndAddr (FlashStartAddr+FlashMaxSize)


uint16_t getFlashSize()
{


    return FlashMaxSize;

}


 



bool __JHAL_flashErasePage(const uint32_t targetaddress )
{



    //1、解锁FLASH

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef

    FLASH_EraseInitTypeDef f;
#if defined (STM32F407xx)
    f.TypeErase = FLASH_TYPEERASE_SECTORS;     //擦除类型，扇区擦除
    f.Sector = targetaddress ; //要擦除的扇区起始地址
    f.VoltageRange = FLASH_VOLTAGE_RANGE_3;    //电压范围，VCC=2.7~3.6V之间!!
    f.NbSectors = 1;                           //一次只擦除一个扇区
#else
    f.TypeErase = FLASH_TYPEERASE_PAGES;//标明Flash执行页面只做擦除操作
    f.PageAddress = targetaddress; //要擦除的扇区起始地址
    f.NbPages = 1; //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值  一页1K大小
#endif


    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数

    if(  HAL_FLASHEx_Erase(&f, &PageError)!=HAL_OK)  //扇区擦除
    {

        return false;
    }




    return   FLASH_WaitForLastOperation(FLASH_WAITETIME)==HAL_OK; //等待上次操作完成
}






 

/*------------------Jyen--------------------------Jyen-----------------------*/
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
    HAL_FLASH_Unlock();

    //判断开始地址是否按照扇区对齐 否则会出错
    while((startPageAddr%FlashPageSize)!=0);
    //地址超出
    while(endAddr>FlashEndAddr);
    //TODO 跨页将产生一个全局警告方便debug
    u8 missionsRetriedCount=10;
    do {

        for (u32 i = startPageAddr; i <= endAddr; i += FlashPageSize)
        {
            if(! __JHAL_flashErasePage(i))//程序区页擦
            {
                continue;
            }
        }
        break;
    } while(--missionsRetriedCount!=0);

    HAL_FLASH_Lock();
    JHAL_enableInterrupts();


    if(missionsRetriedCount>0)
    {
        return true;
    } else {
        return false;
    }

}




/**
  * 描述  flash 程序区写单字数据
  * 输入  address： 指定flash地址
  *  p_FlashBuffer: 写入数据的指针
  *
  * 返回  无。
  * 注：写入前要先擦除地址所在的页  8字节对齐(uint64)  写入
  */
bool __JHAL_flashWrite8Byte(uint32_t address,void* p_FlashBuffer)
{

    if(HAL_OK==  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,address,* (u64_t*)p_FlashBuffer))
    {
        return true;
    }

    return false;

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
    leng=(leng/8)+(leng%8!=0);

    JHAL_disableInterrupts();

    for(u16 i=0; i<leng; i++) //一页1024byte，缓冲一次写64bit=8byte，128个缓冲块
    {
        if(!  __JHAL_flashWrite8Byte(address,p_FlashBuffer))
        {
            return false;
        }
        p_FlashBuffer+=8;
        address+=8;
    }
    JHAL_enableInterrupts();
    return true;
}
 



u32 JHAL_uidGetHigh()
{
    return HAL_GetUIDw2();
}
u32 JHAL_uidGetMiddle()
{
    return HAL_GetUIDw1();
}
u32 JHAL_uidGetLow()
{
    return HAL_GetUIDw0();
}


