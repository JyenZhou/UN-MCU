#include "JSYS.h"



/** ----------------------------__JHAL_instructionTest----------------------------------- 
  * 描述：指令测试-常用操作符
  *
  * 参数：
**	  	 : [输入/出] 
  *
  * 返回值:无
  * 注:无
  *-----------------------------Jyen-2022-11-22-------------------------------------- */

void __JHAL_instructionTest()

{
    u32_t op1, op2, op3, result;
    u32_t ret;

    ret = 0u;
    op1 = 0x33333333u;
    op2 = 0x22222222u;

    /* 加法 */
    result = op1 + op2;
    if (result != 0x55555555u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* 减法 */
    result = op1 - op2;
    if (result != 0x11111111u)
    {
        ret = 0xFFFFFFFFu;
    }

    op2 = 2u;

    /* 乘法 */
    result = op1 * op2;
    if (result != 0x66666666u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* 除法 */
    result = op1 / op2;
    if (result != 0x19999999u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* 取反 */
    result = ~op1;
    if (result != ~0x33333333u)
    {
        ret = 0xFFFFFFFFu;
    }

    /* 左移 */
    result = op1 << 12;
    if (result != (0x33333000u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* 右移 */
    result = op1 >> 12;
    if (result != (0x00033333u) )
    {
        ret = 0xFFFFFFFFu;
    }

    op2 = 0x22222222u;

    /* 按位与 */
    result = op1 & op2;
    if (result != (0x22222222u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* 按位或 */
    result = op1 | op2;
    if (result != (0x33333333u) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* 异或 */
    result = op1 ^ op2;
    if (result != (0x11111111u) )
    {
        ret = 0xFFFFFFFFu;
    }

    op1 = 0x77777777u;
    op2 = 0x33333333u;
    op3 = 2u;

    /* 乘加 */
    result = op1 + (op2 * op3);
    if (result != (0xDDDDDDDDu) )
    {
        ret = 0xFFFFFFFFu;
    }

    /* 乘减 */
    result = op1 - (op2 * op3);
    if (result != (0x11111111u) )
    {
        ret = 0xFFFFFFFFu;
    }
    // 指令测试失败
    while(ret!=0u);

}







void __JHAL_systemSelfTest(void)
{
    // 指令测试
    __JHAL_instructionTest();


}















