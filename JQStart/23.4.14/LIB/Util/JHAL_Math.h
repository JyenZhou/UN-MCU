#ifndef __MYMath__H__
#define __MYMath__H__
#include "JSYS.h"

typedef struct
{
    u32 index;
    float value;
}JHAL_FloatInde4Set;


/*------------------Jyen--------------------------Jyen-----------------------*/
/*******************************************************************************
*@ 函数功能或简介: 多项式计算
  * @输入参数:    n：次数 ，即最高指数
									a：系数放在数组里，从常数项到高阶排序，a[0]表示常数项 
									x： 要计算的 x
  * @返 回 值: 无
  * @备注: 无
  *
  *
  ******************************************************************************
  *
  *------------------Jyen-------------------------Jyen-------------------------*/

double JHAL_polynomial(int n, double a[], double x);
/** ----------------------------JHAL_mathSortArrayU32Asc----------------------------------- 
  * @描述： u32类型数组排序，从小到大的,下标0最小,升序 
  *
  * @参数：
**	  	 array: [要排序的数组] 
**			 length: [数组长度] 
  *
  * @返回值:无
  * @注:传进来数组顺序将被修改
  *-----------------------------Jyen-2022-11-22-------------------------------------- */
void JHAL_sortU32ArrayAsc( u32* array, u32 length);
/* ...降序的,下标0最大*/
void JHAL_sortU32ArrayDesc( u32* array, u32 length);
/* ...u8类型的数组操作 */
void JHAL_sortU8ArrayAsc( u8* array, u32 length);
void JHAL_sortU8ArrayDesc( u8* array, u32 length);

/** ----------------------------JHAL_sortFloatIndexSetAsc----------------------------------- 
  * @描述：变相实现带返回索引的快排
  *
  * @参数：
**	  	 arraySet: 数组 
**			 length: 数组长度 
  *
  * @返回值:无
  * @注:无
  *-----------------------------Jyen-2022-11-27-------------------------------------- */
void JHAL_sortFloatIndexSetAsc( JHAL_FloatInde4Set *arraySet, u32 length);



/** ----------------------------JHAL_rangeMatchByOrderedArray----------------------------------- 
  * @描述：判断一个数处于序数组哪两个下标之间
  *
  * @参数：
**	  	 num: 要判断的数 
**			 arr[]: 数组 
**			 startIndex: 开始下标 当判断某段数组时使用 这个用地址 当匹配成功时会修改该值当做返回值
**			 endIndex: 结束下标 当判断某段数组时使用
  *
  * @返回值:是否存在某两个下标之间
  * @注:无
  *-----------------------------Jyen-2023-04-10-------------------------------------- */



bool JHAL_rangeMatchByOrderedArray(u32 num, u32 arr[], u8* startIndex, u8 endIndex);
//和上面比这是无序的 只匹配最先查找到的
bool JHAL_rangeMatchArray(u32 num, u32 arr[], u8 *startIndex, u8 endIndex) ;
#endif


