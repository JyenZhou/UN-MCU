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


//����IICֹͣ�ź�
void __JHAL_i2csfStop(JHAL_I2CSF  *config)
{

    if(config->isIO_Mode_PP)
    {

        JHAL_gpioModeSet(config->sclPort,config->sclPin,JHAL_IO_PP_UP);

        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP_UP);
    } else {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_OD_UP);
        JHAL_gpioModeSet(config->sclPort,config->sclPin,JHAL_IO_OD_UP);

    }
		 /* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾSHT20_IIC����ֹͣ�ź� */
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,false); 
	  JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
 
  

  JHAL_enableInterrupts();
}


//IIC��ʼ��
void JHAL_i2csfOpen(JHAL_I2CSF  *config  )
{
    JHAL_delayOpen((JHAL_Delay) {
        NULL
    });



    __JHAL_i2csfStop(config);



}


//����IIC��ʼ�ź�
void __JHAL_i2csfStart(JHAL_I2CSF  *config)
{
	  JHAL_disableInterrupts();
/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾSHT20_IIC���������ź� */
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,false);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    JHAL_delayUs( config->delayMultiple);


}



 
 bool __JHAL_i2csfWaitAck(JHAL_I2CSF  *config)
{
    bool ret=true;
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_IN);

    }
   
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs( config->delayMultiple);
    if(JHAL_gpioReadPin(config->sdaPort,config->sdaPin))
    {
   
            return false;
 
    }
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP_UP);

    }
 JHAL_delayUs( config->delayMultiple);
    return ret;
}
//���� ACK/NACKӦ��

void __JHAL_i2csfSendAck(JHAL_I2CSF  *config,bool isAck)
{

    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,!isAck);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    JHAL_delayUs( config->delayMultiple);
    JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);

}

//IIC����һ���ֽ�
 
  
void __JHAL_i2csfSendByte(JHAL_I2CSF  *config, uint8_t txdata)
{

    u8 t;

    JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
    for(t=0; t<8; t++)
    {
  
        JHAL_gpioWitePin(config->sdaPort,config->sdaPin,(bool)( txdata & 0x80));
        JHAL_delayUs( config->delayMultiple);
        JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
        JHAL_delayUs( config->delayMultiple);
        JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
        if(config->deviceType==JHAL_I2CSF_Device_TEA5767)
        {
            JHAL_delayUs( config->delayMultiple);//��TEA5767��������ʱ���Ǳ����
        }
				 if (t == 7)
        {
          JHAL_gpioWitePin(config->sdaPort,config->sdaPin,true);
        }
        txdata<<=1;
				 JHAL_delayUs( config->delayMultiple);
    }
 
}


//��1���ֽڣ�ack=trueʱ������ACK;ack=false������nACK
 
 
u8 __JHAL_i2csfReadByte(JHAL_I2CSF  *config,bool ack)
{
    u8 i,receive=0;
    if(config->isIO_Mode_PP)
    {
        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_IN);

    }

    for(i=0; i<8; i++ )
    {
        receive<<=1;
        JHAL_gpioWitePin(config->sclPort,config->sclPin,true);
        JHAL_delayUs( config->delayMultiple);


        if(JHAL_gpioReadPin(config->sdaPort,config->sdaPin) )
        {
            receive++;
        }
        JHAL_gpioWitePin(config->sclPort,config->sclPin,false);
        JHAL_delayUs( config->delayMultiple);
    }

    if(config->isIO_Mode_PP)
    {

        JHAL_gpioModeSet(config->sdaPort,config->sdaPin,JHAL_IO_PP_UP);

    }

    __JHAL_i2csfSendAck( config,ack);


    return receive;
}
 

 


bool JHAL_i2csfMemWrite(JHAL_I2CSF  *config, u16 memAddress,bool memSizeIs16,  u8 *pData,  u8 dataSize)
{

    dataSize++;
    if(memSizeIs16)
    {
        dataSize++;
    }
    u8 tdata[dataSize];

    u8 index=0;
    if(memSizeIs16)
    {
        tdata[index++]=(u8)(memAddress>>8);
        tdata[index++]=(u8)(memAddress);
    } else {
        tdata[index++]=(u8)(memAddress);
    }
    for(u8 i=0; i<dataSize; i++)
    {
        tdata[index++]=pData[i];
    }


    return JHAL_i2csfTransmit(config,tdata,dataSize);


}


bool  JHAL_i2csfTransmit(JHAL_I2CSF  * config, uint8_t *pData, u8 dataSize)
{
  
    __JHAL_i2csfStart(config);
    __JHAL_i2csfSendByte(config,config->slaveAddress );

    if(!__JHAL_i2csfWaitAck(config))
    {
			  __JHAL_i2csfStop(config);
      
        return false;
    }

    for(uint8 i=0; i<dataSize; i++)
    {
        __JHAL_i2csfSendByte(config,pData[i]);
        if(!__JHAL_i2csfWaitAck(config))
        {
             __JHAL_i2csfStop(config);
            return false;
        }
    }
    __JHAL_i2csfStop(config);
  
    return true;


}



//isStop  д�Ĵ�����һ���ֹͣ����restart��   
bool JHAL_i2csfReceice(JHAL_I2CSF  * config, u8* memData,u8 memLength,bool isStop, u8 *readDataBuff, u8 dataSize)
{
   
    __JHAL_i2csfStart(config);
    __JHAL_i2csfSendByte(config,config->slaveAddress);
    if(!__JHAL_i2csfWaitAck(config))
    {
        __JHAL_i2csfStop(config);
       
        return false;
    }

    for(u8 i=0; i<memLength; i++)

    {
        __JHAL_i2csfSendByte(config, memData[i] );
        if(!__JHAL_i2csfWaitAck(config))
        {
            __JHAL_i2csfStop(config);
           
            return false;
        }
				if(isStop)
				{
					  __JHAL_i2csfStop(config);
				}

    }


    __JHAL_i2csfStart(config);
    __JHAL_i2csfSendByte(config, config->slaveAddress +1);

    if(!__JHAL_i2csfWaitAck(config))
    {
        __JHAL_i2csfStop(config);
        return false;
    }
    for(uint8 i=0; i<dataSize-1; i++)
    {


        readDataBuff[i] = __JHAL_i2csfReadByte(config,true);

    }
    readDataBuff[dataSize-1]= __JHAL_i2csfReadByte(config,false);

    __JHAL_i2csfStop(config);
   
    return true;


}
 //��װһ�����õĶ�8/16λ������ַ�ķ������ڲ���stop���ٶ� ����Ǳ�׼�����ֶ����ö���
bool JHAL_i2csfMemRead(JHAL_I2CSF  * config, u16 memAddress,bool memSizeIs16,u8 *pData, u8 dataSize)
{
    u8 memData[2];

    u8 index=0;
    if(memSizeIs16)
    {
        memData[index++]=(u8)(memAddress>>8);
        memData[index++]=(u8)(memAddress);
    } else {
        memData[index++]=(u8)(memAddress);
    }

		
  return  JHAL_i2csfReceice(config,memData,memSizeIs16+1,true,pData,dataSize);


      

}








