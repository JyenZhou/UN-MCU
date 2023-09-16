#include "JHAL_SoftwareIIC.h"
#include "JHAL_Delay.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		模拟IIC通用通信

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
///////////////////局部方法/变量///////////////////////////////////////////////////



JHAL_IIC_InitTypeDef _iicInitStructList[JHAL_IIC_Number];

//IO模式设置
void __JHAL_sdaIOModeSet(u8  id,JHAL_IO_MODE mode)
{
    JHAL_gpioModeSet(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin,mode);
}

//设置IO电平
void __JHAL_sdaWritePin(u8 id,bool level)
{

    JHAL_gpioWitePin(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin,level);

}
//读取IO电平
bool  __JHAL_sdaReadPin(u8 id)
{

    return JHAL_gpioReadPin(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin);

}

//IO模式设置 0输入  1输出  2开漏
void __JHAL_sclIOModeSet(u8  id,JHAL_IO_MODE mode)
{
    JHAL_gpioModeSet(_iicInitStructList[id].sclPort,_iicInitStructList[id].sclPin,mode);
}

//设置IO状态 true高阻态/高电平  false低电平
void __JHAL_sclWritePin(u8 id,bool level)
{


    JHAL_gpioWitePin(_iicInitStructList[id].sclPort,_iicInitStructList[id].sclPin,level);

}



//////////////////////////////////////////////////////////////////////////////////


//产生IIC起始信号
void __JHAL_iicStart(u8 id)
{


    __JHAL_sdaWritePin(id,true);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sdaWritePin(id,false);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);



}
//产生IIC停止信号
void __JHAL_iicStop(u8 id)
{
    __JHAL_sdaWritePin(id,false);
    __JHAL_sclWritePin(id,false);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sdaWritePin(id,true);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);


}



//等待应答信号到来
//返回值：false，接收应答失败
//       true ，接收应答成功
bool __JHAL_iicWaitAck(u8 id)
{
    bool ret=true;
    __JHAL_sdaWritePin(id,true);
    if(_iicInitStructList[id].isIO_Mode_PP)
    {

        __JHAL_sdaIOModeSet(id,JHAL_IO_IN);
    }
    u16 ucErrTime=0;

    JHAL_delayUs(1*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(1*_iicInitStructList[id].delayMultiple);
    while(__JHAL_sdaReadPin(id))
    {
        if(ucErrTime++>500*_iicInitStructList[id].delayMultiple)
        {
            __JHAL_iicStop(id);
            ret=false;
            break;
        }
    }
    __JHAL_sclWritePin(id,false);
    if(_iicInitStructList[id].isIO_Mode_PP)
    {

        __JHAL_sdaIOModeSet(id,JHAL_IO_PP);
    }

    return ret;
}
//产生ACK应答
void __JHAL_iicSendAck(u8 id)
{
    __JHAL_sclWritePin(id,false);
    __JHAL_sdaWritePin(id,false);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);
}
//不产生ACK应答
void __JHAL_iicSendNAck(u8 id)
{
    __JHAL_sclWritePin(id,false);
    __JHAL_sdaWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void __JHAL_iicSendByte(u8 id, uint8_t txdata)
{

    u8 t;
    __JHAL_sclWritePin(id,false);
    for(t=0; t<8; t++)
    {

        __JHAL_sdaWritePin(id,(bool)(((txdata&0x80)>>7)!=0));
        JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
        __JHAL_sclWritePin(id,true);
        JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
        __JHAL_sclWritePin(id,false);
        if(_iicInitStructList[id].deviceType==JHAL_IIC_Device_TEA5767)
        {
            JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);//对TEA5767这三个延时都是必须的
        }
        txdata<<=1;
    }


}
//读1个字节，ack=true时，发送ACK，ack=false，发送nACK
u8 __JHAL_iicReadByte(u8 id,bool ack)
{
    u8 i,receive=0;
    if(_iicInitStructList[id].isIO_Mode_PP)
    {

        __JHAL_sdaIOModeSet(id,JHAL_IO_IN);
    }

    for(i=0; i<8; i++ )
    {
        __JHAL_sclWritePin(id,false);
        JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
        __JHAL_sclWritePin(id,true);
        receive<<=1;
        if(__JHAL_sdaReadPin(id)==true)
        {
            receive++;
        }
        JHAL_delayUs(1*_iicInitStructList[id].delayMultiple);
    }
    if (ack)
    {
        __JHAL_iicSendAck( id);
    }
    else
    {
        __JHAL_iicSendNAck( id);
    }
    if(_iicInitStructList[id].isIO_Mode_PP)
    {

        __JHAL_sdaIOModeSet(id,JHAL_IO_PP);
    }

    return receive;
}




//IIC初始化
void JHAL_iicInit(u8 id,JHAL_IICConfig config )
{
    /*
        static bool isFree=true;
        if(!isFree)
        {
            free(_iicInitStructList);
            _iicInitStructList = NULL;
        }
    //初始化为指向n个元素的数组
      //  _iicInitStructList = (JHAL_IIC_InitTypeDef*)malloc(sizeof(JHAL_IIC_InitTypeDef)*iicNumber);
        if (_iicInitStructList== NULL)
        {
    //内存申请失败 空间不足
            while(1);
        }
        isFree=false;

    //赋值
        *_iicInitStructList=*initStructList;
    */

    _iicInitStructList[id]=config;
    if(config.isIO_Mode_PP)
    {
        __JHAL_sclIOModeSet(id,JHAL_IO_PP);
        __JHAL_sdaIOModeSet(id,JHAL_IO_PP);
    } else {
        __JHAL_sdaIOModeSet(id,JHAL_IO_OD);
        __JHAL_sclIOModeSet(id,JHAL_IO_OD);

    }
    __JHAL_iicStop(id);




}







bool JHAL_iicMemWrite(u8 id, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize)
{

       dataSize++;
    if(memSizeIs16)
    {
        dataSize++;
    }
    u8 tdata[dataSize];

    if(memSizeIs16)
    {
        tdata[0]=(u8)(memAddress>>8);
        tdata[1]=(u8)(memAddress);
    } else {
        tdata[0]=(u8)(memAddress);
    }
    for(u8 i=1+(u8)memSizeIs16; i<dataSize; i++)
    {
        tdata[i]=pData[i];
    }


    return JHAL_iicTransmit(id,tdata,dataSize);
	

}


bool  JHAL_iicTransmit(u8 id, uint8_t *pData, u8 dataSize)
{
	 JHAL_disableInterrupts();
    __JHAL_iicStart(id);
    __JHAL_iicSendByte(id,_iicInitStructList[id].slaveAddress<<1);

    if(!__JHAL_iicWaitAck(id))
    {
					 JHAL_enableInterrupts();
        return false;
    }

    for(uint8 i=0; i<dataSize; i++)
    {
        __JHAL_iicSendByte(id,pData[i]);
        if(!__JHAL_iicWaitAck(id))
        {
							 JHAL_enableInterrupts();
            return false;
        }
    }
    __JHAL_iicStop(id);
			 JHAL_enableInterrupts();
    return true;
		

}


bool  JHAL_iicReceice(u8 id, uint8_t *pData,  u8 dataSize)
{
	 JHAL_disableInterrupts();
    __JHAL_iicStart(id);
    __JHAL_iicSendByte(id,_iicInitStructList[id].slaveAddress<<1);
    if(!__JHAL_iicWaitAck(id))
    {
				 JHAL_enableInterrupts();
        return false;
    }
    __JHAL_iicStart(id);
    __JHAL_iicSendByte(id,(_iicInitStructList[id].slaveAddress<<1)+1);

    if(!__JHAL_iicWaitAck(id))
    {
				 JHAL_enableInterrupts();
        return false;
    }
    for(uint8 i=0; i<dataSize-1; i++)
    {


        pData[i] = __JHAL_iicReadByte(id,true);

    }
    pData[dataSize-1]= __JHAL_iicReadByte(id,false);

    __JHAL_iicStop(id);
		 	 JHAL_enableInterrupts();
    return true;


}

bool JHAL_iicMemRead(u8 id, u16 memAddress,bool memSizeIs16,u8 *pData, u8 dataSize)
{
 JHAL_disableInterrupts();
    __JHAL_iicStart(id);
    __JHAL_iicSendByte(id,_iicInitStructList[id].slaveAddress);
    if(!__JHAL_iicWaitAck(id))
    {
        __JHAL_iicStop(id);
				 JHAL_enableInterrupts();
        return false;
    }
    if(memSizeIs16)
    {
        __JHAL_iicSendByte(id,(u8)(memAddress>>8));
        if(!__JHAL_iicWaitAck(id))
        {
            __JHAL_iicStop(id);
						 JHAL_enableInterrupts();
            return false;
        }

    }
    __JHAL_iicSendByte(id,(u8)memAddress);
    if(!__JHAL_iicWaitAck(id))
    {
        __JHAL_iicStop(id);
				 JHAL_enableInterrupts();
        return false;
    }


    __JHAL_iicStart(id);
    __JHAL_iicSendByte(id,(_iicInitStructList[id].slaveAddress<<1)+1);

    if(!__JHAL_iicWaitAck(id))
    {
        __JHAL_iicStop(id);
				 JHAL_enableInterrupts();
        return false;
    }
    for(uint8 i=0; i<dataSize-1; i++)
    {


        pData[i] = __JHAL_iicReadByte(id,true);

    }
    pData[dataSize-1]= __JHAL_iicReadByte(id,false);

    __JHAL_iicStop(id);
			 JHAL_enableInterrupts();
    return true;

}








