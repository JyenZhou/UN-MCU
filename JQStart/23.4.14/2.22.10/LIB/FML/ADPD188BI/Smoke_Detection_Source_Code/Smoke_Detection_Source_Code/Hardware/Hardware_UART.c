#include "Hardware.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SIZE_OF_TX_BUFFER  1024
#define SIZE_OF_RX_BUFFER  255
#define UART_DEVICE_NUM 0
#define UART_BLOCKING

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

/* Private variables ---------------------------------------------------------*/
ADI_UART_HANDLE hDevOutput = NULL;
uint8_t OutDeviceMem[ADI_UART_BIDIR_MEMORY_SIZE];

/* Private variables ---------------------------------------------------------*/

/**
 * @brief       UART Callback function
 * @param[in]   Device handle
 * @param[in]   Event
 * @param[in]   8-bit data pointer
 * @return      none 
 */
static void UARTCallback(
                         void        *pAppHandle,
                         uint32_t     nEvent,
                         void        *pArg
                             ) {
    /* CASEOF (event type) */
    switch (nEvent) {
    case ADI_UART_EVENT_TX_BUFFER_PROCESSED:

        break;

    case ADI_UART_EVENT_RX_BUFFER_PROCESSED:


        break;
    }
    /* return */
}

/**
 * @brief       UART baudrate select 
 * @param[in]   Buadrate
 * @return      none 
 */
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
                                        UART_OSR_1);
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

  }
}

/**
 * @brief       configure UART 
 * @param[in]   none
 * @return      none 
 */
static void SetupUart(void){
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
  
  adi_uart_RegisterCallback(hDevOutput, UARTCallback, hDevOutput);
  
}

/**
    * @brief    USART1 init function
    * @param    None
    * @retval   None
    */
void UART1_Init(void) {

//  adi_initpinmux_uart();
  /* Open the UART device.Data transfer is bidirectional with NORMAL mode by default.  */
  SetupUart();
}

/**
    * @brief    UART initialization. This function initiaises all the UART controllers used.
    * @param    None
    * @retval   None
    */
void UART_Init(void) {
  UART1_Init();

}

/**
 * @brief       output char value to UART interface 
 * @param[in]   x  value to be writed via UART
 * @return      none 
 */
void uart_put(unsigned char x)
{
  pADI_UART0->TX = x;
  while ((pADI_UART0->LSR & 0x00000020) == 0)  ;
}

/**
 * @brief       output a string value to UART 
 * @param[in]   none
 * @return      none 
 */
int  uart_printf( const char * format, ... ){
//  char str[100];
  uint8_t sendlength = 0;
  
  sendlength = strlen(format);
  for(int i=0; i<sendlength; i++){
    uart_put(format[i]);
  }
  return (0);
}


/**
 * @brief       write function for stdio print
 * @param[in]   none
 * @return      none 
 */
size_t
__dwrite(int fd, const unsigned char *buf, size_t count)
{
  int n = count;
  const unsigned char *p = buf;
  while(n--)
    uart_put(*p++);
  return count;
}
