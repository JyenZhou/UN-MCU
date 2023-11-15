#ifndef __JHAL_JSON__H__
#define __JHAL_JSON__H__
#include "JSYS.h"

/** ----------------------------JHAL_jsonPack2HmnstHIS----------------------------------- 
  * @描述：
  *
  * @参数：
**	  	 json: [输 出]  json数组的缓存
**			 productID: [输入 ] 产品ID 15bit的
**			 mdata: [输入 ]  二维数组的传感器主数据  例如64个传感器的VC VH数据格式为 vc0-vc63  vh0-vh63 一起是128个长度数据
**			 matrixRowLength: [输入 ] 矩阵的行长度即传感器个数
**			 matrixColumnLength: [输入/出] 矩阵的列长度即数据种类数
**			 sdata: [输入 ] 从数据即单独的数据 例如温湿度
**			 sLength: [输入/出] 从数据种类数
         msg  异常消息无时为""
  *
  * @返回值:字符占用长度
  * @注:无
  *-----------------------------Jyen-2023-10-10-------------------------------------- */

 
/** ----------------------------JHAL_jsonPack2HmnstHIS----------------------------------- 
  * @描述：
  *
  * @参数：
**	  	 json: [输 出]  json数组的缓存
**			 productID: [输入 ] 产品ID 15bit的
**			 mdata: [输入 ]   传感器主数据数据排列为   例如64个传感器的VC VH数据格式为 vc0-vc63  vh0-vh63 一起是128个长度数据
**			 matrixTypeLength  [输入 ]传感器数据种类数  例如有vc vs 
**			 matrixRowLength: [输入 ] 一般板子上都是矩阵 排列的   行长度 
**			 matrixColumnLength: [输入/出]  列长度 
**			 sdata: [输入 ] 从数据即单独的数据 例如温湿度
**			 sTypeLength: [输入/出] 从数据种类数
*      msg  异常消息无时为""
  * @返回值: 字符占用长度
  * @注:这个封装不支持不对齐的矩阵  但是上位机是支持的 需要重写2纬嵌套json   【 【【种类1行1值】【种类1行2值...】，【【种类2行1值】【种类2行2值...】  】
  *-----------------------------Jyen-2023-10-10-------------------------------------- */

 u16 JHAL_jsonPack2HmnstHIS ( char *json,u16 productID ,  float* mdata  ,u8  matrixTypeLength,u8  matrixRowLength ,u8   matrixColumnLength,float* sdata,u8  sTypeLength ,char* errMsg);

#endif

