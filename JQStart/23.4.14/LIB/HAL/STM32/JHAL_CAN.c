#include "../JHAL_CAN.h"










/*
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
*/


void JHAL_canInit(JHAL_CAN can,JHAL_CANConfig *config)
{


}
void  JHAL_canDeInit(JHAL_CAN can) {

}




/*在中断中接收一帧后缓存起来直到用户调用后才清除 主要是用于用户定期轮询
 * 返回值true值有效  false值无效
注意：1.用户若长时间没使用始终不会更新新来的值 2.不适用连续帧 及响应高的场景 需要自行从接收中断处理*/
bool JHAL_canGetRxBuff(JHAL_CAN can,JHAL_CANBox* canBox)
{
    return false;

}
bool JHAL_canSendDatas(JHAL_CAN can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback ) {
    return false;
}






