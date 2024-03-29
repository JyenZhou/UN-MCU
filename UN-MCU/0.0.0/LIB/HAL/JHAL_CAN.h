/*************Jyen*************************Jyen********************Jyen***********************Jyen********************
   *                                                                *
 *                                                                   *
*
 		CAN默认通用HAL驱动
  使用CAN0 帧类型 默认数据帧非远程帧         若增加不同型号单片机 需要在该平台对应.c中增加相关引脚映射
*                                                                     *
  *                                                                 *
   *                                                              *
*********Jyen******************Jyen************************Jyen*****************************************/


#ifndef __JHAL_CAN__H__
#define __JHAL_CAN__H__
#include "JUTIL.h"
#ifdef __cplusPlus
extern "C" {
#endif




    typedef enum
    {
        JHAL_CANBaudRate125K=0,
        JHAL_CANBaudRate250K,
        JHAL_CANBaudRate500K,
        JHAL_CANBaudRate1M,
    } JHAL_CANBaudRate;

    typedef struct {
        bool isOpen:1;
        bool isRxFinsh:1;


    } __JHAL_canOtherInfo;



    typedef struct
    {
        u32 id;
        u8 *data;
        u16 length;
//  true为扩展帧 false为数据帧
        bool isExtendFrame:1;
//true为远程帧
        bool isRemoteFrame:1;
    } JHAL_CANBox;

    /*    canBaudRat:波特率
        filterRxIDList：只接受指定数据帧ID发来的数据的数组
        filterRxIDNumber： 过滤帧的长度*/
    typedef struct {
        u8 id:3;
        JHAL_CANBaudRate baudRate;
        u32  *filterRxIDList;
        u8 filterRxIDNumber;
        JHAL_CANBox rxDataBuff;

        __JHAL_canOtherInfo __info;
    } JHAL_CAN;

    /*CAN发送 支持多帧 注意  JHAL_CANBox中data实体需要与其作用域一致
    使用CAN0
    length：要发送的数据长度 大于8时自动多帧发送

    sendOneFrameCallback：发送完成1帧数据后的回调  多帧时可以在该函数中完成延时/喂狗等操作  不需要回调时传NULL


    void   canSendOneFrameCallback()
    {
    delayMsAdnFeedDog(5);
    }
    */
    bool JHAL_canSendDatas(JHAL_CAN *can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback );


    /* 使用默认配置需要注意使用的哪组CAN及引脚是否需要重映射 初始化配置 true 初始化使能  false反初始化不使能

    */
    bool JHAL_canOpen( JHAL_CAN *can);
    bool  JHAL_canClose(JHAL_CAN *can);
    /*在中断中接收一帧后缓存起来直到用户调用后才清除 主要是用于用户定期轮询
     * 返回值true值有效  false值无效
      canBox  通过指针带回来所赋值
    注意：1.用户若长时间没使用始终不会更新新来的值 2.不适用连续帧 及响应高的场景 需要自行从接收中断处理*/
    u16 JHAL_canRxFinsh(JHAL_CAN *can);



#ifdef CplusPlus
}
#endif

#endif








