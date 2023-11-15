/**
  ****************************JHAL_Math.c******************************************
  * @brief   ��װһЩ���õķ��� ���಻�Ǻ���ȷ ��Ҫ����Ū̫��ɢ���ļ�
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
*@ �������ܻ���: ����ʽ����
  * @�������:    n������ �������ָ��
									a��ϵ������������ӳ�����߽�����a[0]��ʾ������ 
									x�� Ҫ����� x
  * @�� �� ֵ: ��
  * @��ע: ��
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
 

/***********���ÿ��ŷ�װ***************************************************************/

//����ֵ��<0(�������û�)��>0(�����û�)��0(�������û�)��
int __JHAL_u32CompareOrderByAscend(const void *a, const void *b)
{
    return *(uint32_t*)a - *(uint32_t*)b; //��С��������
    //return *(int *)b - *(int *)a;// �ɴ�С����
    //return *(double *)a > *(double *)b ? 1 : -1;//С�����͵�
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
  * @������ u32�����������򣬴�С�����,����
  *
  * @������
**	  	 array: Ҫ���������
**			 length: ���鳤��
  *
  * @����ֵ:��
  * @ע:����������˳�򽫱��޸�
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

void JHAL_sortU32ArrayAsc( u32* array, u32 length)
{
    qsort(array, length, sizeof(u32), __JHAL_u32CompareOrderByAscend);
}
/*�����*/
void JHAL_sortU32ArrayDesc( u32* array, u32 length)
{
    qsort(array, length, sizeof(u32), __JHAL_u32CompareOrderByDescend);
}

/* u8���͵� */
void JHAL_sortU8ArrayAsc( u8* array, u32 length)
{
    qsort(array, length, sizeof(u8), __JHAL_u8CompareOrderByAscend);
}

void JHAL_sortU8ArrayDesc( u8* array, u32 length)
{
    qsort(array, length, sizeof(u8), __JHAL_u8CompareOrderByDescend);
}








//����ֵ��<0(�������û�)��>0(�����û�)��0(�������û�)��
int __JHAL_FloatIndexSetCompareOrderByAscend(const void *a, const void *b)
{

return ((JHAL_FloatInde4Set*)a)->value > ((JHAL_FloatInde4Set*)b)->value?1:-1;



}



/** ----------------------------JHAL_sortFloatIndexSetAsc----------------------------------- 
  * @����������ʵ�ִ����������Ŀ���
  *
  * @������
**	  	 arraySet: ���� 
**			 length: ���鳤�� 
  *
  * @����ֵ:��
  * @ע:��
  *-----------------------------Jyen-2022-11-27-------------------------------------- */

void JHAL_sortFloatIndexSetAsc( JHAL_FloatInde4Set *arraySet, u32 length)
{

    qsort(arraySet, length, sizeof(JHAL_FloatInde4Set), __JHAL_FloatIndexSetCompareOrderByAscend);
}




/** ----------------------------JHAL_rangeMatchByOrderedArray----------------------------------- 
  * @�������ж�һ���������������������±�֮��
  *
  * @������
**	  	 num: Ҫ�жϵ��� 
**			 arr[]: ���� 
**			 startIndex: ��ʼ�±� ���ж�ĳ������ʱʹ�� ����õ�ַ ��ƥ��ɹ�ʱ���޸ĸ�ֵ��������ֵ
**			 endIndex: �����±� ���ж�ĳ������ʱʹ��
  *
  * @����ֵ:�Ƿ����ĳ�����±�֮��
  * @ע:��
  *-----------------------------Jyen-2023-04-10-------------------------------------- */



bool JHAL_rangeMatchByOrderedArray(u32 num, u32 arr[], u8* startIndex, u8 endIndex)
{
    if (num < arr[0] || num > arr[endIndex]) {
        return false;  // ���ֲ������鷶Χ��
    }
    
    u8 left = *startIndex;
    u8 right = endIndex - 1;
    
    // ���ֲ���Ŀ��������������
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
    
    return false;  // û�ҵ�Ŀ������
}
//���������������� ֻƥ�����Ȳ��ҵ���
bool JHAL_rangeMatchArray(u32 num, u32 arr[], u8 *startIndex, u8 endIndex) {
	
    for(; *startIndex < endIndex; (*startIndex)++) {
        if(num >= arr[*startIndex] && num < arr[(*startIndex)+1]) {                       
            return true;
        }
    }
    return false;
}
 
 

