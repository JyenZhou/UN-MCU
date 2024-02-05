#ifndef  ADS1256
#ifndef __ADS1256__H__
#define __ADS1256__H__
#include "JHAL.h"
#ifdef __CplusPlus
extern "C" {
#endif
//SPI端口
    extern SPI_HandleTypeDef hspi4;
#define SPIX hspi4
//AD_DIN  SPIX_MOSI_Pin
#define 				AD_DIN_Pin    GPIO_PIN_6 //ADS1256 DIN
#define 				AD_DIN_GPIO_Port GPIOE   //ADS1256 DIN
////AD_DOUT SPIX_MISO_Pin
#define 	AD_DOUT_Pin GPIO_PIN_5 //ADS1256 DOUT
#define 	AD_DOUT_GPIO_Port GPIOE //ADS1256 DOUT
////AD_SCLK SPIX_SCK_Pin
#define 		AD_SCLK_Pin  GPIO_PIN_2  //ADS1256 SCLK
#define 		AD_SCLK_GPIO_Port GPIOE   //ADS1256 SCLK
//AD_CS
#define SPIX_Default_NSS_Pin			  	AD_CS1_Pin  //ADS1256 CS
#define SPIX_Default_NSS_Prot			  	AD_CS1_GPIO_Port  //ADS1256 CS
//其他引脚


#define ADS1256_DRDY_Pin					AD_DRDY_Pin    //ADS1256 D1
#define ADS1256_DRDY_Prot				AD_DRDY_GPIO_Port    //ADS1256 D1

//#define ADS1256_RST_Pin					AD_RST_Pin //ADS1256 D0
//#define ADS1256_RST_Prot				AD_RST_GPIO_Port //ADS1256 D0
//#define ADS1256_SYNC_Pin					AD_SYNC_Pin  //ADS1256 D2
//#define ADS1256_SYNC_Prot				AD_SYNC_GPIO_Port    //ADS1256 D1
//#define ADS1256_D0				GPIO_Pin_12 //ADS1256 D0
//#define ADS1256_D1				GPIOB    //ADS1256 D1
//#define ADS1256_D2				GPIO_Pin_12  //ADS1256 D2
//#define ADS1256_D3				GPIO_Pin_12  //ADS1256 D2


//单端或差分模式 0代表单端  1代表差分
#define  Single_endedDifferenceModel 0

    /**************************定义ads1256命令*******************/
#define ADS1256_CMD_WAKEUP   0x00
#define ADS1256_CMD_RDATA    0x01
#define ADS1256_CMD_RDATAC   0x03
#define ADS1256_CMD_SDATAC   0x0f
#define ADS1256_CMD_RREG     0x10
#define ADS1256_CMD_WREG     0x50
#define ADS1256_CMD_SELFCAL  0xf0
#define ADS1256_CMD_SELFOCAL 0xf1
#define ADS1256_CMD_SELFGCAL 0xf2
#define ADS1256_CMD_SYSOCAL  0xf3
#define ADS1256_CMD_SYSGCAL  0xf4
#define ADS1256_CMD_SYNC     0xfc
#define ADS1256_CMD_STANDBY  0xfd
#define ADS1256_CMD_REST    0xfe

    /**************************定义ads1256寄存器地址**************/
#define ADS1256_STATUS       0x00
#define ADS1256_MUX          0x01
#define ADS1256_ADCON        0x02
#define ADS1256_DRATE        0x03
#define ADS1256_IO           0x04
#define ADS1256_OFC0         0x05
#define ADS1256_OFC1         0x06
#define ADS1256_OFC2         0x07
#define ADS1256_FSC0         0x08
#define ADS1256_FSC1         0x09
#define ADS1256_FSC2         0x0A


    /***************************定义设置ads1256采集速率代码*******/
#define ADS1256_MUXP_AIN0   0x00
#define ADS1256_MUXP_AIN1   0x10
#define ADS1256_MUXP_AIN2   0x20
#define ADS1256_MUXP_AIN3   0x30
#define ADS1256_MUXP_AIN4   0x40
#define ADS1256_MUXP_AIN5   0x50
#define ADS1256_MUXP_AIN6   0x60
#define ADS1256_MUXP_AIN7   0x70
#define ADS1256_MUXP_AINCOM 0x80
    /****************************定义负输入通道*******/
#define ADS1256_MUXN_AIN0   0x00
#define ADS1256_MUXN_AIN1   0x01
#define ADS1256_MUXN_AIN2   0x02
#define ADS1256_MUXN_AIN3   0x03
#define ADS1256_MUXN_AIN4   0x04
#define ADS1256_MUXN_AIN5   0x05
#define ADS1256_MUXN_AIN6   0x06
#define ADS1256_MUXN_AIN7   0x07
#define ADS1256_MUXN_AINCOM 0x08



    /***************************定义增益枚举*********************/
    typedef enum
    {
        ADS1256_GAIN_1 =     0x00,
        ADS1256_GAIN_2   =   0x01,
        ADS1256_GAIN_4   =   0x02,
        ADS1256_GAIN_8   =   0x03,
        ADS1256_GAIN_16  =   0x04,
        ADS1256_GAIN_32    = 0x05,
        ADS1256_GAIN_64  =   0x06
    } ADS1256_Magnification_TypeDef;


    /***************************定义设置ads1256采集速率代码*******/
#define ADS1256_DRATE_30000SPS   0xF0
#define ADS1256_DRATE_15000SPS   0xE0
#define ADS1256_DRATE_7500SPS   0xD0
#define ADS1256_DRATE_3750SPS   0xC0
#define ADS1256_DRATE_2000SPS   0xB0
#define ADS1256_DRATE_1000SPS   0xA1
#define ADS1256_DRATE_500SPS    0x92
#define ADS1256_DRATE_100SPS    0x82
#define ADS1256_DRATE_60SPS     0x72
#define ADS1256_DRATE_50SPS     0x63
#define ADS1256_DRATE_30SPS     0x53
#define ADS1256_DRATE_25SPS     0x43
#define ADS1256_DRATE_15SPS     0x33
#define ADS1256_DRATE_10SPS     0x23
#define ADS1256_DRATE_5SPS      0x13
#define ADS1256_DRATE_2_5SPS    0x03

		
		extern   GPIO_TypeDef *SPIX_NSS_Prot ;
	  extern	 uint16 SPIX_NSS_Pin;
		
 
  void ADS1256ReadData(unsigned char channel,int32 *adcVule);
    void ADS1256_Config(ADS1256_Magnification_TypeDef magnification);   //初始化ADS1256
    void Init_ADS1256_Init(void);
void ADS1256Test(void);
#ifdef CplusPlus
}
#endif

#endif
#endif





