#ifndef __JFML_INCLUDES__H__
#define __JFML_INCLUDES__H__
#include "JHAL.h"
#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************************************
   *                                                                *
 *                                                                   *
*                                                                     *
            存放自己封装的片上外设等板载模块整合的.h
						用ifdef屏蔽是为了低的优化等级中不添加过多无用头文件
						                    *
  *                                                                 *
   *                                                              *
*********@作者*Jyen******************@作者*Jyen***********************@作者*Jyen********************/

#ifdef XXOS_MAXTASK
#include "xxddq-1.2.3.h"
#endif
#include "FML/THTB/NTC/NTC.h"
#include "FML/ADPD188BI/adpdDrv.h"
#if defined GasUtil4ICRA_GasSensorNumber 
#include "FML/GAS/GasUtil4ICRA.h"
#endif

#if  defined GasUtil4ICRA2_GasSensorNumber 
#include "FML/GAS/GasUtil4ICRA2.h"
#endif

//ADS1115电压采集芯片
 
#include "FML/ADC/ADS1115/ADS1115.h"

//温湿度传感器
/*温湿度*/
#ifdef SHT20_EN
#include "BISP/BSP/TemperatureAndHumidity/SHT20/SHT20.h"
#endif

#ifdef PT100_MAX31865_EN
#include "BISP/BSP/TAndH/PT100_MAX31865/PT100_MAX31865.h"
#endif





 

// ad7608
#ifdef AD7608_EN
#include "BISP/BSP/ADC/AD7608/AD7608.h"

#endif


#ifdef IS31FL3733_EN
#include "BISP/BSP/IS31FL3733/IS31FL3733.h"
#endif

/*定时器比较输出-步进电机1*/
#ifdef StepperMotor1
#include "BISP/BSP/StepperMotor123/motro1.h"
#endif

/*定时器比较输出-步进电机2的用法*/
#ifdef StepperMotor2
#include "BISP/BSP/StepperMotor123/motro2.h"
#endif

/*定时器比较输出-步进电机3的用法*/
#ifdef StepperMotor3
#include "BISP/BSP/StepperMotor123/motro3.h"
#endif


/*LCD12864不带字库液晶屏 M没字库的意思  另外这个字库共用于OLED*/
#if ((defined LCD12864_M ) || (defined OLED_96 ) )

#include "BISP/BSP/LCD12864/lcd12864_m.h"
#endif

/*OLED0.96寸*/
#ifdef OLED_96
#include "BISP/BSP/OLED_96/OLED.h"
#endif
#ifdef S4_74HC595
#include "BISP/BSP/Display/DigitalTube/S4_74HC595/S4_74HC595.h"
#endif



/*网络*/
/*W5500模块多个依赖的头文件*/
#ifdef W5500Module
#include "BISP/BSP/W5500/W5500/socket.h"
#include "BISP/BSP/W5500/W5500/w5500.h"
#include "BISP/BSP/W5500/W5500/w5500_conf.h"
#include "BISP/BSP/W5500/W5500/utility.h"
#include "BISP/BSP/W5500/Internet/dhcp.h"
#include "BISP/BSP/W5500/Internet/tcp_demo.h"

#include "BISP/BSP/W5500/GeneralTIM/bsp_GeneralTIM.h"
#include "BISP/BSP/W5500/i2c/bsp_EEPROM.h"
#endif

#ifdef Lora_E22_EN
#include "BISP/BSP/Network/Lora/lora_e22_400T30s.h"
#endif


/*TPS7A7100_RGW 0.9-3.5V 可选电压模块*/
#ifdef TPS7A7100_RGW
#include "BISP/BSP/TPS/TPS7A7100/TPS7A7100_RGW.h"
#endif
#ifdef TPS7A7100_RGW_2
#include "BISP/BSP/TPS/TPS7A7100/TPS7A7100_RGW_2.h"
#endif
#ifdef TPS7A4700_RWG
#include "BISP/BSP/TPS/TPS7A4700/TPS7A4700_RGWR.h"
#endif
#ifdef TPS7A4700_RWG_2
#include "BISP/BSP/TPS/TPS7A4700/TPS7A4700_RGWR_2.h"
#endif


/*通过电阻改变输出电压的模块*/
#ifdef TPS7A7001
#include "BISP/BSP/TPS/TPS7A7001/TPS7A7001.h"
#endif


/*通过真值表 切换开关*/
#ifdef ADG704
#include "BISP/BSP/ADG704/ADG704.h"
#endif
#ifdef ADG704_2
#include "BISP/BSP/ADG704/ADG704_2.h"
#endif
#ifdef ADG706
#include "BISP/BSP/ADG/ADG706/ADG706.h"
#endif
#ifdef ADG706_2
#include "BISP/BSP/ADG/ADG706/ADG706_2.h"
#endif


#ifdef CD4051B
#include "BISP/BSP/CD4051B/CD4051B.h"
#endif

#ifdef CD4051B_2
#include "BISP/BSP/CD4051B/CD4051B_2.h"
#endif


#ifdef CD4051B_3
#include "BISP/BSP/CD4051B/CD4051B_3.h"
#endif

#ifdef ADS1256
#include "BISP/BSP/ADC/ADS1256/ADS1256.h"
#endif

#ifdef BL1551
#include "BISP/BSP/BL/BL1551/bl1551.h"
#endif


#ifdef BL1551_2
#include "BISP/BSP/BL/BL1551/bl1551_2.h"
#endif
//
#ifdef ADG1204_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204.h"
#endif
#ifdef ADG1204_2_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204_2.h"
#endif

#ifdef ADG1204_3_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204_3.h"
#endif

#ifdef ADG1204_4_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204_4.h"
#endif

#ifdef ADG1204_5_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204_5.h"
#endif

#ifdef ADG1204_6_EN
#include "BISP/BSP/AnalogSwitch/ADG1204/ADG1204_6.h"
#endif



void  JQStart(void);
void disableInterrupt(void);
void enableInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif


