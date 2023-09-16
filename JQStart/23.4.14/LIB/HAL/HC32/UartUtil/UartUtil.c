#include "uartUtil.h"

#define     T1_PORT                 (3)
#define     T1_PIN                  (3)


//�ѽ��յ��ĳ��ȼ�¼
uint16 volatile Uart1_ReceivingCount=0;
//���ձ�־λ  ������ɺ� ��1  Ҳ���ǰ������־λ����while����  ����Ϊ1��ʱ��Ϳ���ִ�н��RecData��
unsigned char volatile RecEnd_Uart1=0;
//���տ�ʼ��־λ
unsigned char volatile RecStart_Uart1=0;
//���ڷ���״̬  ��Ҫ�������� �������ж�ģʽʱ �ҵĴ��ڷ���ʱ��Ȼ�ر��˽��� ������ʱ���ڽ����� �ᵼ�´�����Ѵ����ִ����������²�һ��
unsigned char volatile Sendingstatus_Uart1=0;


//����Ҫ�Ĳ��������ջ����� ���͹����ı𳬳�������������С������ڽ���ʱ�����һ���ж�
unsigned char   ReceiveData_Uart1[Uart1_ReceivingLength]= {0};

void Uart1_RxIntCallback(void)
{
//Uart_SendData(UARTCH1,M0P_UART1->SBUF);
    if(RecEnd_Uart1==1)
        return;
    uint8 aRxBuffer_UART1=M0P_UART1->SBUF;
    if(aRxBuffer_UART1==0xFF&&RecStart_Uart1==0) //'FF',start recevie package
    {   Uart1_ReceivingCount = 0;
        RecStart_Uart1 = 1;
        ReceiveData_Uart1[Uart1_ReceivingCount++] = aRxBuffer_UART1;
    }
    else  if(RecStart_Uart1) {

        ReceiveData_Uart1[Uart1_ReceivingCount++] = aRxBuffer_UART1;
        if(Uart1_ReceivingCount==Uart1_ReceivingLength)
        {
            //��־�������
            RecEnd_Uart1 = 1;
            Uart1_ReceivingCount = 0;
            RecStart_Uart1 = 0;


        }


    }



}





void ErrIntCallback(void)
{

}


void uartUtilinit()
{

    uint16_t timer=0;
    uint32_t pclk=0;

    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;


    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);


//    Gpio_InitIO(T1_PORT,T1_PIN,GpioDirIn);
//    Gpio_InitIO(0,3,GpioDirOut);
//    Gpio_SetIO(0,3,1);

    Gpio_InitIOExt(0,1,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
    Gpio_InitIOExt(0,2,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
//Gpio_InitIOExt(3,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE);
//    Gpio_InitIOExt(3,6,GpioDirOut,TRUE,FALSE,FALSE,FALSE);

    //ͨ���˿�����
//    Gpio_SetFunc_UART1TX_P35();
//    Gpio_SetFunc_UART1RX_P36();
    Gpio_SetFunc_UART1_RXD_P02()  ;
    Gpio_SetFunc_UART1_TXD_P01();

    //����ʱ��ʹ��
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Clk_SetPeripheralGate(ClkPeripheralUart1,TRUE);



    stcUartIrqCb.pfnRxIrqCb = Uart1_RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;


    stcConfig.enRunMode = UartMode1;//��������Ĵ˴���ת��4��ģʽ����


    stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//˫�������ʹ���
    stcBaud.u32Baud = 9600;//���²�����λ��
    stcBaud.u8Mode = UartMode3; //���㲨������Ҫģʽ����
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH1,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH1, &stcConfig);
    Uart_EnableIrq(UARTCH1,UartRxIrq);
    Uart_ClrStatus(UARTCH1,UartRxFull);
    Uart_EnableFunc(UARTCH1,UartRx);
// Uart_SetMMDOrCk(UARTCH1,Addr);
}


void Uart1_Output(uint8_t u8Data)
{
    M0P_UART1->SCON_f.REN = 0;
    M0P_UART1->SBUF = u8Data;

    while (TRUE != M0P_UART1->ISR_f.TI)
    {
        ;
    }
    M0P_UART1->ICR_f.TICLR = 0;
}


int fputc (int ch,FILE * f)
{
#if 0
    if (((uint8_t)ch) == '\n')
    {
        Uart1_Output('\r');
    }
    Uart1_Output(ch);
#else
    Uart_SendData(UARTCH1,(uint8_t )ch);
#endif

    return ch;
}

void uart1SendData(uint8 * data,uint16 length)
{
    for(uint16 i=0; i<length; i++ )
    {
        Uart_SendData(UARTCH1,data[i]);
    }

}
//
//int fgetc (FILE * f)
//{
//    uint8_t ch=0;
//  //  HAL_UART_Receive(&huart1,&ch,1,0xffff);
//    return ch;
//}




