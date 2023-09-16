#include "JHAL_SoftwareIIC.h"
#include "JHAL_Delay.h"

/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		ģ��IICͨ��ͨ��

*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/
///////////////////�ֲ�����/����///////////////////////////////////////////////////



JHAL_IIC_InitTypeDef _iicInitStructList[JHAL_IIC_Number];

//IOģʽ����
void __JHAL_sdaIOModeSet(u8  id,JHAL_IO_MODE mode)
{
    JHAL_gpioModeSet(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin,mode);
}

//����IO��ƽ
void __JHAL_sdaWritePin(u8 id,bool level)
{

    JHAL_gpioWitePin(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin,level);

}
//��ȡIO��ƽ
bool  __JHAL_sdaReadPin(u8 id)
{

    return JHAL_gpioReadPin(_iicInitStructList[id].sdaPort,_iicInitStructList[id].sdaPin);

}

//IOģʽ���� 0����  1���  2��©
void __JHAL_sclIOModeSet(u8  id,JHAL_IO_MODE mode)
{
    JHAL_gpioModeSet(_iicInitStructList[id].sclPort,_iicInitStructList[id].sclPin,mode);
}

//����IO״̬ true����̬/�ߵ�ƽ  false�͵�ƽ
void __JHAL_sclWritePin(u8 id,bool level)
{


    JHAL_gpioWitePin(_iicInitStructList[id].sclPort,_iicInitStructList[id].sclPin,level);

}



//////////////////////////////////////////////////////////////////////////////////


//����IIC��ʼ�ź�
void __JHAL_iicStart(u8 id)
{


    __JHAL_sdaWritePin(id,true);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sdaWritePin(id,false);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);



}
//����IICֹͣ�ź�
void __JHAL_iicStop(u8 id)
{
    __JHAL_sdaWritePin(id,false);
    __JHAL_sclWritePin(id,false);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);
    __JHAL_sdaWritePin(id,true);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(4*_iicInitStructList[id].delayMultiple);


}



//�ȴ�Ӧ���źŵ���
//����ֵ��false������Ӧ��ʧ��
//       true ������Ӧ��ɹ�
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
//����ACKӦ��
void __JHAL_iicSendAck(u8 id)
{
    __JHAL_sclWritePin(id,false);
    __JHAL_sdaWritePin(id,false);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);
}
//������ACKӦ��
void __JHAL_iicSendNAck(u8 id)
{
    __JHAL_sclWritePin(id,false);
    __JHAL_sdaWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,true);
    JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);
    __JHAL_sclWritePin(id,false);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
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
            JHAL_delayUs(2*_iicInitStructList[id].delayMultiple);//��TEA5767��������ʱ���Ǳ����
        }
        txdata<<=1;
    }


}
//��1���ֽڣ�ack=trueʱ������ACK��ack=false������nACK
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




//IIC��ʼ��
void JHAL_iicInit(u8 id,JHAL_IICConfig config )
{
    /*
        static bool isFree=true;
        if(!isFree)
        {
            free(_iicInitStructList);
            _iicInitStructList = NULL;
        }
    //��ʼ��Ϊָ��n��Ԫ�ص�����
      //  _iicInitStructList = (JHAL_IIC_InitTypeDef*)malloc(sizeof(JHAL_IIC_InitTypeDef)*iicNumber);
        if (_iicInitStructList== NULL)
        {
    //�ڴ�����ʧ�� �ռ䲻��
            while(1);
        }
        isFree=false;

    //��ֵ
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








