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



#include "FlashSimulationEeprom.h"


//写入的地址（去掉了代码大小和起始地址的），写入类型,(FLASH_TYPEPROGRAM_HALFWORD,FLASH_TYPEPROGRAM_WORD,FLASH_TYPEPROGRAM_DOUBLEWORD)写入数据，写入的长度
void wirteFlash(uint32  writeFlashAddr,uint32_t TypeProgram,uint32 *writeFlashData,u16 NumToWrite)
{

    writeFlashAddr+= Robot_Num_Flash_Add;
    //1、解锁FLASH
    HAL_FLASH_Unlock();
    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;//标明Flash执行页面只做擦除操作
    f.PageAddress = writeFlashAddr;//声明要擦除的地址
    f.NbPages = 1; //说明要擦除的页数，此参数必须是Min_Data = 1和Max_Data =(最大页数-初始页的值)之间的值  一页1K大小
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);

    //3、对FLASH烧写	for(i=0;i<NumToWrite;i++)
    for(uint16 i=0; i<NumToWrite; i++)
    {
        HAL_FLASH_Program(TypeProgram,writeFlashAddr,writeFlashData[i]);

        if(TypeProgram==FLASH_TYPEPROGRAM_HALFWORD)
        {   //半字节-->uint16

            writeFlashAddr+=2;//地址增加2.
        } else if(TypeProgram==FLASH_TYPEPROGRAM_WORD)
        {   //字节-->uint32
            writeFlashAddr+=4;//地址增加4.
        } else if(TypeProgram==FLASH_TYPEPROGRAM_DOUBLEWORD)
        {   //大字节-->uint64
            writeFlashAddr+=8;
        }
    }
    //4、锁住FLASH
    HAL_FLASH_Lock();
}



//从指定地址开始读出指定长度的数据
//ReadAddr:读取地址 与写入的一致去掉了起始和代码大小
//pBuffer:数据接收缓冲
//NumToRead: 读取个数
void STMFLASH_Read_HALFWORD(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)
{   ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu16*)ReadAddr;//读取2个字节.

        ReadAddr+=2;//偏移2个字节.
    }
}
void STMFLASH_Read_WORD(u32 ReadAddr,u32 *pBuffer,u16 NumToRead)
{
    ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu32*)ReadAddr;//读取2个字节.

        ReadAddr+=4;//偏移2个字节.
    }
}
void STMFLASH_Read_DOUBLEWORD(u32 ReadAddr,u64 *pBuffer,u16 NumToRead)
{
    ReadAddr+= Robot_Num_Flash_Add;
    for(u16 i=0; i<NumToRead; i++)
    {
        pBuffer[i]=	 *(vu64*)ReadAddr;//读取2个字节.

        ReadAddr+=4;//偏移2个字节.
    }
}

/*存小数测试*/
//注意以下几点
//1.Flash内存范围是 0x0800 0000 ~ 0x0802 0000  ，
//2.要注意MCU一页的大小，一些MCU一页是1KB，一些MCU一页是2KB
//3.这里Flash写入的地址必须是偶数（ 如果是操作16位总线,那么访问时必须按偶数地址对齐。 否则就不一定受这样的限制。例如对串行Flash进行读操作是可以对奇地址进行单字节读取的。）
void test()
{
		float datai[5]={3.141592654,0.618,0x99,0x1000};
	float rdatai[5];
		//这里要注意写入类型//FLASH_TYPEPROGRAM_WORD
    wirteFlash(0x00,FLASH_TYPEPROGRAM_WORD,(uint32*)datai,4);
    HAL_Delay(1000);
    STMFLASH_Read_WORD(0x00,(uint32*)rdatai,5);
}
