#include "../JHAL_CAN.h"










/*
 canBaudRat:������
filterRxIDList��ֻ����ָ��ID���������ݵ����� ֧�ֶ�� ���16��  ��������һ��֡�ȹ�����Ҫ��Ŀ��������д
filterRxIDNumber�� ����֡�ĳ���  0������ �������16
*/


void JHAL_canInit(JHAL_CAN can,JHAL_CANConfig *config)
{


}
void  JHAL_canDeInit(JHAL_CAN can) {

}




/*���ж��н���һ֡�󻺴�����ֱ���û����ú����� ��Ҫ�������û�������ѯ
 * ����ֵtrueֵ��Ч  falseֵ��Ч
ע�⣺1.�û�����ʱ��ûʹ��ʼ�ղ������������ֵ 2.����������֡ ����Ӧ�ߵĳ��� ��Ҫ���дӽ����жϴ���*/
bool JHAL_canGetRxBuff(JHAL_CAN can,JHAL_CANBox* canBox)
{
    return false;

}
bool JHAL_canSendDatas(JHAL_CAN can,JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback ) {
    return false;
}






