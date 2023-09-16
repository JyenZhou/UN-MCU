/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *

CUBEMX ����

��֪Ϊʲô��1ʱ������Ҫ���ڶ�2ʱ������


            16ͨ��ADC1+DMA����
						 ADC��ʼ�� һ��Ҫ��ʱ�ٶ�ȡ  ��Ȼ��һ�ζ����ǿ�ֵ  ֮����Ķ�����һ��ֵ
*                                                                     *
  *                                                                 *
   *                                                              *
*********@����*Jyen******************@����*Jyen***********************@����*Jyen********************/

//ʹ��CAN1�ı�׼֡��ʽ
//#define EN_CAN1_STDMODE
////ʹ��CAN1����չ֡��ʽ
//#define EN_CAN1_EXTMODE
/*
 *  CAN ����źŵı�ʾ
 *  1:���Ե�ƽ   H2.5v - L2.5v = 0v
 *  0:���Ե�ƽ   H3.5v - L1.5v = 2v
 */


#include "Util_CAN.h"
#if defined(EN_CAN1_STDMODE) && defined(EN_CAN1_EXTMODE)
����ʹ�ñ�׼֡��ͬʱ����ʹ����չ֡��
#endif

#if defined(EN_CAN1_STDMODE) || defined(EN_CAN1_EXTMODE)
CAN_TxHeaderTypeDef txHeader_can1;
uint32_t txMailbox_can1;

/**
  * ��������: CANͨ�ű�����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */

void CAN1_SetTxMsg(void)
{

#ifdef EN_CAN1_EXTMODE
//��չ֡��ʽ
    txHeader_can1.IDE=CAN_ID_EXT;

 
    txHeader_can1.ExtId=0x18ff7200;					   //ʹ�õ���չID


#else
//��׼֡��ʽ
    txHeader_can1.IDE = CAN_ID_STD;
    txHeader_can1.StdId=0x601;
    txHeader_can1.TransmitGlobalTime = DISABLE;
#endif

    //���ݳ���Ϊ �ֽ�
    txHeader_can1.DLC=8;
//���͵�������  ֡���� ����֡
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
    /*-2- ��ʼ���˲������ ------------------------------------------*/
   vApp_CAN1_Filter_Init();

    HAL_CAN_Start(&hcan1);
  /*-4- ʹ���ж�֪ͨ ----------------------------------------------*/
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
   HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_FULL);
#endif
}



uint8_t               rxData_can1[8];
CAN_RxHeaderTypeDef hCAN1_RxHeader ;



/*******************************************************************************
* Function Name  : HAL_CAN_RxFifo0MsgPendingCallback
* Description    : ��Ϣ���ջص�����
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

����Ϣ��������  Message Filter [����Ϊ�˲��� ����]
 ���� �����������а�ʮ��̨�豸�� ���඼��ͨ�ţ� ���� ��Ҫ�ǽӵ�һ����Ϣ ��Ҫ���� ������û���á�
���� �������ʱ��ܳ��� ���� ��Ӳ�� �˵����Լ����õ���Ϣ��
 ֻ�и��Լ���ص���Ϣ����ʱ�� �ŷ����жϣ�֪ͨ���ա�

* Description    : ��ʼ���˲���
* Input          : pFilter �˲����������ʼ��ȫ��ֵ
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
//�˴�ѡ��fifo��Ҫ�������ط�һ��
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;



HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
}



