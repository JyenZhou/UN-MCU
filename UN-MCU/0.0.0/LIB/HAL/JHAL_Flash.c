 #ifdef JHAL_FlashStartAddr

//flash按也存的 我的数据就也按照页来 从最后一页起存
u32 __JHAL_flashPage2Addr(u16 page)
{

    //flash大小超出
    while(page>(JHAL_FlashMaxSize/JHAL_FlashPageSize)||page==0);

    return	JHAL_FlashEndAddr-page*JHAL_FlashPageSize;
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

bool JHAL_flashErasePages(uint32_t startPageAddr, uint32_t endAddr)
{
	   
    //判断开始地址是否按照扇区对齐 否则会出错
    if((startPageAddr%JHAL_FlashPageSize)!=0)
		{
				JHAL_error("JHAL_flashErasePages_Unaligned");
		}
    //地址超出
    if(endAddr>JHAL_FlashEndAddr)
		{
			JHAL_error("JHAL_flashErasePages_AddrOver");
		}
     
    u8 missionsRetriedCount=10;
    do {

        for (u32 i = startPageAddr; i <= endAddr; i += JHAL_FlashPageSize)
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
        while((length+8>JHAL_FlashPageSize));
        u32 startAddr=__JHAL_flashPage2Addr(page);
        if(!JHAL_flashErasePages(startAddr,startAddr+length+8))
        {
            continue;
        }
        u32  flashCrc=JHAL_crc(JHAL_CRC_Mode_16_Modbus,data, length);
        if(!JHAL_flashWriteNByte(startAddr,(u8*)&flashCrc,8))
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
    while((length+8>JHAL_FlashPageSize));
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



#endif


//96位UID相当于3个32bit位变量  每8bit位变量相当于一个字符  所以最大需要4*3+1 =13个字符空间 为了兼容 低位uid先存 
void uid2string(char* string,int buffSize )
{

    snprintf(string, buffSize, "%X%X%X",  JHAL_uidGetLow(), JHAL_uidGetMiddle(),JHAL_uidGetHigh());
 
 
}


