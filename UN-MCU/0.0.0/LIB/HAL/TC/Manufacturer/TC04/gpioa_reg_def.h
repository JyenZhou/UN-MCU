//----------------------------------------------------------------------
//         COPYRIGHT (C) 2022 TINYCHIP SEMICONDUCTOR                    
    //              CONFIDENTIAL & ALL RIGHTS RESERVED                      
//----------------------------------------------------------------------
// File name      : gpioa_reg_def.h                                     
// Author         : mfpan                                               
// E-Mail         : mfpan@tinychip.com.cn                               
//----------------------------------------------------------------------
// Description    :                                                     
// Register Header File of GPIOA                                     
//----------------------------------------------------------------------
// Reversion History:                                                   
// 2022-10-21 17:24:11 -- automatically generated by reg_gen.pl Revision 1.11          
//----------------------------------------------------------------------

#ifndef   GPIOA_DEF_H__
#define   GPIOA_DEF_H__

// Register Offset Macro Definition
#define GPIOA_DATAIN_OFFSET                                     0x0000
#define GPIOA_DATAOUT_OFFSET                                    0x0004
#define GPIOA_OUTEN_SET_OFFSET                                  0x0008
#define GPIOA_OUTEN_CLR_OFFSET                                  0x000C
#define GPIOA_INT_CLR_OFFSET                                    0x0010
#define GPIOA_INT_MSK_OFFSET                                    0x0014
#define GPIOA_INT_POL_SEL_OFFSET                                0x0018
#define GPIOA_INT_TYP_SEL_OFFSET                                0x001C
#define GPIOA_INT_STATUS_OFFSET                                 0x0020
#define GPIOA_INT_RAW_STATUS_OFFSET                             0x0024

// Register Address Macro Definition
#define GPIOA_DATAIN_ADDR                                       ( GPIOA_BASE_ADDR + GPIOA_DATAIN_OFFSET)
#define GPIOA_DATAOUT_ADDR                                      ( GPIOA_BASE_ADDR + GPIOA_DATAOUT_OFFSET)
#define GPIOA_OUTEN_SET_ADDR                                    ( GPIOA_BASE_ADDR + GPIOA_OUTEN_SET_OFFSET)
#define GPIOA_OUTEN_CLR_ADDR                                    ( GPIOA_BASE_ADDR + GPIOA_OUTEN_CLR_OFFSET)
#define GPIOA_INT_CLR_ADDR                                      ( GPIOA_BASE_ADDR + GPIOA_INT_CLR_OFFSET)
#define GPIOA_INT_MSK_ADDR                                      ( GPIOA_BASE_ADDR + GPIOA_INT_MSK_OFFSET)
#define GPIOA_INT_POL_SEL_ADDR                                  ( GPIOA_BASE_ADDR + GPIOA_INT_POL_SEL_OFFSET)
#define GPIOA_INT_TYP_SEL_ADDR                                  ( GPIOA_BASE_ADDR + GPIOA_INT_TYP_SEL_OFFSET)
#define GPIOA_INT_STATUS_ADDR                                   ( GPIOA_BASE_ADDR + GPIOA_INT_STATUS_OFFSET)
#define GPIOA_INT_RAW_STATUS_ADDR                               ( GPIOA_BASE_ADDR + GPIOA_INT_RAW_STATUS_OFFSET)

// Register Field Macro Definition
#define GPIOA_DATAIN_DATAIN_SHIFT                                    0
#define GPIOA_DATAIN_DATAIN_MASK                                     0x00003FFF
#define GPIOA_DATAIN_DATAIN_SET(n)                                   (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_DATAIN_DATAIN_CLR                                      0xFFFFC000

#define GPIOA_DATAOUT_DATAOUT_SHIFT                                  0
#define GPIOA_DATAOUT_DATAOUT_MASK                                   0x00003FFF
#define GPIOA_DATAOUT_DATAOUT_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_DATAOUT_DATAOUT_CLR                                    0xFFFFC000

#define GPIOA_OUTEN_SET_OUTEN_SET_SHIFT                              0
#define GPIOA_OUTEN_SET_OUTEN_SET_MASK                               0x00003FFF
#define GPIOA_OUTEN_SET_OUTEN_SET_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_OUTEN_SET_OUTEN_SET_CLR                                0xFFFFC000

#define GPIOA_OUTEN_CLR_OUTEN_CLR_SHIFT                              0
#define GPIOA_OUTEN_CLR_OUTEN_CLR_MASK                               0x00003FFF
#define GPIOA_OUTEN_CLR_OUTEN_CLR_SET(n)                             (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_OUTEN_CLR_OUTEN_CLR_CLR                                0xFFFFC000

#define GPIOA_INT_CLR_INT_CLR_SHIFT                                  0
#define GPIOA_INT_CLR_INT_CLR_MASK                                   0x00003FFF
#define GPIOA_INT_CLR_INT_CLR_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_CLR_INT_CLR_CLR                                    0xFFFFC000

#define GPIOA_INT_MSK_INT_MSK_SHIFT                                  0
#define GPIOA_INT_MSK_INT_MSK_MASK                                   0x00003FFF
#define GPIOA_INT_MSK_INT_MSK_SET(n)                                 (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_MSK_INT_MSK_CLR                                    0xFFFFC000

#define GPIOA_INT_POL_SEL_INT_POL_SEL_SHIFT                          0
#define GPIOA_INT_POL_SEL_INT_POL_SEL_MASK                           0x00003FFF
#define GPIOA_INT_POL_SEL_INT_POL_SEL_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_POL_SEL_INT_POL_SEL_CLR                            0xFFFFC000

#define GPIOA_INT_TYP_SEL_INT_TYP_SEL_SHIFT                          0
#define GPIOA_INT_TYP_SEL_INT_TYP_SEL_MASK                           0x00003FFF
#define GPIOA_INT_TYP_SEL_INT_TYP_SEL_SET(n)                         (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_TYP_SEL_INT_TYP_SEL_CLR                            0xFFFFC000

#define GPIOA_INT_STATUS_INT_STATUS_SHIFT                            0
#define GPIOA_INT_STATUS_INT_STATUS_MASK                             0x00003FFF
#define GPIOA_INT_STATUS_INT_STATUS_SET(n)                           (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_STATUS_INT_STATUS_CLR                              0xFFFFC000

#define GPIOA_INT_RAW_STATUS_INT_RAW_STATUS_SHIFT                    0
#define GPIOA_INT_RAW_STATUS_INT_RAW_STATUS_MASK                     0x00003FFF
#define GPIOA_INT_RAW_STATUS_INT_RAW_STATUS_SET(n)                   (((uint32_t)(n) << 0  ) & 0x00003FFF)
#define GPIOA_INT_RAW_STATUS_INT_RAW_STATUS_CLR                      0xFFFFC000

// Register Structure Definition
typedef struct
{
    __IO uint32_t DATAIN          : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_DATAIN_FIELD_T;

typedef struct
{
    __IO uint32_t DATAOUT         : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_DATAOUT_FIELD_T;

typedef struct
{
    __IO uint32_t OUTEN_SET       : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_OUTEN_SET_FIELD_T;

typedef struct
{
    __IO uint32_t OUTEN_CLR       : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_OUTEN_CLR_FIELD_T;

typedef struct
{
    __IO uint32_t INT_CLR         : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_CLR_FIELD_T;

typedef struct
{
    __IO uint32_t INT_MSK         : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_MSK_FIELD_T;

typedef struct
{
    __IO uint32_t INT_POL_SEL     : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_POL_SEL_FIELD_T;

typedef struct
{
    __IO uint32_t INT_TYP_SEL     : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_TYP_SEL_FIELD_T;

typedef struct
{
    __IO uint32_t INT_STATUS      : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_STATUS_FIELD_T;

typedef struct
{
    __IO uint32_t INT_RAW_STATUS  : 14 ; // 13 : 0 
    __IO uint32_t RESERVED0       : 18 ; // 31 : 14 
} GPIOA_INT_RAW_STATUS_FIELD_T;

// Register Map Structure Definition
typedef struct
{
    union 
    { 
        __IO  uint32_t DATAIN                          ; // 0x0000
        GPIOA_DATAIN_FIELD_T DATAIN_F                  ;
    };

    union 
    { 
        __IO  uint32_t DATAOUT                         ; // 0x0004
        GPIOA_DATAOUT_FIELD_T DATAOUT_F                ;
    };

    union 
    { 
        __IO  uint32_t OUTEN_SET                       ; // 0x0008
        GPIOA_OUTEN_SET_FIELD_T OUTEN_SET_F            ;
    };

    union 
    { 
        __IO  uint32_t OUTEN_CLR                       ; // 0x000C
        GPIOA_OUTEN_CLR_FIELD_T OUTEN_CLR_F            ;
    };

    union 
    { 
        __IO  uint32_t INT_CLR                         ; // 0x0010
        GPIOA_INT_CLR_FIELD_T INT_CLR_F                ;
    };

    union 
    { 
        __IO  uint32_t INT_MSK                         ; // 0x0014
        GPIOA_INT_MSK_FIELD_T INT_MSK_F                ;
    };

    union 
    { 
        __IO  uint32_t INT_POL_SEL                     ; // 0x0018
        GPIOA_INT_POL_SEL_FIELD_T INT_POL_SEL_F        ;
    };

    union 
    { 
        __IO  uint32_t INT_TYP_SEL                     ; // 0x001C
        GPIOA_INT_TYP_SEL_FIELD_T INT_TYP_SEL_F        ;
    };

    union 
    { 
        __IO  uint32_t INT_STATUS                      ; // 0x0020
        GPIOA_INT_STATUS_FIELD_T INT_STATUS_F          ;
    };

    union 
    { 
        __IO  uint32_t INT_RAW_STATUS                  ; // 0x0024
        GPIOA_INT_RAW_STATUS_FIELD_T INT_RAW_STATUS_F  ;
    };

} GPIOA_REG_TypeDef;

#endif