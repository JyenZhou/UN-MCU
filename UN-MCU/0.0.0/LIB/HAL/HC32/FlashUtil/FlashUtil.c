/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*             使用方法见下面测试程序                                                        *
            STM32内部flash模拟EEPROM读写
       可能程序版本较老  要手动 添加声明 void FLASH_PageErase(uint32_t PageAddress);
			 在  #include "stm32f1xx_hal_flash_ex.h" 中
*                                                                   *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/



#include "flashUtil.h"


uint32 Robot_Num_Flash_Add=0;

/*******************************************************************************
* FLASH 中断服务回调函数
******************************************************************************/
static volatile uint32_t u32FlashTestFlag   = 0;
void FlashCallback(void)
{
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);
        u32FlashTestFlag |= 0x01;
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        u32FlashTestFlag |= 0x02;
        Flash_DisableIrq(flash_int1);
    }

}

void flashUtilInit()
{
    Flash_Init(FlashCallback, 0);
//对齐flash地址 当代码大小传进来不是512的倍数强行进一位使它为521的倍数
    uint32 f=CodeSize/521;
    Robot_Num_Flash_Add=521*(f+1);

//不知道为什么还要+8  这是从内存中看出来的 推测的
    Robot_Num_Flash_Add+=8;

}


//写入的地址（去掉了代码大小和起始地址的），写入类型,(FLASH_TYPEPROGRAM_HALFWORD,FLASH_TYPEPROGRAM_WORD,FLASH_TYPEPROGRAM_DOUBLEWORD)写入数据，写入的长度
en_result_t wirteFlashWORD(uint32  writeFlashAddr,uint32 *writeFlashData,u16 NumToWrite,uint8 isSectorErase)
{
    en_result_t       enResult = Error;
    uint16 i;

    writeFlashAddr+= Robot_Num_Flash_Add;

    for(  i=0; i<NumToWrite; i++)
    {
//超出512个字节要再擦除要写的下一扇区

        if(isSectorErase)
        {
            //512/4=128 也就是说129次时要擦除新的地方了
            if(i%128==0)
            {
                Flash_SectorErase(writeFlashAddr);
            }
        }
//先对a取地址，然后把这个地址强转成uint32_t *类型，再解引用，此时编译器会按照整形的规则去解读这个地址的内容，
//主要为了能够存取float类型的值 否则直接写 Flash_WriteWord(writeFlashAddr,writeFlashData[i]);
        enResult = Flash_WriteWord(writeFlashAddr,*(volatile uint32*)&writeFlashData[i]);
        if (Ok != enResult)
        {
            return enResult;
        }
        //字节-->uint32
        writeFlashAddr+=4;//地址增加4.

    }
    enResult=Ok;
    return enResult;
}



//从指定地址开始读出指定长度的数据
//ReadAddr:读取地址 与写入的一致去掉了起始和代码大小
//pBuffer:数据接收缓冲
//NumToRead: 读取个数
void STMFLASH_Read_HALFWORD(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for( i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu16*)ReadAddr;//读取2个字节.

        ReadAddr+=2;//偏移2个字节.
    }
}



void STMFLASH_Read_WORD(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for(i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu32*)ReadAddr;//读取2个字节.

        ReadAddr+=4;//偏移2个字节.
    }
}
void STMFLASH_Read_WORD_Float(u32 ReadAddr,float *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for(i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile float*)ReadAddr;//读取2个字节.

        ReadAddr+=4;//偏移2个字节.
    }
}
void STMFLASH_Read_DOUBLEWORD(u32 ReadAddr,u64 *pBuffer,u16 NumToRead)
{   u16 i;
    ReadAddr+= Robot_Num_Flash_Add;
    for( i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(volatile vu64*)ReadAddr;//读取2个字节.

        ReadAddr+=4;//偏移2个字节.
    }
}

/*存小数测试*/
//注意以下几点
//1.Flash内存范围是 0x0800 0000 ~ 0x0802 0000  ，
//2.要注意MCU一页的大小，一些MCU一页是1KB，一些MCU一页是2KB
//3.这里Flash写入的地址必须是偶数（ 如果是操作16位总线,那么访问时必须按偶数地址对齐。 否则就不一定受这样的限制。例如对串行Flash进行读操作是可以对奇地址进行单字节读取的。）
//void test()
//{
//		float datai[5]={3.141592654,0.618,0x99,0x1000};
//	float rdatai[5];
//		//这里要注意写入类型//FLASH_TYPEPROGRAM_WORD
//  //  wirteFlash(0x00,FLASH_TYPEPROGRAM_WORD,(uint32*)datai,4);
////HAL_Delay(1000);
////    STMFLASH_Read_WORD(0x00,(uint32*)rdatai,5);
//}


