/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX 配置

不知为什么段1时间周期要大于段2时间周期


            16通道ADC1+DMA处理
						 ADC开始后 一定要延时再读取  不然第一次读的是空值  之后读的都是上一次值
*                                                                     *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

//使能CAN1的标准帧格式
//#define EN_CAN1_STDMODE
////使能CAN1的扩展帧格式
//#define EN_CAN1_EXTMODE
/*
 *  CAN 差分信号的表示
 *  1:隐性电平   H2.5v - L2.5v = 0v
 *  0:显性电平   H3.5v - L1.5v = 2v
 */


#include "Util_CAN.h"
#if defined(EN_CAN1_STDMODE) && defined(EN_CAN1_EXTMODE)
定义使用标准帧又同时定义使用扩展帧了
#endif

#if defined(EN_CAN1_STDMODE) || defined(EN_CAN1_EXTMODE)
CAN_TxHeaderTypeDef txHeader_can1;
uint32_t txMailbox_can1;

/**
  * 函数功能: CAN通信报文内容设置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */

void CAN1_SetTxMsg(void)
{

#ifdef EN_CAN1_EXTMODE
//扩展帧格式
    txHeader_can1.IDE=CAN_ID_EXT;

 
    txHeader_can1.ExtId=0x18ff7200;					   //使用的扩展ID


#else
//标准帧格式
    txHeader_can1.IDE = CAN_ID_STD;
    txHeader_can1.StdId=0x601;
    txHeader_can1.TransmitGlobalTime = DISABLE;
#endif

    //数据长度为 字节
    txHeader_can1.DLC=8;
//发送的是数据  帧类型 数据帧
    txHeader_can1.RTR=CAN_RTR_DATA;


}




void can1_sendData(uint8_t txData[])
{
    HAL_CAN_AddTxMessage(&hcan1, &txHeader_can1, txData, &txMailbox_can1);
}


#endif

void vApp_CAN1_Filter_Init(void);
void bisp_CANUtil_Init (void)
{
#if defined(EN_CAN1_STDMODE) || defined(EN_CAN1_EXTMODE)
 
    CAN1_SetTxMsg();
    /*-2- 初始化滤波器句柄 ------------------------------------------*/
   vApp_CAN1_Filter_Init();

    HAL_CAN_Start(&hcan1);
  /*-4- 使能中断通知 ----------------------------------------------*/
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
   HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_FULL);
#endif
}



uint8_t               rxData_can1[8];
CAN_RxHeaderTypeDef hCAN1_RxHeader ;



/*******************************************************************************
* Function Name  : HAL_CAN_RxFifo0MsgPendingCallback
* Description    : 消息接收回调函数
* Input          : hcan
* Output         : None
* Return         : None
****************************************************************************** */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hCAN1_RxHeader, rxData_can1) == HAL_OK)
    {
        HAL_CAN_AddTxMessage(hcan, &txHeader_can1, rxData_can1, &txMailbox_can1);
    }

}
void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{

    if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hCAN1_RxHeader, rxData_can1) == HAL_OK)
    {
        HAL_CAN_AddTxMessage(hcan, &txHeader_can1, rxData_can1, &txMailbox_can1);
    }

}



  /*******************************************************************************
* Function Name  : vApp_CAN1_Filter_Init 

“消息过滤器”  Message Filter [翻译为滤波器 误导人]
 就是 比如总线上有百十来台设备， 互相都在通信， 这样 我要是接到一条消息 都要看下 对我有没有用。
这样 软件耽误时间很长。 所以 用硬件 滤掉对自己无用的消息。
 只有跟自己相关的消息过来时候 才发生中断，通知接收。

* Description    : 初始化滤波器
* Input          : pFilter 滤波器句柄，初始化全部值
                                     IdHigh,
                   IdLow,
                   MaskIdHigh,
                   MaskIdLow,
                   FIFOAssignment,
                   Bank,
                   Mode,
                   Scale,
                   Activation,
                   SlaveStartFilterBank
* Output         : None
* Return         : None
****************************************************************************** */
void vApp_CAN1_Filter_Init()
{

CAN_FilterTypeDef  sFilterConfig;
 /* Configure the CAN Filter */
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
//此处选择fifo号要与其他地方一致
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;



HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
}



