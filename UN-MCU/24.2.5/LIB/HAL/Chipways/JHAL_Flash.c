#include "..\JHAL_Flash.h"
#include <xl_sim.h>
#include <xl_flash.h>
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




#if defined(XL6600A402L6)

//扇区大小1K
#define FlashPageSize FLASH_SECTOR_SIZE
//128K
#define FlashMaxSize  128*FlashPageSize

#define FlashStartAddr FLASH_START_ADDRESS

#elif defined (KF32A140INP)
//128K
#define FlashMaxSize 128*1024
#else
需要为该型号单片机指定falsh大小
#endif

#define FlashEndAddr (FlashStartAddr+FlashMaxSize)



//flash按也存的 我的数据就也按照页来 从最后一页起存
u32 __JHAL_flashPage2Addr(u16 page)
{   //flash大小超出
    while(page>(FlashMaxSize/FlashPageSize)||page==0);

    return	FlashEndAddr-page*FlashPageSize;
}

bool __JHAL_flashGetCmdFinshFlag(void)
{
    if((*(volatile uint32_t*)0x40020000 & FMC_FSTAT_CCIF_MASK) == FMC_FSTAT_CCIF_MASK)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool __JHAL_flashErasePage(const uint32_t targetaddress )
{
    uint32_t timeout=0xfffff;
    bool isOK=true;
    if( FLASH_ERR_SUCCESS!=FLASH_EraseSector(targetaddress))  //扇区擦除
    {

        return false;
    }
    JHAL_disableInterrupts();

    //FLASH_LaunchCMD();	//命令执行
    //FMC->FSTAT |= 0x80000000u;
    *(volatile uint32_t*)0x40020000 |= 0x80000000u;
    while(!__JHAL_flashGetCmdFinshFlag())
    {
        timeout--;
        if(timeout==0)
        {
            isOK = false;
            break;
        }
    }
    JHAL_enableInterrupts();
    return isOK;
}



bool __JHAL_flashWrite64Bit(const uint32_t TargetAddress, const uint32_t DwData0, const uint32_t DwData1)
{
    uint32_t timeout=0xfffff;
    bool isOK=true;


    if(FLASH_ERR_SUCCESS != FLASH_Program2LongWord( TargetAddress, DwData0, DwData1))//flash写8byte
    {
        return false;
    }
    else
    {
        JHAL_disableInterrupts();
        //FLASH_LaunchCMD();
        *(volatile uint32_t*)0x40020000 |= 0x80000000u;
        while(!__JHAL_flashGetCmdFinshFlag())
        {
            timeout--;
            if(timeout==0)
            {
                isOK=false;
                break;
            }
        }
        JHAL_enableInterrupts();
    }
    return isOK;
}





 


u32 JHAL_flashGetEndAddr()
{
    return FlashEndAddr;
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
    uint32_t DwData0=(uint32_t) (*((uint32_t*)(p_FlashBuffer)));
    uint32_t DwData1= (uint32_t)(*(((uint32_t*)(p_FlashBuffer))+1));
//外面没有做对齐FF处理 以空间换时间
//因为内部是const 当不对齐时若不用临时变量对齐当指针越界可能会导致硬件错误

    return  __JHAL_flashWrite64Bit(address,DwData0,DwData1);
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
    for(u16 i=0; i<leng; i++) //一页1024byte，缓冲一次写64bit=8byte，128个缓冲块
    {
        if(!  __JHAL_flashWrite8Byte(address,p_FlashBuffer))
        {
            return false;
        }
        p_FlashBuffer+=8;
        address+=8;
    }
    return true;
}
/*写falsh
startAddr 起始地址
data 可以是结构体
size  数据大小多少个字节(u8)
isbefoErase 是否在写入前擦除 需要注意擦除是整页擦除 有些MCU需要是扇区首地址才能擦除
length 数据长度  注意后面若接着写需要注意被覆盖假设前面用的长度是1-3  后面都要是从4（32位对齐）+4从（crc）即从8开始
重试次数10  错误后返回false
 */

bool JHAL_flashWirte(u32 page,void *data,u16 length)
{
    u8 missionsRetriedCount=10;
    do
    {
        //CRC占用1个   //暂时只支持一页操作
        while((length+8>FlashPageSize));
        u32 startAddr=__JHAL_flashPage2Addr(page);
        if(!JHAL_flashErasePage(startAddr,startAddr+length+8))
        {
            continue;
        }
        u32  flashCrc=JHAL_crc(JHAL_CRC_Mode_16_Modbus,data, length);
        if(!__JHAL_flashWrite8Byte(startAddr,&flashCrc))
        {
            continue;
        }
        if(!JHAL_flashWriteNByte(startAddr+8,data,length))
        {
            continue;
        }

        if(flashCrc!=JHAL_crc(JHAL_CRC_Mode_16_Modbus,(u8 *)startAddr+8, length))
        {
            continue;
        }
        break;
    } while(--missionsRetriedCount!=0);

    if(missionsRetriedCount>0)
    {
        return true;
    } else {
        return false;
    }

}



//默认通过指针直接读 方便兼容不同单片机

bool JHAL_flashRead(u32 page,void *data,u16 length)
{
    //CRC占用1个
    while((length+8>FlashPageSize));
    u32 startAddr=__JHAL_flashPage2Addr(page);
    u32  flashCrc=*((u32 *)startAddr);
    startAddr+=8;

    uint16 crc= JHAL_crc(JHAL_CRC_Mode_16_Modbus,(u8 *)startAddr,length);
    if(flashCrc ==crc)
    {
        for(u16 i=0; i<length; i++)
        {
            ((u8*)data)[i]= *((u8 *)startAddr++);
        }

        return true;
    } else {
        return false;

    }
}

