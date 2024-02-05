/**
  ****************************JHAL_Math.c******************************************
  * @brief   封装一些常用的方法 分类不是很明确 主要不想弄太多散的文件
  *
    ******************************************************************************
  * @file     JHAL_Math.c
  * @author   Jyen
  * @version  v1.0
  * @date     2022-11-27 
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2022,Jyen,China. All Rights Reserved
  ********************************************************************************
  */
  #include "JHAL_Math.h"







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
 
double JHAL_polynomial(int n, double a[], double x)
{
    int i;
    double p = a[n];
    for (i = n; i > 0; i--)
    {
        p = a[i-1] + x*p;
    }
    return p;
}
 

/***********常用快排封装***************************************************************/

//返回值，<0(不进行置换)，>0(进行置换)，0(不进行置换)。
int __JHAL_u32CompareOrderByAscend(const void *a, const void *b)
{
    return *(uint32_t*)a - *(uint32_t*)b; //由小到大排序
    //return *(int *)b - *(int *)a;// 由大到小排序
    //return *(double *)a > *(double *)b ? 1 : -1;//小数类型的
}
int __JHAL_u32CompareOrderByDescend(const void *a, const void *b)
{
    return *(uint32_t*)b - *(uint32_t*)a; 
}
int __JHAL_u8CompareOrderByAscend(const void *a, const void *b)
{
    return *(uint8_t*)a - *(uint8_t*)b;
}
int __JHAL_u8CompareOrderByDescend(const void *a, const void *b)
{
    return *(uint8_t*)b - *(uint8_t*)a; 
}

/** ----------------------------JHAL_mathSortArrayU32Asc----------------------------------- 
  * @描述： u32类型数组排序，从小到大的,升序
  *
  * @参数：
**	  	 array: 要排序的数组
**			 length: 数组长度
  *
  * @返回值:无
  * @注:传进来数组顺序将被修改
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

void JHAL_sort32ArrayAsc( int32* array, u32 length)
{
    qsort(array, length, sizeof(u32), __JHAL_u32CompareOrderByAscend);
}

void JHAL_sortU32ArrayAsc( u32* array, u32 length)
{
    qsort(array, length, sizeof(u32), __JHAL_u32CompareOrderByAscend);
}
/*降序的*/
void JHAL_sortU32ArrayDesc( u32* array, u32 length)
{
    qsort(array, length, sizeof(u32), __JHAL_u32CompareOrderByDescend);
}

/* u8类型的 */
void JHAL_sortU8ArrayAsc( u8* array, u32 length)
{
    qsort(array, length, sizeof(u8), __JHAL_u8CompareOrderByAscend);
}

void JHAL_sortU8ArrayDesc( u8* array, u32 length)
{
    qsort(array, length, sizeof(u8), __JHAL_u8CompareOrderByDescend);
}








//返回值，<0(不进行置换)，>0(进行置换)，0(不进行置换)。
int __JHAL_FloatIndexSetCompareOrderByAscend(const void *a, const void *b)
{

return ((JHAL_FloatInde4Set*)a)->value > ((JHAL_FloatInde4Set*)b)->value?1:-1;



}



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

void JHAL_sortFloatIndexSetAsc( JHAL_FloatInde4Set *arraySet, u32 length)
{

    qsort(arraySet, length, sizeof(JHAL_FloatInde4Set), __JHAL_FloatIndexSetCompareOrderByAscend);
}




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



bool JHAL_rangeMatchByOrderedArray(u32 num, u32 arr[], u8* startIndex, u8 endIndex)
{
    if (num < arr[0] || num > arr[endIndex]) {
        return false;  // 数字不在数组范围内
    }
    
    u8 left = *startIndex;
    u8 right = endIndex - 1;
    
    // 二分查找目标数字所在区间
    while (left <= right) {
        u8 mid = left + (right - left) / 2;
        if (arr[mid] <= num && num < arr[mid+1]) {
            *startIndex = mid;
            return true;
        } else if (arr[mid] > num) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    return false;  // 没找到目标区间
}
//和上面比这是无序的 只匹配最先查找到的
bool JHAL_rangeMatchArray(u32 num, u32 arr[], u8 *startIndex, u8 endIndex) {
	
    for(; *startIndex < endIndex; (*startIndex)++) {
        if(num >= arr[*startIndex] && num < arr[(*startIndex)+1]) {                       
            return true;
        }
    }
    return false;
}
 
 

