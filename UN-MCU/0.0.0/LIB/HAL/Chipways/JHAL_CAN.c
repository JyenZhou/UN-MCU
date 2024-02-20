#include "../JHAL_CAN.h"
#include "xl_mcan.h"
#include "xl_nvic.h"
#define __JHAL_CAN_Exist


/**
 * 注意同一个型号的也要确定硬件上用的哪个引脚
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
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
 */
void __JHAL_canConfig(JHAL_CANBaudRate baudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{
    MCAN_InitTypeDef MCAN_InitStruct= {0};
    MCAN_FilterDef MCAN_FilterStruct;


//主频96M AHBCLK 48M  CAN=24M
    if(baudRate==JHAL_CANBaudRate1M)
    {
        MCAN_InitStruct.MCAN_BRP= 0x3;	//波特率分频( (3+1)分频对应1M  ->(7+1)分频 500K  (15+1)分频250K
        MCAN_InitStruct.MCAN_FDBRP= 0x03;	//波特率分频  + 1
    } else if(baudRate==JHAL_CANBaudRate500K)
    {
        MCAN_InitStruct.MCAN_BRP= 0x7;	//波特率分频( (3+1)分频对应1M  ->(7+1)分频 500K  (15+1)分频250K
        MCAN_InitStruct.MCAN_FDBRP= 0x07;	//波特率分频  + 1
    } else if(baudRate==JHAL_CANBaudRate250K)
    {
        MCAN_InitStruct.MCAN_BRP= 15;	//波特率分频( (3+1)分频对应1M  ->(7+1)分频 500K  (15+1)分频250K
        MCAN_InitStruct.MCAN_FDBRP= 15;	//波特率分频  + 1
    }

    MCAN_InitStruct.MCAN_SJW= MCAN_SJW_4tq;	//同步跳转宽度
    MCAN_InitStruct.MCAN_SAMP=MCAN_SAMPLE_1;	//采样 的样本数
    MCAN_InitStruct.MCAN_TSEG1=MCAN_TSEG1_3tq;	//时段1 4		位时间内确定 时钟周期和采样点
    MCAN_InitStruct.MCAN_TSEG2=MCAN_TSEG2_2tq;	//时段2 3  位时间 = （1+时段1+时段2) * 分频 / f [canclk]


    MCAN_Init(MCAN, &MCAN_InitStruct);	//先进入复位模式，配置好，进入工作模式



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
        // MCAN_FilterStruct.MCAN_IDAR = (((u32) filterRxIDList[0]<<21)&0xFFFF0000)|((((u32)filterRxIDList[0]<<21)|isExtendFrame|isRemoteFrame)&0xFFFF);                    //验收滤波
//设置屏蔽寄存器，这里当标识符寄存器用
        //MCAN_FilterStruct.MCAN_IDAR=(filterRxIDList[0]<<5<<16);
        MCAN_FilterStruct.MCAN_IDAR=filterRxIDList[0];
//设置标识符寄存器
        // MCAN_FilterStruct.MCAN_IDMR^=(filterRxIDList[0]<<3);		          //验收屏蔽
        // MCAN_FilterStruct.MCAN_IDMR=filterRxIDList[0];  // 验收码屏蔽， 1为忽略  上面配置过了0 所以一个id时候这样就行了
    }
    MCAN_FilterConfig(MCAN,&MCAN_FilterStruct);







}




/*
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
*/


bool JHAL_canOpen( JHAL_CAN  *config)
{
    if(!config->__info.isOpen) {
        __can=config;
        SIM_SCGC_Cmd(SIM_SCGC_MCAN,ENABLE);
        //CAN模块初始化，波特率,正常模式
        __JHAL_canConfig(config->baudRate,config->filterRxIDList,config->filterRxIDNumber);
        //使能CAN接收中断
        __JHAL_canRxITConfig();
        MCAN_Enable(MCAN, ENABLE); //mcan 使能
        MCAN_InterruptEn(MCAN,MCAN_ErrorWarningInterruptEn,ENABLE);
        MCAN_InterruptEn(MCAN,MCAN_ReceiveInterruptEn,ENABLE);
        MCAN_InterruptEn(MCAN,MCAN_DataOverrunInterruptEn,ENABLE);
        //发送中用到延时做超时判断等
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



//true发送成功  false发送失败
bool __JHAL_canSendData(JHAL_CANBox canBox)
{

    u8 timeoutMs=100;

    MCAN_MsgTypeDef	CAN_MessageStructure= {0};
    if(canBox.isRemoteFrame)
    {
        CAN_MessageStructure.MCAN_RTR = MCAN_RTR_Remote; //远程帧
    } else {
        CAN_MessageStructure.MCAN_RTR = MCAN_RTR_Data;		 	 // 数据帧
    }
    CAN_MessageStructure.MCAN_ID = canBox.id;
    if(canBox.isExtendFrame)
    {
        //扩展帧ID
        CAN_MessageStructure.MCAN_FF = MCAN_Extended_Frame;

    }
    else
    {   //标准帧
        CAN_MessageStructure.MCAN_FF = MCAN_Standard_Frame;
    }

    CAN_MessageStructure.MCAN_DLC = canBox.length;
    for(u8 i=0; i<canBox.length; i++)
    {
        CAN_MessageStructure.MCAN_Data[i] =canBox. data[i];				 //数据
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
    MCAN_LoadTransmitData(MCAN, &CAN_MessageStructure); //帧传输

    /* set the normal test mode */
    MCAN_SetMode(MCAN, MCAN_NORMALMODE);	//正常模式   MCAN_NORMALMODE

    /* set the transmission Request */
    MCAN_SetCmd(MCAN, MCAN_TransmissionRequest);	//传输请求  MCAN_TransmissionRequest

    for(u8 i=0; i<timeoutMs; i++)
        /*wait  SR.5=1 发送状态     发送缓冲区状态  wait SR.3=1 传输完成状态 */
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


//多帧数据赋值函数并发送数据
bool JHAL_canSendDatas(JHAL_CAN *can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback )
{

    u16 num=0,length= canBox.length;
    u8 remainder=canBox.length%8;
//添加个临时变量 退出前赋值回来 防止这里修改了指针指向外面无法使用canBox.data操作
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

/*在中断中接收一帧后缓存起来直到用户调用后才清除 主要是用于用户定期轮询
 * 返回值true值有效  false值无效
注意：1.用户若长时间没使用始终不会更新新来的值 2.不适用连续帧 及响应高的场景 需要自行从接收中断处理*/




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
//更新接收区的值当用户没有使用值时会保持旧值 与上面配合用
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
*@ 函数功能或简介:CAN接收回调函数  主要配合更新JHAL_canFrameRxBuffUpdate  若JHAL_canFrameRxBuffUpdate不满足要求用户程序中自行重写
 * @输入参数:   无
 * @返 回 值: 无
 * @备注: 电压跌落且回升后can有几率进入busoff， 此时CAN自动进入reset模式， 由于
6600的cantb和acr amr寄存器地址重合， reset模式下对cantb寄存器的操作会变
成对can过滤器寄存器的操作， 在处理busoff之前调用了发送，导致误写了acr
amr，然后再处理busoff，由于acr amr滤波的原因导致无法产生接收中断。
 *先使能错误警告中断
错误警告中断里面判断是否产生busoff
产生busoff后恢复成正常模式
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

