//----------------------------------------------------------------------
//         COPYRIGHT (C) 2022 TINYCHIP SEMICONDUCTOR                    
    //              CONFIDENTIAL & ALL RIGHTS RESERVED                      
//----------------------------------------------------------------------
// File name      : apinmux_reg_def.h                                   
// Author         : mfpan                                               
// E-Mail         : mfpan@tinychip.com.cn                               
//----------------------------------------------------------------------
// Description    :                                                     
// Register Header File of APINMUX                                   
//----------------------------------------------------------------------
// Reversion History:                                                   
// 2022-10-21 17:24:10 -- automatically generated by reg_gen.pl Revision 1.11          
//----------------------------------------------------------------------

#ifndef   APINMUX_DEF_H__
#define   APINMUX_DEF_H__

// Register Offset Macro Definition
#define APINMUX_PA0_CFG_OFFSET                                  0x0000
#define APINMUX_PA1_CFG_OFFSET                                  0x0004
#define APINMUX_PA2_CFG_OFFSET                                  0x0008
#define APINMUX_PA3_CFG_OFFSET                                  0x000C
#define APINMUX_PA4_CFG_OFFSET                                  0x0010
#define APINMUX_PA5_CFG_OFFSET                                  0x0014
#define APINMUX_PA6_CFG_OFFSET                                  0x0018
#define APINMUX_PA7_CFG_OFFSET                                  0x001C
#define APINMUX_PA8_CFG_OFFSET                                  0x0020
#define APINMUX_PA9_CFG_OFFSET                                  0x0024
#define APINMUX_PA10_CFG_OFFSET                                 0x0028
#define APINMUX_PA11_CFG_OFFSET                                 0x002C
#define APINMUX_PA12_CFG_OFFSET                                 0x0030
#define APINMUX_PA13_CFG_OFFSET                                 0x0034
#define APINMUX_PB0_CFG_OFFSET                                  0x0038
#define APINMUX_PB1_CFG_OFFSET                                  0x003C
#define APINMUX_PB2_CFG_OFFSET                                  0x0040
#define APINMUX_PB3_CFG_OFFSET                                  0x0044
#define APINMUX_PB4_CFG_OFFSET                                  0x0048
#define APINMUX_PB5_CFG_OFFSET                                  0x004C
#define APINMUX_PB6_CFG_OFFSET                                  0x0050
#define APINMUX_PB7_CFG_OFFSET                                  0x0054
#define APINMUX_PB8_CFG_OFFSET                                  0x0058
#define APINMUX_PB9_CFG_OFFSET                                  0x005C
#define APINMUX_PB10_CFG_OFFSET                                 0x0060
#define APINMUX_PB11_CFG_OFFSET                                 0x0064
#define APINMUX_PB12_CFG_OFFSET                                 0x0068
#define APINMUX_PB13_CFG_OFFSET                                 0x006C
#define APINMUX_PB14_CFG_OFFSET                                 0x0070
#define APINMUX_PB15_CFG_OFFSET                                 0x0074
#define APINMUX_PB16_CFG_OFFSET                                 0x0078
#define APINMUX_PB17_CFG_OFFSET                                 0x007C
#define APINMUX_PB18_CFG_OFFSET                                 0x0080
#define APINMUX_PB19_CFG_OFFSET                                 0x0084
#define APINMUX_PB20_CFG_OFFSET                                 0x0088
#define APINMUX_PB21_CFG_OFFSET                                 0x008C
#define APINMUX_PB22_CFG_OFFSET                                 0x0090
#define APINMUX_PB23_CFG_OFFSET                                 0x0094
#define APINMUX_PB24_CFG_OFFSET                                 0x0098
#define APINMUX_PB25_CFG_OFFSET                                 0x009C
#define APINMUX_PB26_CFG_OFFSET                                 0x00A0
#define APINMUX_PB27_CFG_OFFSET                                 0x00A4
#define APINMUX_PB28_CFG_OFFSET                                 0x00A8
#define APINMUX_PB29_CFG_OFFSET                                 0x00AC
#define APINMUX_PB30_CFG_OFFSET                                 0x00B0

// Register Address Macro Definition
#define APINMUX_PA0_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA0_CFG_OFFSET)
#define APINMUX_PA1_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA1_CFG_OFFSET)
#define APINMUX_PA2_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA2_CFG_OFFSET)
#define APINMUX_PA3_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA3_CFG_OFFSET)
#define APINMUX_PA4_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA4_CFG_OFFSET)
#define APINMUX_PA5_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA5_CFG_OFFSET)
#define APINMUX_PA6_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA6_CFG_OFFSET)
#define APINMUX_PA7_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA7_CFG_OFFSET)
#define APINMUX_PA8_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA8_CFG_OFFSET)
#define APINMUX_PA9_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PA9_CFG_OFFSET)
#define APINMUX_PA10_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PA10_CFG_OFFSET)
#define APINMUX_PA11_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PA11_CFG_OFFSET)
#define APINMUX_PA12_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PA12_CFG_OFFSET)
#define APINMUX_PA13_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PA13_CFG_OFFSET)
#define APINMUX_PB0_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB0_CFG_OFFSET)
#define APINMUX_PB1_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB1_CFG_OFFSET)
#define APINMUX_PB2_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB2_CFG_OFFSET)
#define APINMUX_PB3_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB3_CFG_OFFSET)
#define APINMUX_PB4_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB4_CFG_OFFSET)
#define APINMUX_PB5_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB5_CFG_OFFSET)
#define APINMUX_PB6_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB6_CFG_OFFSET)
#define APINMUX_PB7_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB7_CFG_OFFSET)
#define APINMUX_PB8_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB8_CFG_OFFSET)
#define APINMUX_PB9_CFG_ADDR                                    ( APINMUX_BASE_ADDR + APINMUX_PB9_CFG_OFFSET)
#define APINMUX_PB10_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB10_CFG_OFFSET)
#define APINMUX_PB11_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB11_CFG_OFFSET)
#define APINMUX_PB12_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB12_CFG_OFFSET)
#define APINMUX_PB13_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB13_CFG_OFFSET)
#define APINMUX_PB14_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB14_CFG_OFFSET)
#define APINMUX_PB15_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB15_CFG_OFFSET)
#define APINMUX_PB16_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB16_CFG_OFFSET)
#define APINMUX_PB17_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB17_CFG_OFFSET)
#define APINMUX_PB18_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB18_CFG_OFFSET)
#define APINMUX_PB19_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB19_CFG_OFFSET)
#define APINMUX_PB20_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB20_CFG_OFFSET)
#define APINMUX_PB21_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB21_CFG_OFFSET)
#define APINMUX_PB22_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB22_CFG_OFFSET)
#define APINMUX_PB23_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB23_CFG_OFFSET)
#define APINMUX_PB24_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB24_CFG_OFFSET)
#define APINMUX_PB25_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB25_CFG_OFFSET)
#define APINMUX_PB26_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB26_CFG_OFFSET)
#define APINMUX_PB27_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB27_CFG_OFFSET)
#define APINMUX_PB28_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB28_CFG_OFFSET)
#define APINMUX_PB29_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB29_CFG_OFFSET)
#define APINMUX_PB30_CFG_ADDR                                   ( APINMUX_BASE_ADDR + APINMUX_PB30_CFG_OFFSET)

// Register Field Macro Definition
#define APINMUX_CFG_SR_SHIFT                                 24
#define APINMUX_CFG_SR_MASK                                  0x01000000
#define APINMUX_CFG_SR_SET(n)                                (((uint32_t)(n) << 24 ) & 0x01000000)
#define APINMUX_CFG_SR_CLR                                   0xFEFFFFFF

#define APINMUX_CFG_OS_SHIFT                                 23
#define APINMUX_CFG_OS_MASK                                  0x00800000
#define APINMUX_CFG_OS_SET(n)                                (((uint32_t)(n) << 23 ) & 0x00800000)
#define APINMUX_CFG_OS_CLR                                   0xFF7FFFFF

#define APINMUX_CFG_OD_SHIFT                                 22
#define APINMUX_CFG_OD_MASK                                  0x00400000
#define APINMUX_CFG_OD_SET(n)                                (((uint32_t)(n) << 22 ) & 0x00400000)
#define APINMUX_CFG_OD_CLR                                   0xFFBFFFFF

#define APINMUX_CFG_DR_SHIFT                                 21
#define APINMUX_CFG_DR_MASK                                  0x00200000
#define APINMUX_CFG_DR_SET(n)                                (((uint32_t)(n) << 21 ) & 0x00200000)
#define APINMUX_CFG_DR_CLR                                   0xFFDFFFFF

#define APINMUX_CFG_CS_SHIFT                                 20
#define APINMUX_CFG_CS_MASK                                  0x00100000
#define APINMUX_CFG_CS_SET(n)                                (((uint32_t)(n) << 20 ) & 0x00100000)
#define APINMUX_CFG_CS_CLR                                   0xFFEFFFFF

#define APINMUX_CFG_SW_IE_SHIFT                              19
#define APINMUX_CFG_SW_IE_MASK                               0x00080000
#define APINMUX_CFG_SW_IE_SET(n)                             (((uint32_t)(n) << 19 ) & 0x00080000)
#define APINMUX_CFG_SW_IE_CLR                                0xFFF7FFFF

#define APINMUX_CFG_SW_PU_SHIFT                              18
#define APINMUX_CFG_SW_PU_MASK                               0x00040000
#define APINMUX_CFG_SW_PU_SET(n)                             (((uint32_t)(n) << 18 ) & 0x00040000)
#define APINMUX_CFG_SW_PU_CLR                                0xFFFBFFFF

#define APINMUX_CFG_SW_PD_SHIFT                              17
#define APINMUX_CFG_SW_PD_MASK                               0x00020000
#define APINMUX_CFG_SW_PD_SET(n)                             (((uint32_t)(n) << 17 ) & 0x00020000)
#define APINMUX_CFG_SW_PD_CLR                                0xFFFDFFFF

#define APINMUX_CFG_SW_PULL_SEL_SHIFT                        16
#define APINMUX_CFG_SW_PULL_SEL_MASK                         0x00010000
#define APINMUX_CFG_SW_PULL_SEL_SET(n)                       (((uint32_t)(n) << 16 ) & 0x00010000)
#define APINMUX_CFG_SW_PULL_SEL_CLR                          0xFFFEFFFF

#define APINMUX_CFG_SW_SRC_SEL_SHIFT                         8
#define APINMUX_CFG_SW_SRC_SEL_MASK                          0x00000700
#define APINMUX_CFG_SW_SRC_SEL_SET(n)                        (((uint32_t)(n) << 8  ) & 0x00000700)
#define APINMUX_CFG_SW_SRC_SEL_CLR                           0xFFFFF8FF


// Register Structure Definition
typedef struct
{
    __IO uint32_t RESERVED2       : 8  ; // 7  : 0 
    __IO uint32_t SW_SRC_SEL      : 3  ; // 10 : 8 
    __IO uint32_t RESERVED1       : 5  ; // 15 : 11 
    __IO uint32_t SW_PULL_SEL     : 1  ; // 16 : 16 
    __IO uint32_t SW_PD           : 1  ; // 17 : 17 
    __IO uint32_t SW_PU           : 1  ; // 18 : 18 
    __IO uint32_t SW_IE           : 1  ; // 19 : 19 
    __IO uint32_t CS              : 1  ; // 20 : 20 
    __IO uint32_t DR              : 1  ; // 21 : 21 
    __IO uint32_t OD              : 1  ; // 22 : 22 
    __IO uint32_t OS              : 1  ; // 23 : 23 
    __IO uint32_t SR              : 1  ; // 24 : 24 
    __IO uint32_t RESERVED0       : 7  ; // 31 : 25 
} APINMUX_CFG_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union 
    { 
        __IO  uint32_t PA0_CFG             ; // 0x0000
        APINMUX_CFG_FIELD_T PA0_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA1_CFG             ; // 0x0004
        APINMUX_CFG_FIELD_T PA1_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA2_CFG             ; // 0x0008
        APINMUX_CFG_FIELD_T PA2_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA3_CFG             ; // 0x000C
        APINMUX_CFG_FIELD_T PA3_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA4_CFG             ; // 0x0010
        APINMUX_CFG_FIELD_T PA4_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA5_CFG             ; // 0x0014
        APINMUX_CFG_FIELD_T PA5_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA6_CFG             ; // 0x0018
        APINMUX_CFG_FIELD_T PA6_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA7_CFG             ; // 0x001C
        APINMUX_CFG_FIELD_T PA7_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA8_CFG             ; // 0x0020
        APINMUX_CFG_FIELD_T PA8_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA9_CFG             ; // 0x0024
        APINMUX_CFG_FIELD_T PA9_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PA10_CFG            ; // 0x0028
        APINMUX_CFG_FIELD_T PA10_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PA11_CFG            ; // 0x002C
        APINMUX_CFG_FIELD_T PA11_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PA12_CFG            ; // 0x0030
        APINMUX_CFG_FIELD_T PA12_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PA13_CFG            ; // 0x0034
        APINMUX_CFG_FIELD_T PA13_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB0_CFG             ; // 0x0038
        APINMUX_CFG_FIELD_T PB0_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB1_CFG             ; // 0x003C
        APINMUX_CFG_FIELD_T PB1_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB2_CFG             ; // 0x0040
        APINMUX_CFG_FIELD_T PB2_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB3_CFG             ; // 0x0044
        APINMUX_CFG_FIELD_T PB3_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB4_CFG             ; // 0x0048
        APINMUX_CFG_FIELD_T PB4_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB5_CFG             ; // 0x004C
        APINMUX_CFG_FIELD_T PB5_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB6_CFG             ; // 0x0050
        APINMUX_CFG_FIELD_T PB6_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB7_CFG             ; // 0x0054
        APINMUX_CFG_FIELD_T PB7_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB8_CFG             ; // 0x0058
        APINMUX_CFG_FIELD_T PB8_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB9_CFG             ; // 0x005C
        APINMUX_CFG_FIELD_T PB9_CFG_F  ;
    };

    union 
    { 
        __IO  uint32_t PB10_CFG            ; // 0x0060
        APINMUX_CFG_FIELD_T PB10_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB11_CFG            ; // 0x0064
        APINMUX_CFG_FIELD_T PB11_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB12_CFG            ; // 0x0068
        APINMUX_CFG_FIELD_T PB12_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB13_CFG            ; // 0x006C
        APINMUX_CFG_FIELD_T PB13_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB14_CFG            ; // 0x0070
        APINMUX_CFG_FIELD_T PB14_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB15_CFG            ; // 0x0074
        APINMUX_CFG_FIELD_T PB15_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB16_CFG            ; // 0x0078
        APINMUX_CFG_FIELD_T PB16_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB17_CFG            ; // 0x007C
        APINMUX_CFG_FIELD_T PB17_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB18_CFG            ; // 0x0080
        APINMUX_CFG_FIELD_T PB18_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB19_CFG            ; // 0x0084
        APINMUX_CFG_FIELD_T PB19_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB20_CFG            ; // 0x0088
        APINMUX_CFG_FIELD_T PB20_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB21_CFG            ; // 0x008C
        APINMUX_CFG_FIELD_T PB21_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB22_CFG            ; // 0x0090
        APINMUX_CFG_FIELD_T PB22_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB23_CFG            ; // 0x0094
        APINMUX_CFG_FIELD_T PB23_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB24_CFG            ; // 0x0098
        APINMUX_CFG_FIELD_T PB24_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB25_CFG            ; // 0x009C
        APINMUX_CFG_FIELD_T PB25_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB26_CFG            ; // 0x00A0
        APINMUX_CFG_FIELD_T PB26_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB27_CFG            ; // 0x00A4
        APINMUX_CFG_FIELD_T PB27_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB28_CFG            ; // 0x00A8
        APINMUX_CFG_FIELD_T PB28_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB29_CFG            ; // 0x00AC
        APINMUX_CFG_FIELD_T PB29_CFG_F;
    };

    union 
    { 
        __IO  uint32_t PB30_CFG            ; // 0x00B0
        APINMUX_CFG_FIELD_T PB30_CFG_F;
    };

} APINMUX_REG_TypeDef;

#endif
