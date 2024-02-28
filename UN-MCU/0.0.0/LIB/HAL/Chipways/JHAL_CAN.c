#include "../JHAL_CAN.h"
#include "xl_mcan.h"
#include "xl_nvic.h"
#define __JHAL_CAN_Exist


/**
 * ע��ͬһ���ͺŵ�ҲҪȷ��Ӳ�����õ��ĸ�����
 *
 *
 *
 * */



JHAL_CAN *__can;







void __JHAL_canRxITConfig()
{

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = MCAN_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 canBaudRat:������
filterRxIDList��ֻ����ָ��ID���������ݵ����� ֧�ֶ�� ���16��  ��������һ��֡�ȹ�����Ҫ��Ŀ��������д
filterRxIDNumber�� ����֡�ĳ���  0������ �������16
 */
void __JHAL_canConfig(JHAL_CANBaudRate baudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{
    MCAN_InitTypeDef MCAN_InitStruct= {0};
    MCAN_FilterDef MCAN_FilterStruct;


//��Ƶ96M AHBCLK 48M  CAN=24M
    if(baudRate==JHAL_CANBaudRate1M)
    {
        MCAN_InitStruct.MCAN_BRP= 0x3;	//�����ʷ�Ƶ( (3+1)��Ƶ��Ӧ1M  ->(7+1)��Ƶ 500K  (15+1)��Ƶ250K
        MCAN_InitStruct.MCAN_FDBRP= 0x03;	//�����ʷ�Ƶ  + 1
    } else if(baudRate==JHAL_CANBaudRate500K)
    {
        MCAN_InitStruct.MCAN_BRP= 0x7;	//�����ʷ�Ƶ( (3+1)��Ƶ��Ӧ1M  ->(7+1)��Ƶ 500K  (15+1)��Ƶ250K
        MCAN_InitStruct.MCAN_FDBRP= 0x07;	//�����ʷ�Ƶ  + 1
    } else if(baudRate==JHAL_CANBaudRate250K)
    {
        MCAN_InitStruct.MCAN_BRP= 15;	//�����ʷ�Ƶ( (3+1)��Ƶ��Ӧ1M  ->(7+1)��Ƶ 500K  (15+1)��Ƶ250K
        MCAN_InitStruct.MCAN_FDBRP= 15;	//�����ʷ�Ƶ  + 1
    }

    MCAN_InitStruct.MCAN_SJW= MCAN_SJW_4tq;	//ͬ����ת���
    MCAN_InitStruct.MCAN_SAMP=MCAN_SAMPLE_1;	//���� ��������
    MCAN_InitStruct.MCAN_TSEG1=MCAN_TSEG1_3tq;	//ʱ��1 4		λʱ����ȷ�� ʱ�����ںͲ�����
    MCAN_InitStruct.MCAN_TSEG2=MCAN_TSEG2_2tq;	//ʱ��2 3  λʱ�� = ��1+ʱ��1+ʱ��2) * ��Ƶ / f [canclk]


    MCAN_Init(MCAN, &MCAN_InitStruct);	//�Ƚ��븴λģʽ�����úã����빤��ģʽ



    MCAN_FilterStruct.MCAN_DIDAR = 0xFFFFFFFF;
    MCAN_FilterStruct.MCAN_DIDMR = 0xFFFFFFFF;
    MCAN_FilterStruct.MCAN_FiltMode = MCAN_Single_Filter;
    MCAN_FilterStruct.MCAN_IDMode = MCAN_Standard_Frame;
    MCAN_FilterStruct.MCAN_IDAR = 0x0;
    MCAN_FilterStruct.MCAN_IDMR = 0xFFFFFFFF;
    //https://www.icode9.com/content-4-705458.html
//   u8 isExtendFrame=0;
//   u8 isRemoteFrame=0;
    if(filterRxIDNumber!=0)
    {
        MCAN_FilterStruct.MCAN_IDMR = 0x0;
        // MCAN_FilterStruct.MCAN_IDAR = (((u32) filterRxIDList[0]<<21)&0xFFFF0000)|((((u32)filterRxIDList[0]<<21)|isExtendFrame|isRemoteFrame)&0xFFFF);                    //�����˲�
//�������μĴ��������ﵱ��ʶ���Ĵ�����
        //MCAN_FilterStruct.MCAN_IDAR=(filterRxIDList[0]<<5<<16);
        MCAN_FilterStruct.MCAN_IDAR=filterRxIDList[0];
//���ñ�ʶ���Ĵ���
        // MCAN_FilterStruct.MCAN_IDMR^=(filterRxIDList[0]<<3);		          //��������
        // MCAN_FilterStruct.MCAN_IDMR=filterRxIDList[0];  // ���������Σ� 1Ϊ����  �������ù���0 ����һ��idʱ������������
    }
    MCAN_FilterConfig(MCAN,&MCAN_FilterStruct);







}




/*
 canBaudRat:������
filterRxIDList��ֻ����ָ��ID���������ݵ����� ֧�ֶ�� ���16��  ��������һ��֡�ȹ�����Ҫ��Ŀ��������д
filterRxIDNumber�� ����֡�ĳ���  0������ �������16
*/


bool JHAL_canOpen( JHAL_CAN  *config)
{
    if(!config->__info.isOpen) {
        __can=config;
        SIM_SCGC_Cmd(SIM_SCGC_MCAN,ENABLE);
        //CANģ���ʼ����������,����ģʽ
        __JHAL_canConfig(config->baudRate,config->filterRxIDList,config->filterRxIDNumber);
        //ʹ��CAN�����ж�
        __JHAL_canRxITConfig();
        MCAN_Enable(MCAN, ENABLE); //mcan ʹ��
        MCAN_InterruptEn(MCAN,MCAN_ErrorWarningInterruptEn,ENABLE);
        MCAN_InterruptEn(MCAN,MCAN_ReceiveInterruptEn,ENABLE);
        MCAN_InterruptEn(MCAN,MCAN_DataOverrunInterruptEn,ENABLE);
        //�������õ���ʱ����ʱ�жϵ�
        JHAL_delayOpen(*(JHAL_Delay *)NULL);
        return  config->__info.isOpen=true;
    }
    return false;
}
bool  JHAL_canClose(JHAL_CAN *can) {
    if( can->__info.isOpen) {
        SIM_SCGC_Cmd(SIM_SCGC_MCAN,DISABLE);
        MCAN_Enable(MCAN, DISABLE);
        MCAN_InterruptEn(MCAN,MCAN_ReceiveInterruptEn,DISABLE);
        MCAN_InterruptEn(MCAN,MCAN_DataOverrunInterruptEn,DISABLE);
        MCAN_InterruptEn(MCAN,MCAN_ErrorWarningInterruptEn,DISABLE);
        can->__info.isOpen=false;

        return  true;
    }
    return false;
}



//true���ͳɹ�  false����ʧ��
bool __JHAL_canSendData(JHAL_CANBox canBox)
{

    u8 timeoutMs=100;

    MCAN_MsgTypeDef	CAN_MessageStructure= {0};
    if(canBox.isRemoteFrame)
    {
        CAN_MessageStructure.MCAN_RTR = MCAN_RTR_Remote; //Զ��֡
    } else {
        CAN_MessageStructure.MCAN_RTR = MCAN_RTR_Data;		 	 // ����֡
    }
    CAN_MessageStructure.MCAN_ID = canBox.id;
    if(canBox.isExtendFrame)
    {
        //��չ֡ID
        CAN_MessageStructure.MCAN_FF = MCAN_Extended_Frame;

    }
    else
    {   //��׼֡
        CAN_MessageStructure.MCAN_FF = MCAN_Standard_Frame;
    }

    CAN_MessageStructure.MCAN_DLC = canBox.length;
    for(u8 i=0; i<canBox.length; i++)
    {
        CAN_MessageStructure.MCAN_Data[i] =canBox. data[i];				 //����
    }
    for(u8 i=0; i<timeoutMs; i++)
        /* CAN be write only when the transimit buffer released SR.2=1*/
    {
        if(MCAN_GetStatus(MCAN, MCAN_TransBufferStatus))
        {
            break;
        }
        JHAL_delayMs(1);
        if(i==timeoutMs-1)
        {
            return false;
        }

    }
    /* write the data to the buffer */
    MCAN_LoadTransmitData(MCAN, &CAN_MessageStructure); //֡����

    /* set the normal test mode */
    MCAN_SetMode(MCAN, MCAN_NORMALMODE);	//����ģʽ   MCAN_NORMALMODE

    /* set the transmission Request */
    MCAN_SetCmd(MCAN, MCAN_TransmissionRequest);	//��������  MCAN_TransmissionRequest

    for(u8 i=0; i<timeoutMs; i++)
        /*wait  SR.5=1 ����״̬     ���ͻ�����״̬  wait SR.3=1 �������״̬ */
    {
        if(!MCAN_GetStatus(MCAN, MCAN_TransmitStatus)&&MCAN_GetStatus(MCAN, MCAN_TransBufferStatus)&&MCAN_GetStatus(MCAN, MCAN_TransCompleteStatus))
        {
            break;
        }
        JHAL_delayMs(1);
        if(i==timeoutMs-1)
        {
            return false;
        }

    }

    return true;
}


//��֡���ݸ�ֵ��������������
bool JHAL_canSendDatas(JHAL_CAN *can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback )
{

    u16 num=0,length= canBox.length;
    u8 remainder=canBox.length%8;
//��Ӹ���ʱ���� �˳�ǰ��ֵ���� ��ֹ�����޸���ָ��ָ�������޷�ʹ��canBox.data����
    u8 *data= canBox.data;
    for(num = 0; num < length/8; num++)
    {
        canBox.length=8;
        if(  !__JHAL_canSendData(canBox))
        {
            canBox.data=data;
            return false;
        }
        canBox.data+=8;
        if(sendOneFrameCallback!=NULL)
        {
            sendOneFrameCallback();
        }

    }
    if(remainder != 0)
    {
        uint8 tData[8]= {0};
        for(uint8 i = 0; i<remainder; i++)
        {
            tData[i]=	canBox.data[i];
        }
        canBox.length=remainder;
        canBox.data=tData;
        if(! __JHAL_canSendData(canBox))
        {
            canBox.data=data;
            return false;
        }

    }
    canBox.data=data;
    return true;
}






u8 __canRxDataBuff[8] ;
JHAL_CANBox  __jcanRxBoxBuff= {.data=__canRxDataBuff}  ;

/*���ж��н���һ֡�󻺴�����ֱ���û����ú����� ��Ҫ�������û�������ѯ
 * ����ֵtrueֵ��Ч  falseֵ��Ч
ע�⣺1.�û�����ʱ��ûʹ��ʼ�ղ������������ֵ 2.����������֡ ����Ӧ�ߵĳ��� ��Ҫ���дӽ����жϴ���*/




u16 JHAL_canRxFinsh( JHAL_CAN * jacn)
{

    if(!__can-> __info.isRxFinsh)
    {
        return 0;
    }
    memcpy( jacn->rxDataBuff.data, __canRxDataBuff,jacn->rxDataBuff.length= __jcanRxBoxBuff.length);
    jacn->rxDataBuff.id=__jcanRxBoxBuff.id;
    jacn->rxDataBuff.isExtendFrame=__jcanRxBoxBuff.isExtendFrame;
    jacn->rxDataBuff.isRemoteFrame=__jcanRxBoxBuff.isRemoteFrame;

    __can-> __info.isRxFinsh=false;
    return jacn->rxDataBuff.length;

}
//���½�������ֵ���û�û��ʹ��ֵʱ�ᱣ�־�ֵ �����������
void __JHAL_canFrameRxBuffUpdate(JHAL_CANBox canBox)
{
    if(!__can-> __info.isRxFinsh)
    {
        __jcanRxBoxBuff.id=canBox.id;
        __jcanRxBoxBuff.isExtendFrame=canBox.isExtendFrame;
        __jcanRxBoxBuff.isRemoteFrame=canBox.isRemoteFrame;
        memcpy(__jcanRxBoxBuff.data, canBox.data, canBox.length );
        __jcanRxBoxBuff.length=canBox.length;
        __can-> __info.isRxFinsh=true;
    }

}

/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���:CAN���ջص�����  ��Ҫ��ϸ���JHAL_canFrameRxBuffUpdate  ��JHAL_canFrameRxBuffUpdate������Ҫ���û�������������д
 * @�������:   ��
 * @�� �� ֵ: ��
 * @��ע: ��ѹ�����һ�����can�м��ʽ���busoff�� ��ʱCAN�Զ�����resetģʽ�� ����
6600��cantb��acr amr�Ĵ�����ַ�غϣ� resetģʽ�¶�cantb�Ĵ����Ĳ������
�ɶ�can�������Ĵ����Ĳ����� �ڴ���busoff֮ǰ�����˷��ͣ�������д��acr
amr��Ȼ���ٴ���busoff������acr amr�˲���ԭ�����޷����������жϡ�
 *��ʹ�ܴ��󾯸��ж�
���󾯸��ж������ж��Ƿ����busoff
����busoff��ָ�������ģʽ
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/
void  MCAN_IRQHandler (void)
{
    u32 ir_reg = MCAN->CANIR;//get interrupt status reg value
    u32 sr_reg = MCAN->CANSR;//get status reg value
    if((ir_reg & 0x00000001) == 0x00000001)//receive frame
    {
        MCAN_MsgTypeDef rxMessage;

        rxMessage.MCAN_DLC = 0;
        MCAN_ReceiveData(MCAN, &rxMessage);


        JHAL_CANBox canBox= {0};
        canBox.isRemoteFrame=(bool)rxMessage.MCAN_RTR;

        canBox.id=rxMessage.MCAN_ID;

        canBox.data=rxMessage.MCAN_Data;
        canBox.length=rxMessage.MCAN_DLC;
        __JHAL_canFrameRxBuffUpdate(canBox);
        /* RX finished, return to Normal Operating Mode	*/
        MCAN_SetCmd(MCAN,MCAN_ReleaseReceiveBuffer); //Aaron he add 20180328
    } else if((ir_reg&0x00000004)==0x00000004) //warning error
    {
        if(((MCAN->CANSR&0x80)==0x80))//busoff
        {
            MCAN_SetMode(MCAN, MCAN_NORMALMODE);	//   MCAN_NORMALMODE
            MCAN_SetCmd(MCAN, MCAN_TransmissionRequest);	//  MCAN_TransmissionRequest

        }
    } else if((ir_reg & 0x00000008) == 0x00000008) //over run
    {
        if((sr_reg&0x00000010) != 0x00000010)
        {
            MCAN->CANCMR = 0x24;
            //MCAN_ReceiveData(MCAN, &RxMessage);
            MCAN->CANCMR = 0x28;
        }

    }


}

