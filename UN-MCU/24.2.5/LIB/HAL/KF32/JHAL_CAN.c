#include "../JHAL_CAN.h"

    	/**
    	 * 注意同一个型号的也要确定硬件上用的哪个引脚
    	 *
    	 *
    	 *
    	 *
    	 *
    	 * /

/*MSP相关初始化 can引脚重定义*/
void JHAL_canMSPInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
#if defined(KF32A151MQV)
引脚未对上配置的

    	/*配置CAN引脚类型为复用模式，开启对应端口时钟*/
    	GPIO_InitStructure.m_Mode = GPIO_MODE_RMP;
    	GPIO_InitStructure.m_Speed = GPIO_HIGH_SPEED;
    	GPIO_InitStructure.m_PullDown = GPIO_NOPULL;
    	GPIO_InitStructure.m_PullUp = GPIO_NOPULL;
    	GPIO_InitStructure.m_OpenDrain = GPIO_POD_PP;
    	GPIO_InitStructure.m_Pin = GPIO_PIN_MASK_13|GPIO_PIN_MASK_14;
    	GPIO_Configuration(GPIOD_SFR,&GPIO_InitStructure);
    	/*配置引脚映射功能为CAN模式*/
    	PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIODCLKEN,TRUE);
    	GPIO_Pin_RMP_Config(GPIOD_SFR,GPIO_Pin_Num_13,GPIO_RMP_AF9_CAN1);
    	GPIO_Pin_RMP_Config(GPIOD_SFR,GPIO_Pin_Num_14,GPIO_RMP_AF9_CAN1);


#elif defined (KF32A140INP)

    	/*配置CAN引脚类型为复用模式，开启对应端口时钟*/
    	GPIO_InitStructure.m_Mode = GPIO_MODE_RMP;
    	GPIO_InitStructure.m_Speed = GPIO_HIGH_SPEED;
    	GPIO_InitStructure.m_PullDown = GPIO_NOPULL;
    	GPIO_InitStructure.m_PullUp = GPIO_NOPULL;
    	GPIO_InitStructure.m_OpenDrain = GPIO_POD_PP;
    	GPIO_InitStructure.m_Pin = GPIO_PIN_MASK_2|GPIO_PIN_MASK_3;
    	GPIO_Configuration(GPIOB_SFR,&GPIO_InitStructure);
    	/*配置引脚映射功能为CAN模式*/
    	PCLK_CTL0_Peripheral_Clock_Enable(PCLK_CTL0_GPIODCLKEN,TRUE);
    	GPIO_Pin_RMP_Config(GPIOB_SFR,GPIO_Pin_Num_2,GPIO_RMP_AF9_CAN0);
    	GPIO_Pin_RMP_Config(GPIOB_SFR,GPIO_Pin_Num_3,GPIO_RMP_AF9_CAN0);

#else

    需要新增相关型号引脚映射
#endif

}




/*============================================================================
 *void xINT_CAN(CAN_SFRmap* CANx)
------------------------------------------------------------------------------
描述		: can中断配置
输入		: 1.CANx: 指向CAN内存结构的指针，取值为CAN0_SFR~CAN3_SFR
输出		: 无
备注		:
============================================================================*/
void JHAL_canRxITConfig()
{

	/* 开启发送中断 */
//	CAN_Set_INT_Enable(CANx,CAN_INT_TRANSMIT|CAN_INT_BUS_ERROR|CAN_INT_ARBITRATION_LOST,FALSE);
	/* 开启接收中断 */
	CAN_Set_INT_Enable(CAN0_SFR,CAN_INT_RECEIVE,TRUE);
	/* 开启包溢出中断 */
//	CAN_Set_INT_Enable(CANx,CAN_INT_DATA_OVERFLOW,TRUE);

		/* CAN中断配置 */
			INT_Interrupt_Priority_Config(INT_CAN0,3,1);						//CAN抢占优先级4,子优先级0
			INT_Clear_Interrupt_Flag(INT_CAN0);									//CAN清中断标志
			INT_Interrupt_Enable(INT_CAN0,TRUE);								//CAN中断使能
			INT_All_Enable (TRUE);

}

/*
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
 */
void JHAL_canConfig(CAN_SFRmap* CANx,JHAL_CANBaudRate canBaudRate,u32  *filterRxIDList,u8 filterRxIDNumber)
{
	CAN_InitTypeDef CAN_InitStructure;
	/* CAN时钟使能，复位禁能 */

	CAN_Reset(CANx);

	if(filterRxIDNumber!=0)

	{
		CAN_InitStructure.m_Acceptance = filterRxIDList[0];                    //验收滤波
		CAN_InitStructure.m_AcceptanceMask = 0xffffffff;		          //验收屏蔽
	}

	CAN_InitStructure.m_WorkSource = CAN_SOURCE_HFCLK_DIV_2;      //CAN时钟内部高频

	if(canBaudRate==JHAL_CANBaudRate125K)
	{
		CAN_InitStructure.m_BaudRate = 7;					  //时钟分频  1M
	}
	else if(canBaudRate==JHAL_CANBaudRate250K)
	{
		CAN_InitStructure.m_BaudRate = 3;					  //时钟分频  2M
	}
	else if(canBaudRate==JHAL_CANBaudRate500K)
	{
		CAN_InitStructure.m_BaudRate = 1;					  //时钟分频  4M
	}
	else if(canBaudRate==JHAL_CANBaudRate1M)
	{
		CAN_InitStructure.m_BaudRate = 0;					  //时钟分频 8M
	}
	//TSEG1设置与TSEG2设置比值75%采样率，采样点8个，采样率=（Seg1+Sync）/(Seg1+Seg2+Sync)*100%
	CAN_InitStructure.m_TimeSeg1 = 4;						   //TSEG1
	CAN_InitStructure.m_TimeSeg2 = 1;						   //TSEG2
	CAN_InitStructure.m_BusSample = CAN_BUS_SAMPLE_3_TIMES;	   //采样点次数
	CAN_InitStructure.m_SyncJumpWidth = 1;					   //同步跳转宽度
	CAN_InitStructure.m_Enable = TRUE;						   //使能
	CAN_InitStructure.m_Mode = CAN_MODE_NORMAL;				           //模式
	CAN_Configuration(CANx,&CAN_InitStructure);			   //写入配置



}





/*默认使用CAN0
 canBaudRat:波特率
filterRxIDList：只接受指定ID发来的数据的数组 支持多个 最大16个  其他例如一段帧等功能需要项目中自行重写
filterRxIDNumber： 过滤帧的长度  0不过滤 这里最大16
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

            //CAN模块初始化，波特率,正常模式
            JHAL_canConfig(CAN0_SFR,canBaudRate,filterRxIDList,filterRxIDNumber);
              //使能CAN接收中断
            JHAL_canRxITConfig();

        } else {

        	CAN_Reset(CAN0_SFR);
        }
    }



}




//true发送成功  false发送失败
bool JHAL_canSendData(JHAL_CANBox canBox)
{
	/* 标准数据帧  */

	CAN_MessageTypeDef	CAN_MessageStructure={0};

if(canBox.isRemoteFrame)
{
	CAN_MessageStructure.m_RemoteTransmit = CAN_REMOTE_FRAME; //远程帧
}else{
	CAN_MessageStructure.m_RemoteTransmit = CAN_DATA_FRAME;		 	 // 数据帧
}

	if(canBox.isExtendFrame)
	{
		//扩展帧ID
		CAN_MessageStructure.m_FrameFormat = CAN_FRAME_FORMAT_EFF;
		CAN_MessageStructure.m_ExtendedID = canBox.id;
	}
	else
	{ //标准帧
		CAN_MessageStructure.m_FrameFormat = CAN_FRAME_FORMAT_SFF;
		CAN_MessageStructure.m_StandardID = canBox.id;

	}

	CAN_MessageStructure.m_DataLength = canBox.length;
	for(u8 i=0;i<canBox.length;i++)
	{
		CAN_MessageStructure.m_Data[i] =canBox. data[i];				 //数据
	}


	/* 发送缓冲器空 */
	if((!CAN_Get_Transmit_Status(CAN0_SFR,CAN_TX_BUFFER_STATUS)))
	{
		return false;
	}
	/* 转载数据到发送缓冲器 */
	CAN_Transmit_Message_Configuration(CAN0_SFR,&CAN_MessageStructure);
	/* 单次发送 */

	CAN0_SFR->CTLR = CAN0_SFR->CTLR | 0x300;


	return true;
}
//0发送成功  1发送失败
//多帧数据赋值函数并发送数据
//多帧数据赋值函数并发送数据
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
/*在中断中接收一帧后缓存起来直到用户调用后才清除 主要是用于用户定期轮询
 * 返回值true值有效  false值无效
注意：1.用户若长时间没使用始终不会更新新来的值 2.不适用连续帧 及响应高的场景 需要自行从接收中断处理*/
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
//更新接收区的值当用户没有使用值时会保持旧值 与上面配合用
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
*@ 函数功能或简介:CAN接收回调函数  主要配合更新JHAL_canFrameRxBuffUpdate  若JHAL_canFrameRxBuffUpdate不满足要求用户程序中自行重写
 * @输入参数:   无
 * @返 回 值: 无
 * @备注: 无
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
	static uint8_t Receice_addr=0x00;//接收RAM偏移地址
	CAN_MessageTypeDef CAN_MessageStructrue;//接收报文结构体

	/* 判断是否为CAN发送标志 */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_TRANSMIT);

		CAN0_SFR->CTLR &= ~0x300;
	}

	/* 判断总线错误标志 */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_BUS_ERROR);
		CAN0_SFR->CTLR &= ~0x300; //总线错误不重发
	}
	/* 判断仲裁标志 */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST) != RESET)
	{
		CAN_Clear_INT_Flag(CAN0_SFR,CAN_INT_ARBITRATION_LOST);
		bus_temp = CAN0_SFR->RCR;//读取RCR标志位，作用下一次仲裁标志位
		CAN0_SFR->CTLR = CAN0_SFR->CTLR | 0x300;//仲裁失败重发
	}

	/* 判断是否为CAN接收标志 */
	if(CAN_Get_INT_Flag(CAN0_SFR,CAN_INT_RECEIVE) != RESET)
	{
		tmprmc = (CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS;//当前邮箱报文邮箱计数
		tmprmd =CAN0_SFR->RCR >>16;                //仅A02版本有此功能，读取下一个邮箱指针地址
		if(tmprmd >=tmprmc)
			Receice_addr =(tmprmd -tmprmc)*0x10;
		else
			Receice_addr =(tmprmd +16 -tmprmc)*0x10;

		/* 接收RAM数据 */
		CAN_Receive_Message_Configuration(CAN0_SFR,Receice_addr,&CAN_MessageStructrue);

		/* 释放一次计数器 */

		j=30;
   		SFR_SET_BIT_ASM(CAN0_SFR->CTLR, CAN_CTLR_RELRX_POS);
   		while((tmprmc - ((CAN0_SFR->CTLR&CAN_CTLR_CANRMC)>>CAN_CTLR_CANRMC0_POS) != 1)&j)
   			j--;       //等待计数标志位释放完成及超时退出
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

