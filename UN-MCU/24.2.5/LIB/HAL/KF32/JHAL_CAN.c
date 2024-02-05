#include "../JHAL_CAN.h"

    	/**
    	 * ע��ͬһ���ͺŵ�ҲҪȷ��Ӳ�����õ��ĸ�����
    	 *
    	 *
    	 *
    	 *
    	 *
    	 * /

/*MSP��س�ʼ�� can�����ض���*/
void JHAL_canMSPInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
#if defined(KF32A151MQV)
����δ�������õ�

    	/*����CAN��������Ϊ����ģʽ��������Ӧ�˿�ʱ��*/
    	GPIO_InitStructure.m_Mode = GPIO_MODE_RMP;
    	GPIO_InitStructure.m_Speed = GPIO_HIGH_SPEED;
    	GPIO_InitStructure.m_PullDown = GPIO_NOPULL;
    	GPIO_InitStructure.m_PullUp = GPIO_NOPULL;
    	GPIO_InitStructure.m_OpenDrain = GPIO_POD_PP;
    	GPIO_InitStructure.m_Pin = GPIO_PIN_MASK_13|GPIO_PIN_MASK_14;
    	GPIO_Configuration(GPIOD_SFR,&GPIO_InitStructure);
    	/*��������ӳ�书��ΪCANģʽ*/
    	PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIODCLKEN,TRUE);
    	GPIO_Pin_RMP_Config(GPIOD_SFR,GPIO_Pin_Num_13,GPIO_RMP_AF9_CAN1);
    	GPIO_Pin_RMP_Config(GPIOD_SFR,GPIO_Pin_Num_14,GPIO_RMP_AF9_CAN1);


#elif defined (KF32A140INP)

    	/*����CAN��������Ϊ����ģʽ��������Ӧ�˿�ʱ��*/
    	GPIO_InitStructure.m_Mode = GPIO_MODE_RMP;
    	GPIO_InitStructure.m_Speed = GPIO_HIGH_SPEED;
    	GPIO_InitStructure.m_PullDown = GPIO_NOPULL;
    	GPIO_InitStructure.m_PullUp = GPIO_NOPULL;
    	GPIO_InitStructure.m_OpenDrain = GPIO_POD_PP;
    	GPIO_InitStructure.m_Pin = GPIO_PIN_MASK_2|GPIO_PIN_MASK_3;
    	GPIO_Configuration(GPIOB_SFR,&GPIO_InitStructure);
    	/*��������ӳ�书��ΪCANģʽ*/
    	PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIODCLKEN,TRUE);
    	GPIO_Pin_RMP_Config(GPIOB_SFR,GPIO_Pin_Num_2,GPIO_RMP_AF9_CAN0);
    	GPIO_Pin_RMP_Config(GPIOB_SFR,GPIO_Pin_Num_3,GPIO_RMP_AF9_CAN0);

#else

    ��Ҫ��������ͺ�����ӳ��
#endif

}




/*============================================================================
 *void xINT_CAN(CAN_SFRmap* CANx)
------------------------------------------------------------------------------
����		: can�ж�����
����		: 1.CANx: ָ��CAN�ڴ�ṹ��ָ�룬ȡֵΪCAN0_SFR~CAN3_SFR
���		: ��
��ע		:
============================================================================*/
void JHAL_canRxITConfig()
{

	/* ���������ж� */
//	CAN_Set_INT_Enable(CANx,CAN_INT_TRANSMIT|CAN_INT_BUS_ERROR|CAN_INT_ARBITRATION_LOST,FALSE);
	/* ���������ж� */
	CAN_Set_INT_Enable(CAN0_SFR,CAN_INT_RECEIVE,TRUE);
	/* ����������ж� */
//	CAN_Set_INT_Enable(CANx,CAN_INT_DATA_OVERFLOW,TRUE);

		/* CAN�ж����� */
			INT_Interrupt_Priority_Config(INT_CAN0,3,1);						//CAN��ռ���ȼ�4,�����ȼ�0
			INT_Clear_Interrupt_Flag(INT_CAN0);									//CAN���жϱ�־
			INT_Interrupt_Enable(INT_CAN0,TRUE);								//CAN�ж�ʹ��
			INT_All_Enable (TRUE);

}

/*
 canBaudRat:������
filterRxIDList��ֻ����ָ��ID���������ݵ����� ֧�ֶ�� ���16��  ��������һ��֡�ȹ�����Ҫ��Ŀ��������д
filterRxIDNumber�� ����֡�ĳ���  0������ �������16
 */
void JHAL_canConfig(CAN_SFRmap* CANx,JHAL_CANBaudRate canBaudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{
	CAN_InitTypeDef CAN_InitStructure;
	/* CANʱ��ʹ�ܣ���λ���� */

	CAN_Reset(CANx);

	if(filterRxIDNumber!=0)

	{
		CAN_InitStructure.m_Acceptance = filterRxIDList[0];                    //�����˲�
		CAN_InitStructure.m_AcceptanceMask = 0xffffffff;		          //��������
	}

	CAN_InitStructure.m_WorkSource = CAN_SOURCE_HFCLK_DIV_2;      //CANʱ���ڲ���Ƶ

	if(canBaudRate==JHAL_CANBaudRate125K)
	{
		CAN_InitStructure.m_BaudRate = 7;					  //ʱ�ӷ�Ƶ  1M
	}
	else if(canBaudRate==JHAL_CANBaudRate250K)
	{
		CAN_InitStructure.m_BaudRate = 3;					  //ʱ�ӷ�Ƶ  2M
	}
	else if(canBaudRate==JHAL_CANBaudRate500K)
	{
		CAN_InitStructure.m_BaudRate = 1;					  //ʱ�ӷ�Ƶ  4M
	}
	else if(canBaudRate==JHAL_CANBaudRate1M)
	{
		CAN_InitStructure.m_BaudRate = 0;					  //ʱ�ӷ�Ƶ 8M
	}
	//TSEG1������TSEG2���ñ�ֵ75%�����ʣ�������8����������=��Seg1+Sync��/(Seg1+Seg2+Sync)*100%
	CAN_InitStructure.m_TimeSeg1 = 4;						   //TSEG1
	CAN_InitStructure.m_TimeSeg2 = 1;						   //TSEG2
	CAN_InitStructure.m_BusSample = CAN_BUS_SAMPLE_3_TIMES;	   //���������
	CAN_InitStructure.m_SyncJumpWidth = 1;					   //ͬ����ת���
	CAN_InitStructure.m_Enable = TRUE;						   //ʹ��
	CAN_InitStructure.m_Mode = CAN_MODE_NORMAL;				           //ģʽ
	CAN_Configuration(CANx,&CAN_InitStructure);			   //д������



}





/*Ĭ��ʹ��CAN0
 canBaudRat:������
filterRxIDList��ֻ����ָ��ID���������ݵ����� ֧�ֶ�� ���16��  ��������һ��֡�ȹ�����Ҫ��Ŀ��������д
filterRxIDNumber�� ����֡�ĳ���  0������ �������16
*/


__attribute__((weak)) void JHAL_canInit(bool  isEN,JHAL_CANBaudRate canBaudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{

    static bool sEN= {false};
    if(sEN!=isEN)
    {
        sEN=isEN;

        if(isEN)
        {
            JHAL_canMSPInit( );

            //CANģ���ʼ����������,����ģʽ
            JHAL_canConfig(CAN0_SFR,canBaudRate,filterRxIDList,filterRxIDNumber);
              //ʹ��CAN�����ж�
            JHAL_canRxITConfig();

        } else {

        	CAN_Reset(CAN0_SFR);
        }
    }



}




//true���ͳɹ�  false����ʧ��
bool JHAL_canSendData(JHAL_CANBox canBox)
{
	/* ��׼����֡  */

	CAN_MessageTypeDef	CAN_MessageStructure={0};

if(canBox.isRemoteFrame)
{
	CAN_MessageStructure.m_RemoteTransmit = CAN_REMOTE_FRAME; //Զ��֡
}else{
	CAN_MessageStructure.m_RemoteTransmit = CAN_DATA_FRAME;		 	 // ����֡
}

	if(canBox.isExtendFrame)
	{
		//��չ֡ID
		CAN_MessageStructure.m_FrameFormat = CAN_FRAME_FORMAT_EFF;
		CAN_MessageStructure.m_ExtendedID = canBox.id;
	}
	else
	{ //��׼֡
		CAN_MessageStructure.m_FrameFormat = CAN_FRAME_FORMAT_SFF;
		CAN_MessageStructure.m_StandardID = canBox.id;

	}

	CAN_MessageStructure.m_DataLength = canBox.length;
	for(u8 i=0;i<canBox.length;i++)
	{
		CAN_MessageStructure.m_Data[i] =canBox. data[i];				 //����
	}


	/* ���ͻ������� */
	if((!CAN_Get_Transmit_Status(CAN0_SFR,CAN_TX_BUFFER_STATUS)))
	{
		return false;
	}
	/* ת�����ݵ����ͻ����� */
	CAN_Transmit_Message_Configuration(CAN0_SFR,&CAN_MessageStructure);
	/* ���η��� */

	CAN0_SFR->CTLR = CAN0_SFR->CTLR | 0x300;


	return true;
}
//0���ͳɹ�  1����ʧ��
//��֡���ݸ�ֵ��������������
//��֡���ݸ�ֵ��������������
bool JHAL_canSendDatas(JHAL_CANBox canBox,FuncPtr  sendOneFrameCallback )
{

    u16 num=0,length= canBox.length;
    u8 remainder=canBox.length%8;

    for(num = 0; num < length/8; num++)
    {
				canBox.length=8;
        if( !JHAL_canSendData(canBox))
        {
            return false;
        }
		canBox.data+=8;
        sendOneFrameCallback();

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
        if( !JHAL_canSendData(canBox))
        {
            return false;
        }

    }
    return true;
}
static u8 rxDataBuff[8],rxDataBuff2[8];
static JHAL_CANBox jcanRxBoxBuff= {.data=rxDataBuff}, jcanRxBoxBuff2= {.data=rxDataBuff2};
bool  jcanRxFinsh=false;
/*���ж��н���һ֡�󻺴�����ֱ���û����ú����� ��Ҫ�������û�������ѯ
 * ����ֵtrueֵ��Ч  falseֵ��Ч
ע�⣺1.�û�����ʱ��ûʹ��ʼ�ղ������������ֵ 2.����������֡ ����Ӧ�ߵĳ��� ��Ҫ���дӽ����жϴ���*/
bool JHAL_canFrameGetRxBuff(JHAL_CANBox* canBox)
{
    if(!jcanRxFinsh)
    {
        return false;
    }
    memcpy(rxDataBuff2, rxDataBuff,jcanRxBoxBuff2.length= jcanRxBoxBuff.length);
    jcanRxBoxBuff2.id=jcanRxBoxBuff.id;
    jcanRxBoxBuff2.isExtendFrame=jcanRxBoxBuff.isExtendFrame;
    jcanRxBoxBuff2.isRemoteFrame=jcanRxBoxBuff.isRemoteFrame;
    *canBox=jcanRxBoxBuff2;
		jcanRxFinsh=false;
    return true;

}
//���½�������ֵ���û�û��ʹ��ֵʱ�ᱣ�־�ֵ �����������
void JHAL_canFrameRxBuffUpdate(JHAL_CANBox canBox)
{
    if(!jcanRxFinsh)
    {
    	jcanRxBoxBuff.id=canBox.id;
    	jcanRxBoxBuff.isExtendFrame=canBox.isExtendFrame;
    	jcanRxBoxBuff.isRemoteFrame=canBox.isRemoteFrame;
        memcpy(jcanRxBoxBuff.data, canBox.data, canBox.length );
        jcanRxBoxBuff.length=canBox.length;
        jcanRxFinsh=true;
    }

}

/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ �������ܻ���:CAN���ջص�����  ��Ҫ��ϸ���JHAL_canFrameRxBuffUpdate  ��JHAL_canFrameRxBuffUpdate������Ҫ���û�������������д
 * @�������:   ��
 * @�� �� ֵ: ��
 * @��ע: ��
 *
 *
 ******************************************************************************
 *
 *------------------Jyen-------------------------Jyen-------------------------*/
void __attribute__((interrupt))_CAN0_exception (void)
{
	volatile uint8_t i,j;
	uint32_t tmprmc,tmprmd;
	uint32_t bus_temp;
	static uint8_t Receice_addr=0x00;//����RAMƫ�Ƶ�ַ
	CAN_MessageTypeDef CAN_MessageStructrue;//���ձ��Ľṹ��

	/* �ж��Ƿ�ΪCAN���ͱ�־ */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT);

		CAN0_SFR->CTLR &= ~0x300;
	}

	/* �ж����ߴ����־ */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR);
		CAN0_SFR->CTLR &= ~0x300; //���ߴ����ط�
	}
	/* �ж��ٲñ�־ */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST);
		bus_temp = CAN0_SFR->RCR;//��ȡRCR��־λ��������һ���ٲñ�־λ
		CAN0_SFR->CTLR = CAN0_SFR->CTLR | 0x300;//�ٲ�ʧ���ط�
	}

	/* �ж��Ƿ�ΪCAN���ձ�־ */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_RECEIVE) != RESET)
	{
		tmprmc = (CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS;//��ǰ���䱨���������
		tmprmd =CAN0_SFR->RCR >>16;                //��A02�汾�д˹��ܣ���ȡ��һ������ָ���ַ
		if(tmprmd >=tmprmc)
			Receice_addr =(tmprmd -tmprmc)*0x10;
		else
			Receice_addr =(tmprmd +16 -tmprmc)*0x10;

		/* ����RAM���� */
		CAN_Receive_Message_Configuration(CAN0_SFR,Receice_addr,&CAN_MessageStructrue);

		/* �ͷ�һ�μ����� */

		j=30;
   		SFR_SET_BIT_ASM(CAN0_SFR->CTLR, CAN_CTLR_RELRX_POS);
   		while((tmprmc - ((CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS) != 1)&j)
   			j--;       //�ȴ�������־λ�ͷ���ɼ���ʱ�˳�
   		SFR_CLR_BIT_ASM(CAN0_SFR->CTLR, CAN_CTLR_RELRX_POS);

   		JHAL_CANBox canBox={0};
   		canBox.isRemoteFrame=(bool)CAN_MessageStructrue.m_RemoteTransmit;

   		if(canBox.isExtendFrame=(bool)CAN_MessageStructrue.m_FrameFormat)
   		{
   			canBox.id=CAN_MessageStructrue.m_ExtendedID;
   		}else{
   			canBox.id=CAN_MessageStructrue.m_StandardID;
   		}
   		canBox.data=CAN_MessageStructrue.m_Data;
   		canBox.length=CAN_MessageStructrue.m_DataLength;
   		JHAL_canFrameRxBuffUpdate(canBox);
	}

}

