#include "../JHAL_Flash.h"

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


//扇区大小1K
#define PageSize 1024

#define FlashStartAddr 0x00000000

#if defined(KF32A151MQV)
//512K
#define FlashMaxSize 0x80000

#elif defined (KF32A140INP)
//128K
#define FlashMaxSize 0x20000
#else
需要为该型号单片机指定falsh大小
#endif

#define FlashEndAddr (FlashStartAddr+FlashMaxSize)




//flash按也存的 我的数据就也按照页来 从最后一页起存
u32 page2Addr(u8 page)
{
	if(page>FlashMaxSize/PageSize||page==0)
	{
		//flash大小超出
		while(1);
	}

return	FlashEndAddr-page*PageSize;
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
	//判断地址是否按照扇区对齐 否则会出错
	  while((startPageAddr%PageSize)!=0);

	if(endAddr>FlashEndAddr)
	{
		//flash大小不够
		while(1);

	}

	INT_All_Enable(FALSE);
    for (u32 i = startPageAddr; i <= endAddr; i += PageSize)
    {
         FLASH_Wipe_Configuration_RAM(FLASH_WIPE_CODE_PAGE,i);//程序区页擦
    }

    INT_All_Enable(TRUE);
    return true;

}




/**
  * 描述  flash 程序区写单字数据
  * 输入  address： 指定flash地址
  *  p_FlashBuffer: 写入数据的指针
  *
  * 返回  无。
  * 注：写入前要先擦除地址所在的页  8字节对齐  写入
  */
void JHAL_Write8Byte(uint32_t address,void* p_FlashBuffer)
{
	u32 baseAddr=address%16;
	u32 offsetAddr=address;
		//--------------------------------------------------------------------------------------------
		FLASH_ProgramTypeDef flash_write;
	    flash_write.m_Mode=FLASH_PROGRAM_WORD;//FLASH_PROGRAM_HALF_PAGE //FLASH_PROGRAM_WORD
		flash_write.m_Zone=FLASH_PROGRAM_CODE;//FLASH_PROGRAM_CFG  FLASH_PROGRAM_CODE
		flash_write.m_Addr=offsetAddr;
		flash_write.m_WriteSize=1;//半页编程不得为0，如果写1（+1）实际写的是2个双字 单字编程无影响
		flash_write.m_Data=p_FlashBuffer;
		INT_All_Enable(FALSE);
		FLASH_Program_Configuration_RAM(&flash_write);
		INT_All_Enable(TRUE);
}
/**
  * 描述  flash 程序区写多个字节数据
  * 输入  address： 指定flash地址
  *  p_FlashBuffer: 写入数据的指针
  *    leng：数据的长度
  * 返回  无。
  * 注：写入前要先擦除地址所在的页 //多出的不足8个字节按四个直接处理  因为是读数据所以后面指针超出没事  但是下次写入地址要隔开8
  */
void FLASH_WriteNByte(uint32_t address,uint8_t *p_FlashBuffer,uint16 leng)//地址必须为被8整除
{
	leng=(leng/8)+(leng%8!=0);
	for(u16 i=0;i<leng;i++)//一页1024byte，缓冲一次写64bit=8byte，128个缓冲块
	{
		JHAL_Write8Byte(address,p_FlashBuffer);//
		p_FlashBuffer+=8;
		address+=8;
	}
}
/*写falsh
startAddr 起始地址
data 可以是结构体
size  数据大小多少个字节(u8)
isbefoErase 是否在写入前擦除 需要注意擦除是整页擦除 有些MCU需要是扇区首地址才能擦除
length 数据长度  注意后面若接着写需要注意被覆盖假设前面用的长度是1-3  后面都要是从4（32位对齐）+4从（crc）即从8开始
 */

bool JHAL_flashWirte(u32 page,void *data,u16 length,bool isbefoErase)
{
	u8 missionsRetriedCount=3;
	do
	{
	//CRC占用1个
	if(length+1>PageSize)
	{
		//暂时只支持一页操作
				while(1);
	}
	u32 startAddr=page2Addr(page);
    if(isbefoErase)
    {
        JHAL_flashErasePage(startAddr,startAddr+length);
    }


    u32  flashCrc=crc16_Modbus(data, length+1);
    JHAL_Write8Byte(startAddr,&flashCrc);

	FLASH_WriteNByte(startAddr+8,data,length);//

	if(flashCrc==crc16_Modbus((u8 *)startAddr+8, length+1))
	{
	return true;
	}

	}while(missionsRetriedCount--!=0);

	return false;
}

/**
  * 描述  flash 程序区读多字节数据
  * 输入  address： 指定flash地址
  *  p_FlashBuffer: 写入数据的指针
  *
  * 返回  无。
  */
void FLASH_READCODE_fun(uint32_t address,uint32_t *p_FlashBuffer,uint32_t length)
{

	volatile uint32_t read_num;

	for(read_num=0;read_num<length;read_num++)
	{
		*p_FlashBuffer=Read_Flash_or_CFR_RAM(address,FLASH_PROGRAM_CODE);
		p_FlashBuffer++;
		address=address+0x0004;
	}
}



bool JHAL_flashRead(u32 page,void *data,u16 length)
{
	//CRC占用1个

	if(length*4+1>PageSize)
	{
		//暂时只支持一页操作
				while(1);
	}
	u32 startAddr=page2Addr(page);



/*
    u32  flashCrc=0;
    FLASH_READCODE_fun(startAddr,  &flashCrc, 1);
    FLASH_READCODE_fun(startAddr+=4, data, length/4);
    u8 overlength=length%4;
    if(overlength!=0)
    {
    	u8 databuff[4];
    	 FLASH_READCODE_fun(startAddr+length-overlength, (u32*)databuff,1 );

    	 for(u8 i=0;i<overlength;i++)
    	 {
    		 ((u8*)data)[length-overlength+i]=databuff[i];
    	 }
    }
    uint16 crc= crc16_Modbus(data,length+1);

    */
	u32  flashCrc=*((u32 *)startAddr);
	startAddr+=8;

for(u16 i=0;i<length;i++)
{
((u8*)data)[i]= *((u8 *)startAddr++);
}


uint16 crc= crc16_Modbus(data,length+1);
    if(flashCrc ==crc)
    {

        return true;
    } else {
        return false;

    }
}

