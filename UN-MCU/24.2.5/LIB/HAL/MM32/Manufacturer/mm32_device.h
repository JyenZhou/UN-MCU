/*
 *******************************************************************************
    @file mm32_device.h
    @brief CMSIS Cortex-M Peripheral Access Layer for MindMotion
           microcontroller devices

    This is a convenience header file for defining the part number on the
    build command line, instead of specifying the part specific header file.

    Example: Add MM32 series to your build options, to define part
              Add "#include "mm32_device.h" to your source files




 *******************************************************************************
 */

#ifndef __MM32_DEVICE_H
#define __MM32_DEVICE_H

#ifdef MM32G0001
#include "mm32g0001.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic ignored "-Wc11-extensions"
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

#ifdef __cplusplus
}
#endif

/** --------------------------------------------------------------------------*/
#endif /* __MM32_DEVICE_H ----------------------------------------------------*/
/** --------------------------------------------------------------------------*/
