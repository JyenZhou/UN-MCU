#include "../JHAL_Flash.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		Flash默认通用HAL驱动  注意封装内部已经做了地址偏移   传进去的00地址就是falsh所在首地址
 		另外要注意大小不要超出  当然这里面也有防错机制 注意用的是自动生成器的值移植不同单片机时候需要修改自动生成的值或者新项目模板也是用自动生成器创建的

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/

 
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

        if((startPageAddr%EFLASH_PAGE_SIZE)!=0)
        {
//判断地址是否按照扇区对齐 否则会出错
            while(true);
        }
startPageAddr+=EFLASH_BASE_ADDRESS;
endAddr+=EFLASH_BASE_ADDRESS;
if(endAddr>EFLASH_END_ADDRESS)
{
//大小超出
while(1);
}



    uint32_t i;
    EFLASH_StatusType FLASHStatus = EFLASH_STATUS_SUCCESS;
    __set_PRIMASK(1);

    EFLASH_UnlockCtrl();
    for (i = startPageAddr; i <= endAddr; i += EFLASH_PAGE_SIZE)
    {
        FLASHStatus = EFLASH_PageErase(i);

        if (FLASHStatus != EFLASH_STATUS_SUCCESS)
        {
            EFLASH_LockCtrl();
            return	(bool)(FLASHStatus==EFLASH_STATUS_SUCCESS);
        }
    }
    EFLASH_LockCtrl();
    __set_PRIMASK(0);
    return (bool)(FLASHStatus==EFLASH_STATUS_SUCCESS);

}


/*写falsh
startAddr 起始地址
data 可以是结构体
size  数据大小多少个字节(u8)
isbefoErase 是否在写入前擦除 需要注意擦除是整页擦除 有些MCU需要是扇区首地址才能擦除
 */
void JHAL_flashWirte(u32 startAddr,u32 *data,u16 length,bool isbefoErase)
{
	if((((length+1)*4)+startAddr)>EFLASH_SIZE)
	{

		//flash大小超出
		while(1);

	}

startAddr+=EFLASH_BASE_ADDRESS;
 


    if(isbefoErase)
    {

        JHAL_flashErasePage(startAddr,startAddr+length);

    }
    u32  flashCrc=crc16_Modbus((u8*)data, length);
//  flash处理
    __set_PRIMASK(1);
    EFLASH_UnlockCtrl();

    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_WaitForLastOperation(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageErase(startAddr);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageProgram(startAddr, &flashCrc, 1);
    EFLASH_WaitEop(EFLASH_PAGE_ERASE_TIMEOUT);
    EFLASH_PageProgram(startAddr+4, data,length/ 4);
    EFLASH_LockCtrl();
    __set_PRIMASK(0);
}






bool JHAL_flashRead(u32 startAddr,u32 *data,u16 length)
{
	if((((length+1)*4)+startAddr)>EFLASH_SIZE)
	{

		//flash大小超出
		while(1);

	}
startAddr+=EFLASH_BASE_ADDRESS;
    u32  flashCrc=0;
    EFLASH_Read(startAddr,  &flashCrc, 1);
    EFLASH_Read(startAddr+4, data, length/ 4);
    uint16 crc= crc16_Modbus((u8*)data,length);
    if(flashCrc ==crc)
    {

        return true;
    } else {
        return false;

    }
}

