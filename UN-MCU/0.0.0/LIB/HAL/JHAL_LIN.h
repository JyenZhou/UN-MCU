#ifndef __JHAL_LIN__H__
#define __JHAL_LIN__H__
#include "JUTIL.h"
#ifdef __cplusPlus
extern "C" {
#endif

#include "JHAL_Uart.h"


    typedef enum
    {
        JHAL_LIN_FrameClass_CLASSIC=0x01,
        JHAL_LIN_FrameClass_ENHANCED=0x02
    } JHAL_LIN_FrameClass;


    typedef struct
    {

        u8 id ;
//是否只是帧头 接收时判断 发送无效
        bool isOnlyHead:1;
//  JHAL_LIN_FrameClass 接收数据过滤时允许多种校验 直接或上即可  注意为了防误触这里每次上电后走通一个协议后就不再支持多协议
        //接收头模式无法自动设置
        u8 frameclass;
        u8 data[8];
//数据长度<=8
        u8 length;
    } JHAL_LinBox;

    typedef struct
    {

//data 无效  length在帧头模式下无效   JHAL_LIN_FrameClass允许多种校验时直接或上
        JHAL_LinBox *boxs;
//长度为0接收所有ID  其他长渡只接受该ID
        u8 number;
    }
    JHAL_LinFilter;

    typedef void         (*JHAL_LinCallback)(bool isMasterMode,JHAL_LinBox box );


//Lin的传输步骤
    typedef enum
    {
        __JHAL_LIN_Start=0,
        __JHAL_LIN_Sys,
        __JHAL_LIN_Head,
        __JHAL_LIN_PID,
        __JHAL_LIN_Data,
        __JHAL_LIN_Check,
        __JHAL_LIN_Finish,
    } __JHAL_LIN_Step;

    typedef struct
    {
        JHAL_LinBox *box;
//是否使能
        volatile    bool enable:1;
//记数
        u8 count;
//当前传输的步骤
//主机模式
        bool isMasterMode;
        volatile   __JHAL_LIN_Step step;
    }  __JHAL_Lin_RxTxInfo;

    typedef struct
    {
        JHAL_LinFilter filter;
//单帧接收完成中断回调； */
        JHAL_LinCallback itCallback;


    } JHAL_LIN_RXConfig;


    typedef struct {
        JHAL_UART uart ;

    } __JHAL_LIN_therInfo;


    typedef struct {
        u8 id:3;
        uint32_t baudRate;
        JHAL_LIN_RXConfig rxConfig;

        __JHAL_Lin_RxTxInfo __linRxInfo;
        __JHAL_Lin_RxTxInfo __linTxInfo;
        JHAL_LinBox __txBox;
        __JHAL_LIN_therInfo __info;


    } JHAL_LIN;



    /*单帧中断发送； 返回false代表上一组还未发送完；  masterMode是主机模式发送:  主机发送头模式接收的数据在回调中 */
    bool JHAL_linSendData4IT(JHAL_LIN *lin,bool isMasterMode,JHAL_LinBox box );
//获取中断发送状态 true发送已空闲
    bool JHAL_linGetStatus4SendData4IT(JHAL_LIN *lin);


    bool JHAL_linOpen(JHAL_LIN *lin);

    bool  JHAL_linClose(JHAL_LIN *lin);

//接收中断单帧使能 接收信息在JHAL_linInit的config中指定
    void JHAL_linEnableReceiveIT (JHAL_LIN *lin);
//立即失能接收
    void  JHAL_linAbortReceiveIT (JHAL_LIN *lin);




#ifdef CplusPlus
}
#endif

#endif








