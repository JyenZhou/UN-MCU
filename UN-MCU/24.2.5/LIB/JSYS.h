#ifndef __JSYS_INCLUDES__H__
#define  __JSYS_INCLUDES__H__
#ifdef __CplusPlus
extern "C" {
#endif
//�����ҵķ�����ǰ����Ϊ�˲����ظ�����ô���
#include <manufacturer.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
    /**********BEGIN******************�޷��Ŷ���*************BEGIN************BEGIN**********************************/
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


    /***************************END****�޷��Ŷ���******END*********************************************/



    /****BEGIN*******BEGIN******�з��Ŷ���********BEGIN**********BEGIN***********************/


    typedef volatile int8_t vint8;
    typedef   int8_t int8;
    typedef   int16_t       int16;
    typedef  int32_t            int32;
    typedef  int64_t            int64;
    /***************END************END****�з��Ŷ���*****END***************END*****************************************/






    /**����ָ������Ϊvoid/void���� */
    typedef void         (*FuncPtr)(void);

    /**����ָ������Ϊvoid/uint8_t���� */
    typedef void         (*FuncPtrArg1)(uint8_t u8Param);
		
		    /**����ָ������Ϊvoid/uint8_t���� */
    typedef void         (*FuncPtrArg1x)(uint8_t *u8Param);
		 typedef void         (*FuncPtrArg2)(uint8_t u8Param,uint8_t u8Param2);


    /**����ָ������Ϊvoid/uint8_t���� */
    typedef u8         (*FuncPtrArg1Ret1)(uint8_t u8Param);
		
		 
		 
		 typedef enum{
			 JHAL_Alignment_Left,
			 JHAL_Alignment_Middle,
			 JHAL_Alignment_Right ,
			 
		 }JHAL_Alignment;
  
 
  


    /*GCC����*/
#define OS_NORETURN __attribute__((__noreturn__))
#define OS_NOTHROW __attribute__((__nothrow__))
#define OS_NONNULL1 __attribute__((__nonnull__(1)))  //��һ����������Ϊ��
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
#define OS_NONNULL_ALL __attribute__((__nonnull__))  //������������������
//) __attribute__((nonnull(1,3)));  �������˼�ǣ���һ���͵�������������Ϊ�ա�
#define OS_SENTINEL __attribute__((__sentinel__))
#define OS_PURE __attribute__((__pure__))
#define OS_CONST __attribute__((__const__))  //�����ʾһ�������ķ���ֵֻ�ɲ��������������������Ļ����Ͳ��ٵ��ô˺�����ֱ�ӷ���ֵ �������ҵĳ��Է��ֻ��ǵ����ˣ����־������Ϸ���Ҫ��gcc��һ��-O�Ĳ����ſ��ԡ��ǶԺ������õ�һ���Ż�
#define OS_WARN_RESULT __attribute__((__warn_unused_result__))
#define OS_MALLOC __attribute__((__malloc__))
#define OS_USED  	__attribute__((__used__))    //��ʾ�����ķ���ֵ���뱻����ʹ�ã�����ᾯ�档
#define OS_UNUSED __attribute__((__unused__))  //__unused 
#define OS_WEAK __attribute__((__weak__))
#define OS_WEAK_IMPORT __attribute__((__weak_import__))
#define OS_NOINLINE __attribute__((__noinline__))
#define OS_ALWAYS_INLINE __attribute__((__always_inline__))  //��δ����ܹ���֤�����������ģ���Ϊ�����ֻ���������Ļ�������������һ�����������ķ�ʽ���ã��������̫���������������Ҳ��һ������������������Ļ���ǿ������
#define OS_TRANSPARENT_UNION __attribute__((__transparent_union__))
#define OS_ALIGNED(n) __attribute__((__aligned__((n))))
#define OS_FORMAT_PRINTF(x,y) __attribute__((__format__(printf,x,y)))
#define OS_EXPORT extern __attribute__((__visibility__("default")))
#define OS_INLINE static __inline__
#define OS_EXPECT(x, v) __builtin_expect((x), (v))
#define OS_BEFORE_MAIN_EXE __attribute__((__constructor__))
#define OS_AFTER_MAIN_EXE __attribute__((destructor))












//������->����
#define  _100Nsec2Nsec 100



//΢��->����
#define  Usec2Nsec 1000


//���뵽΢��
#define  Msec2Usec 1000
//���뵽����
#define  Msec2Nsec (Msec2Usec*Usec2Nsec)


//��->����
#define  Sec2Nsec (Msec2Usec*Msec2Nsec)
//��->����
#define  Sec2Msec 1000
//��->΢��
#define  Sec2Usec (Sec2Msec*Msec2Nsec)

//����->��
#define  Minute2Sec 60
//����->����
#define Minute2Msec (Minute2Sec*Sec2Msec)
//����->΢��
#define Minute2Usec (Minute2Msec*Msec2Usec)


//Сʱ->����
#define  Hour2Minute 60
//Сʱ->��
#define Hour2Sec (Hour2Minute*Minute2Sec)
//Сʱ->����
#define Hour2Msec (Hour2Sec*Sec2Msec)
//Сʱ->΢��
#define Hour2Usec (Hour2Msec*Msec2Usec)


//��->Сʱ
#define  Day2Hour  24
//��->����
#define Day2Minute (Day2Hour*Hour2Minute)
//��->��
#define Day2Sec (Day2Minute*Minute2Sec)
//��->����
#define Day2Msec  (Day2Sec*Sec2Msec)

    /*��->��*/
#define   Year2Month 12

    typedef enum
    {

        JHAL_TimeUnits_NS=1,
        JHAL_TimeUnits_100NS=_100Nsec2Nsec,
        JHAL_TimeUnits_US=Usec2Nsec,
        JHAL_TimeUnits_MS=Msec2Nsec,
        JHAL_TimeUnits_S=Sec2Nsec,
    } JHAL_TimeUnits;


    /*���軻��*/
//M->K
#define  M2KOhm 1000
//K->��
#define  K2Ohm 1000
//M->��
#define  M2Ohm M2KOhm*K2Ohm




//��������ĺô��ܹ��Զ���������ƥ��()�����ٲ�Ȼ�ᱻ�����ɵ�һ��a���ĵط�������
#ifdef 		min
		#undef min
#endif		
		
#ifdef 		max
		#undef max
#endif		

#define min( a, b )  ((a) < (b) ? (a) : (b))
#define max( a, b )  ((a) > (b) ? (a) : (b))
#define	 max3(a,b,c)		(((c)>(b)&&(c)>(a))?(c):((a)>(b))?(a):(b))
#define  min3(a,b,c)    (((c)<(a)&&(c)<(b))?(c):((a)<(b))?(a):(b))


    typedef enum {
        JHAL_Align_Center=0,
        JHAL_Align_Left,
        JHAL_Align_Right,
    } JHAL_Align;


#ifdef __cplusplus
}
#endif

#endif




