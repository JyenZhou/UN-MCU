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



//����IIC��ʼ�ź�
void __JHAL_iicStart(JHAL_IICConfig  *config)
{
    if(config->isIO_Mode_PP)
    {

        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP);


    }

    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);

    JHAL_delayUs(4*config->delayMultiple);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,false);
    JHAL_delayUs(4*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);



}
//����IICֹͣ�ź�
void __JHAL_iicStop(JHAL_IICConfig  *config)
{
    if(config->isIO_Mode_PP)
    {

        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP);


    }
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,false);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    JHAL_delayUs(4*config->delayMultiple);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs(4*config->delayMultiple);


}



//�ȴ�Ӧ���źŵ���
//����ֵ��false������Ӧ��ʧ��
//       true ������Ӧ��ɹ�
bool __JHAL_iicWaitAck(JHAL_IICConfig  *config)
{
    bool ret=true;
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_IN);

    }
    u32 ucErrTime=0;

    JHAL_delayUs(1*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs(1*config->delayMultiple);
    while(JHAL_gpioReadPin(config->sdaPort,config->sdaPin))
    {
        if(ucErrTime++>500*config->delayMultiple)
        {
            __JHAL_iicStop(config);
            ret=false;
            break;
        }
    }
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP);

    }

    return ret;
}
//����ACKӦ��
void __JHAL_iicSendAck(JHAL_IICConfig  *config)
{
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,false);
    JHAL_delayUs(2*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs(2*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
}
//������ACKӦ��
void __JHAL_iicSendNAck(JHAL_IICConfig  *config)
{
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    JHAL_delayUs(2*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs(2*config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void __JHAL_iicSendByte(JHAL_IICConfig  *config, uint8_t txdata)
{

    u8 t;
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    for(t=0; t<8; t++)
    {

        JHAL_gpioWitePin(config->sdaPort,config->sdaPin,(bool)(((txdata&0x80)>>7)!=0));
        JHAL_delayUs(2*config->delayMultiple);
        JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
        JHAL_delayUs(2*config->delayMultiple);
        JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
        if(config->deviceType==JHAL_IIC_Device_TEA5767)
        {
            JHAL_delayUs(2*config->delayMultiple);//��TEA5767��������ʱ���Ǳ����
        }
        txdata<<=1;
    }


}
//��1���ֽڣ�ack=trueʱ������ACK��ack=false������nACK
u8 __JHAL_iicReadByte(JHAL_IICConfig  *config,bool ack)
{
    u8 i,receive=0;
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_IN);

    }

    for(i=0; i<8; i++ )
    {
        JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
        JHAL_delayUs(2*config->delayMultiple);
        JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
        receive<<=1;
        if(JHAL_gpioReadPin(config->sdaPort,config->sdaPin) )
        {
            receive++;
        }
        JHAL_delayUs(1*config->delayMultiple);
    }
    if (ack)
    {
        __JHAL_iicSendAck( config);
    }
    else
    {
        __JHAL_iicSendNAck( config);
    }
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP);

    }

    return receive;
}




//IIC��ʼ��
void JHAL_iicInit(JHAL_IICConfig  *config  )
{


    if(config->isIO_Mode_PP)
    {

        JHAL_gpioModeSet(config->sclPort,config->sclPin,JHAL_IO_PP);

        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP);
    } else {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_OD);
        JHAL_gpioModeSet(config->sclPort,config->sclPin,JHAL_IO_OD);

    }
    __JHAL_iicStop(config);




}




bool JHAL_iicMemWrite(JHAL_IICConfig  *config, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize)
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


    return JHAL_iicTransmit(config,tdata,dataSize);


}


bool  JHAL_iicTransmit(JHAL_IICConfig  * config, uint8_t *pData, u8 dataSize)
{
    JHAL_disableInterrupts();
    __JHAL_iicStart(config);
    __JHAL_iicSendByte(config,config->slaveAddress );

    if(!__JHAL_iicWaitAck(config))
    {
        JHAL_enableInterrupts();
        return false;
    }

    for(uint8 i=0; i<dataSize; i++)
    {
        __JHAL_iicSendByte(config,pData[i]);
        if(!__JHAL_iicWaitAck(config))
        {
            JHAL_enableInterrupts();
            return false;
        }
    }
    __JHAL_iicStop(config);
    JHAL_enableInterrupts();
    return true;


}


bool  JHAL_iicReceice(JHAL_IICConfig  * config, uint8_t *pData,  u8 dataSize)
{
    JHAL_disableInterrupts();
    __JHAL_iicStart(config);
    __JHAL_iicSendByte(config,config->slaveAddress+1);
    if(!__JHAL_iicWaitAck(config))
    {
        JHAL_enableInterrupts();
        return false;
    }
    __JHAL_iicStart(config);
    __JHAL_iicSendByte(config,(config->slaveAddress<<1)+1);

    if(!__JHAL_iicWaitAck(config))
    {
        JHAL_enableInterrupts();
        return false;
    }
    for(uint8 i=0; i<dataSize-1; i++)
    {


        pData[i] = __JHAL_iicReadByte(config,true);

    }
    pData[dataSize-1]= __JHAL_iicReadByte(config,false);

    __JHAL_iicStop(config);
    JHAL_enableInterrupts();
    return true;


}

bool JHAL_iicMemRead(JHAL_IICConfig  * config, u16 memAddress,bool memSizeIs16,u8 *pData, u8 dataSize)
{
    JHAL_disableInterrupts();
    __JHAL_iicStart(config);
    __JHAL_iicSendByte(config,config->slaveAddress);
    if(!__JHAL_iicWaitAck(config))
    {
        __JHAL_iicStop(config);
        JHAL_enableInterrupts();
        return false;
    }
    if(memSizeIs16)
    {
        __JHAL_iicSendByte(config,(u8)(memAddress>>8));
        if(!__JHAL_iicWaitAck(config))
        {
            __JHAL_iicStop(config);
            JHAL_enableInterrupts();
            return false;
        }

    }
    __JHAL_iicSendByte(config,(u8)memAddress);
    if(!__JHAL_iicWaitAck(config))
    {
        __JHAL_iicStop(config);
        JHAL_enableInterrupts();
        return false;
    }


    __JHAL_iicStart(config);
    __JHAL_iicSendByte(config,(config->slaveAddress<<1)+1);

    if(!__JHAL_iicWaitAck(config))
    {
        __JHAL_iicStop(config);
        JHAL_enableInterrupts();
        return false;
    }
    for(uint8 i=0; i<dataSize-1; i++)
    {


        pData[i] = __JHAL_iicReadByte(config,true);

    }
    pData[dataSize-1]= __JHAL_iicReadByte(config,false);

    __JHAL_iicStop(config);
    JHAL_enableInterrupts();
    return true;

}








