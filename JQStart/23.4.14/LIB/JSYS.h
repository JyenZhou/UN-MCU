#ifndef __JSYS_INCLUDES__H__
#define  __JSYS_INCLUDES__H__
#ifdef __CplusPlus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
    /**********BEGIN******************无符号定义*************BEGIN************BEGIN**********************************/
    typedef   uint8_t  u8_t;
    typedef   uint16_t  u16_t;
    typedef   uint32_t  u32_t;
    typedef   uint64_t  u64_t;
//-----------
    typedef   uint8_t  uint8;
    typedef   uint16_t  uint16;
    typedef   uint32_t  uint32;
    typedef   uint64_t  uint64;
//-----------

    typedef  uint8_t  u8;
    typedef  uint16_t  u16;
    typedef  uint32_t u32;
    typedef   uint64_t  u64;

    typedef  uint8_t  U8;
    typedef  uint16_t  U16;
    typedef  uint32_t U32;
    typedef   uint64_t  U64;

//-----------
    typedef volatile uint8 vuint8;
    typedef volatile uint16 vuint16;
    typedef volatile uint32 vuint32;

    typedef volatile uint8 vu8;
    typedef volatile uint16 vu16;
    typedef volatile uint32 vu32;


    /***************************END****无符号定义******END*********************************************/



    /****BEGIN*******BEGIN******有符号定义********BEGIN**********BEGIN***********************/


    typedef volatile int8_t vint8;
    typedef   int8_t int8;
    typedef   int16_t       int16;
    typedef  int32_t            int32;
    typedef  int64_t            int64;
    /***************END************END****有符号定义*****END***************END*****************************************/






    /**函数指针类型为void/void函数 */
    typedef void         (*FuncPtr)(void);

    /**函数指针类型为void/uint8_t函数 */
    typedef void         (*FuncPtrArg1)(uint8_t u8Param);
		
		    /**函数指针类型为void/uint8_t函数 */
    typedef void         (*FuncPtrArg1x)(uint8_t *u8Param);
		 typedef void         (*FuncPtrArg2)(uint8_t u8Param,uint8_t u8Param2);


    /**函数指针类型为void/uint8_t函数 */
    typedef u8         (*FuncPtrArg1Ret1)(uint8_t u8Param);
		
		 
  




    /*GCC定义*/
#define OS_NORETURN __attribute__((__noreturn__))
#define OS_NOTHROW __attribute__((__nothrow__))
#define OS_NONNULL1 __attribute__((__nonnull__(1)))  //第一个参数不能为空
#define OS_NONNULL2 __attribute__((__nonnull__(2)))
#define OS_NONNULL3 __attribute__((__nonnull__(3)))
#define OS_NONNULL4 __attribute__((__nonnull__(4)))
#define OS_NONNULL5 __attribute__((__nonnull__(5)))
#define OS_NONNULL6 __attribute__((__nonnull__(6)))
#define OS_NONNULL7 __attribute__((__nonnull__(7)))
#define OS_NONNULL8 __attribute__((__nonnull__(8)))
#define OS_NONNULL9 __attribute__((__nonnull__(9)))
#define OS_NONNULL10 __attribute__((__nonnull__(10)))
#define OS_NONNULL11 __attribute__((__nonnull__(11)))
#define OS_NONNULL12 __attribute__((__nonnull__(12)))
#define OS_NONNULL13 __attribute__((__nonnull__(13)))
#define OS_NONNULL14 __attribute__((__nonnull__(14)))
#define OS_NONNULL15 __attribute__((__nonnull__(15)))
#define OS_NONNULL_ALL __attribute__((__nonnull__))  //这个参数可以无数多个
//) __attribute__((nonnull(1,3)));  这个的意思是，第一个和第三个参数不能为空。
#define OS_SENTINEL __attribute__((__sentinel__))
#define OS_PURE __attribute__((__pure__))
#define OS_CONST __attribute__((__const__))  //这个表示一个方法的返回值只由参数决定，如果参数不变的话，就不再调用此函数，直接返回值 。经过我的尝试发现还是调用了，后又经查资料发现要给gcc加一个-O的参数才可以。是对函数调用的一种优化
#define OS_WARN_RESULT __attribute__((__warn_unused_result__))
#define OS_MALLOC __attribute__((__malloc__))
#define OS_USED  	__attribute__((__used__))    //表示函数的返回值必须被检查或使用，否则会警告。
#define OS_UNUSED __attribute__((__unused__))  //__unused 
#define OS_WEAK __attribute__((__weak__))
#define OS_WEAK_IMPORT __attribute__((__weak_import__))
#define OS_NOINLINE __attribute__((__noinline__))
#define OS_ALWAYS_INLINE __attribute__((__always_inline__))  //这段代码能够保证代码是内联的，因为你如果只定义内联的话，编译器并不一定会以内联的方式调用，如果代码太多你就算用了内联也不一定会内联，用了这个的话会强制内联
#define OS_TRANSPARENT_UNION __attribute__((__transparent_union__))
#define OS_ALIGNED(n) __attribute__((__aligned__((n))))
#define OS_FORMAT_PRINTF(x,y) __attribute__((__format__(printf,x,y)))
#define OS_EXPORT extern __attribute__((__visibility__("default")))
#define OS_INLINE static __inline__
#define OS_EXPECT(x, v) __builtin_expect((x), (v))
#define OS_BEFORE_MAIN_EXE __attribute__((__constructor__))
#define OS_AFTER_MAIN_EXE __attribute__((destructor))












//百纳秒->纳秒
#define  _100Nsec2Nsec 100



//微秒->纳秒
#define  Usec2Nsec 1000


//毫秒到微秒
#define  Msec2Usec 1000
//毫秒到纳秒
#define  Msec2Nsec (Msec2Usec*Usec2Nsec)


//秒->纳秒
#define  Sec2Nsec (Msec2Usec*Msec2Nsec)
//秒->毫秒
#define  Sec2Msec 1000
//秒->微秒
#define  Sec2Usec (Sec2Msec*Msec2Nsec)

//分钟->秒
#define  Minute2Sec 60
//分钟->毫秒
#define Minute2Msec (Minute2Sec*Sec2Msec)
//分钟->微秒
#define Minute2Usec (Minute2Msec*Msec2Usec)


//小时->分钟
#define  Hour2Minute 60
//小时->秒
#define Hour2Sec (Hour2Minute*Minute2Sec)
//小时->毫秒
#define Hour2Msec (Hour2Sec*Sec2Msec)
//小时->微秒
#define Hour2Usec (Hour2Msec*Msec2Usec)


//天->小时
#define  Day2Hour  24
//天->分钟
#define Day2Minute (Day2Hour*Hour2Minute)
//天->秒
#define Day2Sec (Day2Minute*Minute2Sec)
//天->毫秒
#define Day2Msec  (Day2Sec*Sec2Msec)

    /*年->月*/
#define   Year2Month 12

    typedef enum
    {

        JHAL_TimeUnits_NS=1,
        JHAL_TimeUnits_100NS=_100Nsec2Nsec,
        JHAL_TimeUnits_US=Usec2Nsec,
        JHAL_TimeUnits_MS=Msec2Nsec,
        JHAL_TimeUnits_S=Sec2Nsec,
    } JHAL_TimeUnits;


    /*电阻换算*/
//M->K
#define  M2KOhm 1000
//K->Ω
#define  K2Ohm 1000
//M->Ω
#define  M2Ohm M2KOhm*K2Ohm


//这样定义的好处能够自动进行类型匹配()不能少不然会被拆分造成第一个a与别的地方运算了
#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

#define	 max3(a,b,c)		((c>b&&c>a)?c:(a>b)?a:b)
#define  min3(a,b,c)    ((c<a&&c<b)?c:(a<b)?a:b)


    typedef enum {
        JHAL_Align_Center=0,
        JHAL_Align_Left,
        JHAL_Align_Right,
    } JHAL_Align;


#ifdef __cplusplus
}
#endif

#endif




