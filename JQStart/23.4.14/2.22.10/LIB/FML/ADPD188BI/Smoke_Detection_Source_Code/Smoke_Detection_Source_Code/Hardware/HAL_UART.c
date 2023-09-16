/* Includes -----------------------------------------------------------------*/
#include "stdint.h"
#include "drivers\uart\adi_uart.h"

#if(ADI_CFG_ENABLE_RTOS_SUPPORT == 1)
#include <ssdd_common/common_def.h>
#include "adi_uart_def_v1.h"
#endif
#include "HwIfConfig.h"
#include "main.h"
#ifndef __EV_COG
#include <mailman.h>
//#include <cobs.h>
#ifdef _USING_RTOS_
#include "adi_osal.h"
#endif //_USING_RTOS_
#endif
ADI_UART_HANDLE hDevOutput = NULL;
#pragma data_alignment = 4
#pragma location = "bank2_retained_ram"
uint8_t OutDeviceMem[ADI_UART_BIDIR_MEMORY_SIZE];
uint8_t UartRxFlag =0, UartRxCh = 0;

#define UART0_TX_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<20))
#define UART0_RX_PORTP0_MUX  ((uint32_t) ((uint32_t) 1<<22))


#define SIZE_OF_TX_BUFFER  1024

#define SIZE_OF_RX_BUFFER  255

#define UART_DEVICE_NUM 0

#define UART_BLOCKING
/*
                    Boudrate divider for PCLK-26000000

+------------------------------------------------------------------------+
| CALCULATING UART DIV REGISTER VALUE FOR THE  INPUT CLOCK: 26000000     |
|------------------------------------------------------------------------|
|       BAUDRATE       DIV-C     DIV-M     DIV-N         OSR    DIFF     |
|------------------------------------------------------------------------|
|       00009600        0022      0003      1734        0003    0000     |
|------------------------------------------------------------------------|
|       00019200        0011      0003      1735        0003    0000     |
|------------------------------------------------------------------------|
|       00038400        0017      0001      0501        0003    0000     |
|------------------------------------------------------------------------|
|       00057600        0007      0002      0031        0003    0000     |
|------------------------------------------------------------------------|
|       00115200        0007      0002      0031        0002    0000     |
|------------------------------------------------------------------------|
|       00230400        0007      0002      0031        0001    0000     |
|------------------------------------------------------------------------|
|       00460800        0007      0002      0031        0000    0001     |
|------------------------------------------------------------------------|
|       00921600        0001      0001      1563        0002    0028     |
|------------------------------------------------------------------------|
|       01000000        0001      0001      1280        0002    0000     |
|------------------------------------------------------------------------|
|       01500000        0001      0002      0341        0001    0112     |
|------------------------------------------------------------------------|
|       03000000        0001      0002      0341        0000    0225     |
|------------------------------------------------------------------------|
|       04000000        0001      0001      1280        0000    0000     |
|------------------------------------------------------------------------|
|       05000000        0001      0001      0614        0000    0751     |
|------------------------------------------------------------------------|
|       06000000        0001      0001      0171        0000    0901     |
|------------------------------------------------------------------------|
|       06500000        0001      0001      0000        0000    0000     |
|------------------------------------------------------------------------|

*/

/* Select the baudrate divider as 115200 define BAUD_115200 else 460800 is
set as default
*/
#define UART_DIV_C_11     11
#define UART_DIV_M_3      3
#define UART_DIV_N_1735   1735
#define UART_DIV_C_1      1
#define UART_DIV_C_7      7

#define UART_DIV_M_1      1
#define UART_DIV_M_2      2

#define UART_DIV_N_31     31
#define UART_DIV_N_341    341
#define UART_DIV_N_1563   1563
#define UART_DIV_N_1280   1280

#define UART_OSR_0        0
#define UART_OSR_1        1
#define UART_OSR_2        2
#define UART_OSR_3        3


/* Public function prototypes ---------------------------------------------- */
void UART_ReceiverINTEnable();
void UART_ReceiverINTDisable();
void UART_write(uint8_t *pData, uint32_t size);
void UART_RegisterReceiverCallback( uint16_t (*pfReceiveInputChar)(char *pChar, uint16_t nRxLength) );

/* Private typedef ----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#pragma data_alignment = 4
static char nBufferRx0[SIZE_OF_RX_BUFFER];
static bool nRxLengthFlag = 1;
static uint16_t nRxLength;
//static uint8_t TxCompleteFlag = 0, gTxPendingFlag =0;
// Globals

uint8_t g_pkt_buff[256];

//#pragma data_alignment = 4
// static uint8_t nBufferTx0[SIZE_OF_TX_BUFFER] = "ADI UART V1";
#ifndef __EV_COG
m2m2_hdr_t *g_RX_CMD_PKT = NULL;
#endif
#ifdef _USING_RTOS_
ADI_OSAL_MUTEX_HANDLE UartTransferLock;
#endif //_USING_RTOS_

#ifdef _USING_RTOS_
ADI_OSAL_SEM_HANDLE UartTxSync;
#endif //_USING_RTOS_
/* Private function prototypes -----------------------------------------------*/
static void UART1_Init();
// static void UART2_Init();
// static void UART3_Init();

static void SetupUart(void);

static uint16_t (*pfRegisterUARTReceiveCallback)(char *pChar, uint16_t nRxLength);
#ifndef __EV_COG
uint16_t pkt_uart_cb(char *pByte, uint16_t nRxLength){
    #pragma location = "bank1_retained_ram"
    static  uint8_t uart_rx_buf[256];
    static  uint8_t *bufptr = (uint8_t*)&uart_rx_buf[0];
    static m2m2_hdr_t *p_rcvd_m2m2_ = (m2m2_hdr_t *)uart_rx_buf;

    for (uint8_t i=0; i < nRxLength; i++) {
        *(bufptr++) = pByte[i];
    }
    
    /* swap from network byte order to little endian */
    p_rcvd_m2m2_->src = (M2M2_ADDR_ENUM_t)BYTE_SWAP_16(p_rcvd_m2m2_->src);
    p_rcvd_m2m2_->dest = (M2M2_ADDR_ENUM_t)BYTE_SWAP_16(p_rcvd_m2m2_->dest);
    p_rcvd_m2m2_->length = BYTE_SWAP_16(p_rcvd_m2m2_->length);
    p_rcvd_m2m2_->checksum = BYTE_SWAP_16(p_rcvd_m2m2_->checksum);
    g_RX_CMD_PKT = p_rcvd_m2m2_;    
    
    bufptr = (uint8_t*)&uart_rx_buf[0];
    return 0;
}
#endif
static void UARTCallback(
                         void        *pAppHandle,
                         uint32_t     nEvent,
                         void        *pArg
                             ) {
    /* CASEOF (event type) */
    switch (nEvent) {
    case ADI_UART_EVENT_TX_BUFFER_PROCESSED:
#ifdef _USING_RTOS_
        adi_osal_SemPost(UARTSync);
#else // !_USING_RTOS_
//        TxCompleteFlag =1;
#endif // _USING_RTOS_
//        gTxPendingFlag = 0;
        break;

    case ADI_UART_EVENT_RX_BUFFER_PROCESSED:

        /* BLE_UART / FTDI_UART selected */
        if (nRxLengthFlag) {
            /* The data received is the Length */
            nRxLengthFlag = 0;
            nRxLength = (((uint8_t) nBufferRx0[4] << 8) + (uint8_t) nBufferRx0[5]) - 8;
            if ((adi_uart_SubmitRxBuffer(hDevOutput, &nBufferRx0[8], nRxLength,true))!= ADI_UART_SUCCESS) {
                // printf(" Failed to submit the buffer for Rx channel");
                break;

            }
        }
        else {
            /* The data received is the packet data*/
            if( pfRegisterUARTReceiveCallback )
                (*pfRegisterUARTReceiveCallback)(nBufferRx0, (nRxLength + 8));
            nRxLengthFlag = 1;
            nRxLength = 8;
            if ((adi_uart_SubmitRxBuffer(hDevOutput, nBufferRx0, nRxLength,true))!= ADI_UART_SUCCESS) {
                // printf(" Failed to submit the buffer for Rx channel");
                break;
            }
        }
        break;
    }
    /* return */
}

/* Exported variables --------------------------------------------------------*/
uint8_t* UartRxCmd[10];
uint8_t UartRxCmd_temp[200];

//void UART_ReceiverINTEnable() {
//  if ((adi_uart_EnableRx(hDevOutput, true))!= ADI_UART_SUCCESS) {
//        // printf("\n\t Failed to enable the Rx");
//    }
//}
//
//void UART_ReceiverINTDisable() {
//  if ((adi_uart_EnableRx(hDevOutput, false))!= ADI_UART_SUCCESS) {
//    // printf("\n\t Failed to Disable the Rx");
//  }
//}

void UARTClearFifo() {
    adi_uart_FlushRxFifo(hDevOutput);
    adi_uart_FlushTxFifo(hDevOutput);
}

void UART_baudrate_select(UART_BAUD_OPTIONS options) {
  ADI_UART_RESULT eResult = ADI_UART_SUCCESS;
  switch (options) {
    case BAUD_19200:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_11,
                                        UART_DIV_M_3,
                                        UART_DIV_N_1735,
                                        UART_OSR_3);
      break;
    case BAUD_57600:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_7,
                                        UART_DIV_M_2,
                                        UART_DIV_N_31,
                                        UART_OSR_3);
      break;
    case BAUD_115200:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_7,
                                        UART_DIV_M_2,
                                        UART_DIV_N_31,
                                        UART_OSR_2);
      break;
    case BAUD_230400:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_7,
                                        UART_DIV_M_2,
                                        UART_DIV_N_31,
                                        UART_OSR_1);
      break;
    case BAUD_460800:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_7,
                                        UART_DIV_M_2,
                                        UART_DIV_N_31,
                                        UART_OSR_0);
      break;
    case BAUD_921600:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_1,
                                        UART_DIV_M_1,
                                        UART_DIV_N_1563,
                                        UART_OSR_2);
      break;
    case BAUD_1000000:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_1,
                                        UART_DIV_M_1,
                                        UART_DIV_N_1280,
                                        UART_OSR_2);
      break;
    case BAUD_1500000:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_1,
                                        UART_DIV_M_2,
                                        UART_DIV_N_341,
                                        UART_OSR_1);
      break;
    case BAUD_2000000:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_1,
                                        UART_DIV_M_1,
                                        UART_DIV_N_1280,
                                        UART_OSR_1);
      break;
    case BAUD_3000000:
      eResult = adi_uart_ConfigBaudRate(hDevOutput,
                                        UART_DIV_C_1,
                                        UART_DIV_M_2,
                                        UART_DIV_N_341,
                                        UART_OSR_0);
      break;
    default:
      break;
  }
  if (eResult != ADI_UART_SUCCESS) {
    Debug_Handler();
  }
}

void UART_RegisterReceiverCallback( uint16_t (*pfReceiveInputChar)(char *pChar, uint16_t nRxLength) ) {
  pfRegisterUARTReceiveCallback = pfReceiveInputChar;

  nRxLength = 8; /* Submit 8 byte to receive the header of packet; FTDI UART */

//    /* Enable the DMA associated with UART if UART is expeced to work with DMA mode */
//    adi_uart_EnableDMAMode(hDevOutput, true);

    /* Submit the first buffer for Rx.  */
    if ((adi_uart_SubmitRxBuffer(hDevOutput, nBufferRx0, nRxLength,true))!= ADI_UART_SUCCESS) {
        // printf(" Failed to submit the buffer for Rx channel");
    }

}
/*
 * Initialize the Port Control MUX Registers
 */
int32_t adi_initpinmux_uart(void) {
    /* Port Control MUX registers */
    *((volatile uint32_t *)REG_GPIO0_CFG)|= UART0_TX_PORTP0_MUX | UART0_RX_PORTP0_MUX;

    return 0;
}

/* ************************************************************************* */
/**
    * @brief    UART initialization. This function initiaises all the UART controllers used.
    * @param    None
    * @retval   None
    */
void UART_Init(void) {
  UART1_Init();
  // UART2_Init();
  // UART3_Init();
}

/**
    * @brief    USART1 init function
    * @param    None
    * @retval   None
    */
void UART1_Init(void) {
#ifdef _USING_RTOS_
    adi_osal_MutexCreate(&UartTransferLock);
#endif //_USING_RTOS_
  adi_initpinmux_uart();
  /* Open the UART device.Data transfer is bidirectional with NORMAL mode by default.  */
  SetupUart();
}

void UART_Perf(char *InfoString) {
    int16_t nStrSize;
    uint32_t hwError;

    nStrSize = strlen(InfoString);
#ifdef _USING_RTOS_
    adi_osal_MutexPend(UartTransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#endif //_USING_RTOS_
    /* ignore return codes since there's nothing we can do if it fails */
    /* fails if custom call back is fixed */
    //if (adi_uart_Write(hDevOutput, InfoString, nStrSize) != ADI_UART_SUCCESS) {
    if (adi_uart_Write(hDevOutput, InfoString, nStrSize, true, &hwError) != ADI_UART_SUCCESS) {
// Debug_Handler();
    }
#ifdef _USING_RTOS_
    adi_osal_MutexPost(UartTransferLock);
#endif //_USING_RTOS_
    /* do not quit... */
}

void UART_write(uint8_t *pData, uint32_t size) {
  uint32_t hwError;
#ifdef _USING_RTOS_
    adi_osal_MutexPend(UartTransferLock, ADI_OSAL_TIMEOUT_FOREVER);
#endif //_USING_RTOS_
    //adi_uart_Write(hDevOutput, pData, size);
    adi_uart_Write(hDevOutput, pData, size, true, &hwError);
#ifdef _USING_RTOS_
    adi_osal_MutexPost(UartTransferLock);
#endif //_USING_RTOS_
}

uint8_t UART_read(uint8_t *pData, uint32_t size) {
  //    eUartResult = adi_uart_Read(hDevOutput, pData, size);
    // ADI_UART_RESULT  eUartResult;

    if (UartRxFlag) {
        UartRxFlag =0;
        *pData = UartRxCh;
        return (ADI_UART_SUCCESS);
    } else {
        return(ADI_UART_FAILED);
    }

}

void SetupUart(void) {
    if ((adi_uart_Open(0, ADI_UART_DIR_BIDIRECTION, OutDeviceMem, sizeof(OutDeviceMem), &hDevOutput)) != 0) {
        // printf("\n\t UART device open  failed");
    }
    /* Configure  UART device with NO-PARITY, ONE STOP BIT and 8bit word length. */
    if (adi_uart_SetConfiguration(hDevOutput,
                                  ADI_UART_NO_PARITY,
                                  ADI_UART_ONE_STOPBIT,
                                  ADI_UART_WORDLEN_8BITS) != ADI_UART_SUCCESS) {
        // printf("UART device configuration   failed ");
    }
    /* Baud rate div values are calcuated for PCLK 26Mhz. Please use the
    host utility UartDivCalculator.exe provided with the installer"
    */
   UART_baudrate_select(DEFAULT_UART_BAUD);
    if ((adi_uart_EnableFifo(hDevOutput, true))!= ADI_UART_SUCCESS) {
        // printf("Call back registration failed");
    }

#ifdef ADI_UART_CFG_ENABLE_DMA_SUPPORT
    /* Enable the DMA associated with UART if UART is expeced to work with DMA mode */
    if (adi_uart_EnableDMAMode(hDevOutput, true)!= ADI_UART_SUCCESS) {
        // printf("Failed to enable the DMA mode");
    }
#endif

    /* Register a call back to test callback mode */
    if (adi_uart_RegisterCallback(hDevOutput, UARTCallback, hDevOutput)!= ADI_UART_SUCCESS) {
        // printf("Call back registration failed");
    }
	#ifndef __EV_COG
    UART_RegisterReceiverCallback(pkt_uart_cb);
	#endif
//    /* Submit the first buffer for Rx.  */
//    //if (adi_uart_SubmitRxBuffer(hDevOutput, nBufferRx0, SIZE_OF_RX_BUFFER)!= ADI_UART_SUCCESS) {
//    if (adi_uart_SubmitRxBuffer(hDevOutput, nBufferRx0, SIZE_OF_RX_BUFFER, true)!= ADI_UART_SUCCESS) { //TODO: dma??
//        // printf(" Failed to submit the buffer for Rx channel");
//    }

    /* Enable the Data flow for Rx */
    /*if (adi_uart_EnableRx(hDevOutput, true)!= ADI_UART_SUCCESS) {
        // printf("\n\t Failed to enable the Rx");
    }*/
}

uint8_t getInputCmd(void)
{
  // expect correct inputs
  uint8_t i, j, rxCh,cmdSeg;

  UartRxCmd[0] = &UartRxCmd_temp[0];                                           //Point to first command segment

  for (i=0, j=0; (i< sizeof(UartRxCmd_temp) - 1) && (j < sizeof(UartRxCmd)/4 - 1);)
  {
    if (UART_read(&rxCh, 1) == ADI_UART_SUCCESS) {
      if (rxCh == 0xFE){          //This is recieved when FTDI is plugged in
        break;
      }

      rxCh &= 0x7F;
      if (rxCh == ' ') {
        UartRxCmd_temp[i++] = '\0';                                           //Terminate command segment
        UartRxCmd[++j] = &UartRxCmd_temp[i];                                 //Point to next command segment
        continue;
      }
      if (rxCh == '\r' || rxCh == '\n') {
        // Todo: clear uart FIFO
        break;
      }

      if (rxCh>0x40 && rxCh<0x5b)
        rxCh += 0x20;           // change to lower case
      UartRxCmd_temp[i++] = rxCh;
    }
    else if (i == 0) {
      return 0;
    }
  }

  UartRxCmd_temp[i] = '\0';

  if(j==0 && i==0)                    //Case when /r/n is entered
  {
    while(j < sizeof(UartRxCmd)/4)
      UartRxCmd[j++] = &UartRxCmd_temp[i];
     return 0;
  }

  j++;
  cmdSeg = j;
  while(j < sizeof(UartRxCmd)/4)
    UartRxCmd[j++] = &UartRxCmd_temp[i];

  return (cmdSeg);
}
