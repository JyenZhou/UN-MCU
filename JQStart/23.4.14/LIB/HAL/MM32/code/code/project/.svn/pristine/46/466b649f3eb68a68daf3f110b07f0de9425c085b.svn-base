#include "uart.h"

uint8_t	 recv_byte[USART_RX_LEN];
uint8_t	 recv_byte_temp[USART_RX_LEN];
uint8_t	 recv_count = 0;
uint8_t	 recv_timeout = 0;

#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE s32 __io_putchar(s32 ch)
#else
#define PUTCHAR_PROTOTYPE s32 fputc(s32 ch, FILE *f)

#endif

#ifdef USE_IAR
PUTCHAR_PROTOTYPE {
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#else
void _sys_exit(s32 x)
{
    x = x;
}
//redefine fputcfunction
s32 fputc(s32 ch, FILE* f)
{
    while((UART1->CSR & UART_IT_TXIEN) == 0); //The loop is sent until it is finished
    UART1->TDR = (ch & (u16)0x00FF);
    return ch;
}

#endif

void Send_Data_To_UART0(uint8_t dat)
{
		while((UART1->CSR & UART_IT_TXIEN) == 0){}; //The loop is sent until it is finished
    UART1->TDR = (dat & (u16)0x00FF);
}

void Uart_Init(u32 bound)
{
    //GPIO GPIO set
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART1, ENABLE);   //enableUART1,GPIOAclock
    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);  //
    //UART initialset
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);
    UART_StructInit(&UART_InitStructure);
    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//one stopbit
    UART_InitStructure.UART_Parity = UART_Parity_No;//none odd-even  verify bit
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//No hardware flow control
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx; // receive and sent  mode

    UART_Init(UART1, &UART_InitStructure); //initial uart 1
    UART_Cmd(UART1, ENABLE);               //enable uart 1

    //UART1_TX   GPIOA.12
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //UART1_RX    GPIOA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		exNVIC_Init_TypeDef  NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

    exNVIC_Init(&NVIC_InitStruct);
		
		UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);
}

void UART1_IRQHandler(void)
{
	if (UART_GetITStatus(UART1, UART_IT_RXIEN) != RESET)
	{
		recv_byte_temp[recv_count] = UART_ReceiveData(UART1);
		
		recv_timeout = 0;
		if(recv_count<USART_RX_LEN-1){
			recv_count++;
		}
		
	}
}

