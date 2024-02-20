#include "../JHAL_LIN.h"

static  JHAL_LIN  *__linConfig[__UART_Number];
void __JHAL_linAbortTransmitIT (u8 id)
{
    UART_Type *uart=__JHAL_id2uart(id);
    UART_EnableInterruptCmd(uart, UART_THoldingEmptyIntEN,DISABLE);
    __linConfig[id]-> __linTxInfo.enable=false;

}



void __JHAL_linEnableReceiveIT (u8 id)
{
    UART_Type *uart=__JHAL_id2uart(id);
    __JHAL_uartReadBuffClean(uart);
    UART_EnableInterruptCmd(uart, UART_RLineStatusIntEN,ENABLE);
    UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN,ENABLE);
}
void JHAL_linEnableReceiveIT (JHAL_LIN *lin)
{

    if(  ! lin  ->__linRxInfo.enable)
    {
        lin->__linRxInfo.enable=true;
        __JHAL_linEnableReceiveIT(lin->id);
        lin->__linRxInfo.step = __JHAL_LIN_Start ;
    }
}
void __JHAL_linAbortReceiveIT (u8 id)
{
    UART_Type *uart=__JHAL_id2uart(id);
    __linConfig[ __JHAL_juart2Id(id)]-> __linRxInfo.enable=false;

    UART_EnableInterruptCmd(uart, UART_RLineStatusIntEN,DISABLE);
    UART_EnableInterruptCmd(uart, UART_RDataAvailableIntEN,DISABLE);
}
void JHAL_linAbortReceiveIT (JHAL_LIN *lin )
{

    if(   __linConfig[ __JHAL_juart2Id(lin->id)]-> __linRxInfo.enable)
    {
        __JHAL_linAbortReceiveIT(lin->id);
    }
}
bool  JHAL_linClose (JHAL_LIN *lin)
{
    if (lin->__info.uart.__info.isOpen) {
        JHAL_linAbortReceiveIT(lin );


        u8 id=__JHAL_juart2Id(lin->id);
        __JHAL_linAbortTransmitIT(id);
        JHAL_uartClose( &	 lin-> __info .uart);

        isAnalogLinMode[id]=false;


        return true;
    }

    return false;

}


bool JHAL_linOpen(JHAL_LIN    *config)
{
    if(!config->__info.uart.__info.isOpen) {
        UART_Type *  uart=__JHAL_juart2uart( config->id);
        u8 id=__JHAL_juart2Id( config->id);

        isAnalogLinMode[id]=true;
        config-> __info .uart.baudRate=config->baudRate;

        config-> __info .uart.id=config->id;

        UART_FIFOInitTypeDef UART_FIFOInitStruct;
        UART_FIFOInitStruct.UART_RT = UART_RxFIFO_1Char;
        UART_FIFOInitStruct.UART_TET = UART_TxFIFO_0Char;
        UART_FIFOInitStruct.UART_FIFO_Enable = DISABLE;
        UART_FIFOInit(uart, &UART_FIFOInitStruct);
        JHAL_uartOpen(  &config-> __info .uart);
        UART_LINBreakLengthConfig(uart, UART_LINBreakLength_13b);  //同步间隔至少13bit
        __JHAL_linAbortTransmitIT(id);
        __JHAL_linAbortReceiveIT(id);
//接收规则必须配置至少一项
        while(!config->rxConfig.filter.number);
        config->__linTxInfo.box=&config->__txBox;
        __linConfig[id]=config;

        return true;
    }

    return false;

}
//通过linID获取目标过滤器
JHAL_LinBox* __getfilterBoxByLinID(u8 id,u8 linID)
{
    for(u8 i=0; i<   __linConfig[id]->rxConfig.filter.number; i++)
    {
        if(linID==  __linConfig[id]->rxConfig.filter.boxs[i].id)
        {
            return &(  __linConfig[id]->rxConfig.filter.boxs[i]);
        }
    }
    return NULL;

}

#define BIT(A,B)      (((A)>>(B))&0x01)   /**< return bit has position \a B in byte \a A, A is the variable while */
/*LIN 极性校验*/
uint8_t __JHAL_linCalcParity(uint8_t id)
{
    uint8_t parity, p0,p1;
    parity=id;
    p0=(BIT(parity,0)^BIT(parity,1)^BIT(parity,2)^BIT(parity,4))<<6;     //偶校验位
    p1=(!(BIT(parity,1)^BIT(parity,3)^BIT(parity,4)^BIT(parity,5)))<<7;  //奇校验位
    parity|=(p0|p1);
    return parity;
}
/* LIN 帧校验*/
uint8_t __JHAL_linChecksum(uint8_t id, uint8_t *data,uint8_t length, JHAL_LIN_FrameClass frameClass)
{
    uint8_t i;
    uint32_t check_sum = 0;
    if(frameClass==JHAL_LIN_FrameClass_ENHANCED)
    {
        if(id == 0x3c || id == 0x3d)  //使用增强型校验
        {
            check_sum = 0;
        }
        else
        {
            check_sum = __JHAL_linCalcParity(id);
        }
    }
    for (i = 0; i < length; i++)
    {
        check_sum += *(data++);

        if (check_sum > 0xFF)          //进位
        {
            check_sum -= 0xFF;
        }
    }
    return (~check_sum);  //取反
}
bool JHAL_linSendData4IT(JHAL_LIN *lin,bool isMasterMode,JHAL_LinBox box )
{
    u8 id=  __JHAL_juart2Id( lin->id) ;
    UART_Type *uart=__JHAL_id2uart(id);
    if( !  __linConfig[id]->__linTxInfo.enable) {
        *__linConfig[id]-> __linTxInfo.box=box;
        __linConfig[id]-> __linTxInfo.isMasterMode=isMasterMode;
        if(isMasterMode)
        {
            __linConfig[id]->__linTxInfo.step =  __JHAL_LIN_Start;

        } else {
            __linConfig[id]->__linTxInfo.step =  __JHAL_LIN_Data;
        }
        __linConfig[id]-> __linTxInfo.count=0;
        __linConfig[id]->__linTxInfo.enable=true;
        UART_EnableInterruptCmd(uart, UART_THoldingEmptyIntEN,ENABLE);
        return true;
    }
    return false;
}
bool JHAL_linGetStatus4SendData4IT(JHAL_LIN *lin)
{
    u8 id=  __JHAL_juart2Id( lin->id) ;
    return !  __linConfig[id]->__linTxInfo.enable;
}
/*实际通过中断回调发送处理*/
void __JHAL_linInteruptSendHandler(UART_Type *uart,u8 id)
{
    uint8_t temp = 0;
    uint8_t checksum = 0;
    switch(  __linConfig[id]->__linTxInfo.step)
    {
    case __JHAL_LIN_Start:

        UART_SendLinBreak(uart);
        while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        temp = UART_ReceiveData(uart);
        if(temp != 0x00)
        {
            __JHAL_linAbortTransmitIT(id);
            //return LIN_BREAK_ERROR;
        }
        else
            __linConfig[id]->__linTxInfo.step = __JHAL_LIN_Sys;
        break;
    case __JHAL_LIN_Sys:
        UART_SendData(uart,0x55);
        while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        temp = UART_ReceiveData(uart);
        if(temp != 0x55)
        {
            // LIN_SYS_ERROR;
            __JHAL_linAbortTransmitIT(id);
        }
        else
            __linConfig[id]->__linTxInfo.step = __JHAL_LIN_PID;
        break;
    case __JHAL_LIN_PID:

        UART_SendData(uart,__JHAL_linCalcParity(  __linConfig[id]->	__linTxInfo.box->id));
        while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        temp = UART_ReceiveData(uart);
        if(temp != __JHAL_linCalcParity(  __linConfig[id]->__linTxInfo.box->id))
        {
            //	return LIN_PID_ERROR;
            __JHAL_linAbortTransmitIT(id);
        }
        else
            __linConfig[id]-> __linTxInfo.step = __JHAL_LIN_Data;
        break;

    case __JHAL_LIN_Data:
        UART_SendData(uart,  __linConfig[id]->__linTxInfo.box->data[  __linConfig[id]->__linTxInfo.count++] );
        while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        temp = UART_ReceiveData(uart);
        if(temp !=   __linConfig[id]-> __linTxInfo.box->data[  __linConfig[id]->__linTxInfo.count-1] )
        {
            __JHAL_linAbortTransmitIT(id);

            // LIN_DATA_ERROR;
        }
        else
        {
            if(  __linConfig[id]->__linTxInfo.count ==  __linConfig[id]->__linTxInfo.box->length)
                __linConfig[id]-> __linTxInfo.step = __JHAL_LIN_Check;
        }
        break;
    case __JHAL_LIN_Check:

        checksum = __JHAL_linChecksum(  __linConfig[id]->__linTxInfo.box->id, (u8*)( __linConfig[id]->__linTxInfo.box->data),  __linConfig[id]-> __linTxInfo.box->length,(JHAL_LIN_FrameClass)  (__linConfig[id]->__linTxInfo.box->frameclass));
        UART_SendData(uart,checksum);
        while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        temp = UART_ReceiveData(uart);
        if(temp != checksum)
        {
            __JHAL_linAbortTransmitIT(id);
            // LIN_CHECKSUM_ERROR;
        }
        else
        {
            __JHAL_linAbortTransmitIT(id);
            if(  __linConfig[id]->__linRxInfo.enable)
            {
                __JHAL_linEnableReceiveIT(id);
            }
            __linConfig[id]-> __linTxInfo.step = __JHAL_LIN_Finish;
        }
        break;
    default:
        __JHAL_linAbortTransmitIT(id);
        break;
    }
}
/*中断回调接收处理*/
void __JHAL_linRxSucceedHandler(u8 id,bool isMasterMode,JHAL_LIN_FrameClass frameClass )
{

    __JHAL_linAbortReceiveIT(id);
    __linConfig[id]->__linRxInfo.box->frameclass=frameClass;
    __linConfig[id]-> __linRxInfo.step = __JHAL_LIN_Finish ;
    if(   __linConfig[id]-> rxConfig.itCallback!=NULL)
    {
        __linConfig[id]-> rxConfig.itCallback(isMasterMode, *__linConfig[id]->__linTxInfo.box);
    }
}
void __JHAL_linInteruptReceiveHandler(bool isMasterMode,UART_Type *uart,u8 id)
{
    uint8_t temp,  frameclass = 0;

    switch(  __linConfig[id]->__linRxInfo.step)
    {
    case __JHAL_LIN_Start:
        if(isMasterMode)
        {
            __linConfig[id]->__linRxInfo.step =__JHAL_LIN_Start;
            UART_SendLinBreak(uart);
            while(!UART_GetLineStatus(uart,UART_LSRDataReady));
        }
        temp = UART_ReceiveData(uart);
        if(temp != 0x00)
        {
            //  LIN_BREAK_ERROR;
            __linConfig[id]-> __linRxInfo.step =__JHAL_LIN_Start;
        }
        else
        {
            __linConfig[id]-> __linRxInfo.count=0;
            __linConfig[id]-> __linRxInfo.step = __JHAL_LIN_Sys;
        }
        break;
    case __JHAL_LIN_Sys:
        temp = UART_ReceiveData(uart);
        if(temp != 0x55)
        {
            __linConfig[id]->  __linRxInfo.step =__JHAL_LIN_Start;;
            // LIN_SYS_ERROR;
        }
        else
        {
            // sLIN_Receive_Fram->Sys = temp;
            __linConfig[id]-> __linRxInfo.step = __JHAL_LIN_PID;
        }
        break;
    case __JHAL_LIN_PID:
        temp = UART_ReceiveData(uart);
        if(temp != __JHAL_linCalcParity(temp&0x3F))
        {
            __linConfig[id]-> __linRxInfo.step =__JHAL_LIN_Start;
            //  LIN_PID_ERROR;
        }
        else
        {
            // sLIN_Receive_Fram->PID = temp;
            u8 linID=temp & 0x3F;
            if((__linConfig[id]->__linRxInfo.box=__getfilterBoxByLinID(id,linID))==NULL)
            {
                __linConfig[id]->  __linRxInfo.step =__JHAL_LIN_Start;
                break;
            }

            if(!__linConfig[id]->__linRxInfo.box->isOnlyHead)
            {
                __linConfig[id]->  __linRxInfo.step = __JHAL_LIN_Data;
                break;
            }
            __JHAL_linAbortReceiveIT(id);
            __linConfig[id]->  __linRxInfo.step = __JHAL_LIN_Finish ;
            if(  __linConfig[id]->rxConfig.itCallback!=NULL)
            {
                __linConfig[id]->rxConfig.itCallback( isMasterMode, *__linConfig[id]->__linRxInfo.box);
            }
        }
        break;
    case __JHAL_LIN_Data:
        __linConfig[id]-> __linRxInfo.box->data[  __linConfig[id]->__linRxInfo.count++] = UART_ReceiveData(uart);

        if(  __linConfig[id]->__linRxInfo.count==__linConfig[id]->__linRxInfo.box->length)
        {
            __linConfig[id]-> __linRxInfo.step = __JHAL_LIN_Check;
        }
        break;
    case __JHAL_LIN_Check:


        temp = UART_ReceiveData(uart);
        frameclass= __linConfig[id]->__linRxInfo.box->frameclass|JHAL_LIN_FrameClass_CLASSIC;

        if(frameclass== __linConfig[id]->__linRxInfo.box->frameclass)
        {
            if(temp ==  __JHAL_linChecksum(  __linConfig[id]->__linRxInfo.box->id,(u8*)(  __linConfig[id]-> __linRxInfo.box->data), __linConfig[id]->__linRxInfo.box->length,JHAL_LIN_FrameClass_CLASSIC))
            {

                __JHAL_linRxSucceedHandler(id,isMasterMode,JHAL_LIN_FrameClass_CLASSIC);
                break;
            }
        }

        frameclass=__linConfig[id]->__linRxInfo.box->frameclass|JHAL_LIN_FrameClass_ENHANCED;
        if(frameclass==__linConfig[id]->__linRxInfo.box->frameclass)
        {
            if(temp ==  __JHAL_linChecksum(  __linConfig[id]->__linRxInfo.box->id,(u8*)(  __linConfig[id]-> __linRxInfo.box->data), __linConfig[id]->__linRxInfo.box->length,JHAL_LIN_FrameClass_ENHANCED))
            {
                __JHAL_linRxSucceedHandler(id,isMasterMode,JHAL_LIN_FrameClass_CLASSIC);
                break;
            }
        }

        //  LIN_CHECKSUM_ERROR;
        __linConfig[id]-> __linRxInfo.step =__JHAL_LIN_Start;
        break;

    default:
        __linConfig[id]->  __linRxInfo.step =__JHAL_LIN_Start;
        break;
    }
}



/*由串口中断回调*/
void __JHAL_uartITCallback4Lin(u8 id)
{

    uint8_t intFlags = 0;
    UART_Type *uart=__JHAL_id2uart(id);
    intFlags = UART_GetInterruptStatus(uart);
    if((intFlags & 0x0F)==UART_IID_TxHoldingEmpty)
    {
        if(  __linConfig[id]->__linTxInfo.enable) {
            if(  __linConfig[id]->__linTxInfo.isMasterMode&&  __linConfig[id]->__linTxInfo.box->isOnlyHead) {
//主机发送帧头
                __JHAL_linInteruptReceiveHandler(true,uart,id);

            } else {//发送数据
                __JHAL_linInteruptSendHandler(uart,id);
            }
        }
    }
    else if((intFlags & 0x0F) == UART_IID_RxLineStatus)
    {
        __linConfig[id]->__linRxInfo.step =__JHAL_LIN_Start;

        if(!UART_GetLineStatus(uart,UART_LSRBreakIntr))
        {
            __JHAL_uartReadBuffClean(uart);

        }


    }
    else if((intFlags & 0x0F) == UART_IID_RxDataAvailable)
    {

        if(  __linConfig[id]->__linRxInfo.enable&&!  __linConfig[id]-> __linTxInfo.enable) {
            __JHAL_linInteruptReceiveHandler(false,uart,id);
        } else {
            __JHAL_uartReadBuffClean(uart);
        }
    }
    else
    {

        __JHAL_uartReadBuffClean(uart);
    }

}










