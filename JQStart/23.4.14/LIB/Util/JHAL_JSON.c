
/*crc校验*/

/*
参数说明
pdata 数据 包含要行校验后的数据预留位
 length  数据长度 含校验位的长度

校验模式下返回值 ture成功/false失败
AutoWirte 下自动修改包尾校验值
*/





#include "JHAL_JSON.h"


/** ----------------------------JHAL_jsonPack2HmnstHIS-----------------------------------
  * @描述：
  *
  * @参数：
**	  	 json: [输 出]  json数组的缓存
**			 productID: [输入 ] 产品ID 15bit的
**			 mdata: [输入 ]   传感器主数据数据排列为   例如64个传感器的VC VH数据格式为 vc0,vh0,vc1,vh1,vc2,vh2....vc64,vh64
**			 matrixTypeLength  [输入 ]传感器数据种类数  例如有vc vs
**			 matrixRowLength: [输入 ] 一般板子上都是矩阵 排列的   行长度
**			 matrixColumnLength: [输入/出]  列长度
**			 sdata: [输入 ] 从数据即单独的数据 例如温湿度
**			 sTypeLength: [输入/出] 从数据种类数
*      msg  异常消息无时为""
  * @返回值: 字符占用长度
  * @注:这个封装不支持不对齐的矩阵  但是上位机是支持的 需要重写2纬嵌套json   【 【【种类1列1值】【种类2列1值...】，【【种类1列2值】【种类2列2值...】  】
  *-----------------------------Jyen-2023-10-10-------------------------------------- */

u16 JHAL_jsonPack2HmnstHIS ( char *json,u16 productID,  float* mdata,u8  matrixTypeLength,u8  matrixRowLength,u8   matrixColumnLength,float* sdata,u8  sTypeLength,char* errMsg)

{
#define Float_Str_Size 20
    char float_str[Float_Str_Size]; // 适当选择足够大的缓冲区来存储float的字符串表示

    char uid[13];
    uid2string(uid,13);

    // 使用sprintf将数据格式化为JSON字符串

    sprintf(json, "{\"pid\": %d ,\"uid\":\"%s\",\"md\":[",productID,uid);


    for(u8 j=0; j<matrixRowLength; j++) {
        strcat(json, "[");
        // 将float数组的元素追加到JSON字符串中
        for (int i = 0; i < matrixColumnLength; i++) {

            for(u8 k=0; k<matrixTypeLength; k++) {

                strcat(json, "[");
							
					 		 
                snprintf(float_str,Float_Str_Size, "%.3f", mdata[i+j*matrixColumnLength+k]); // 格式化float为字符串
                strcat(json, float_str); // 将float字符串追加到JSON字符串中


                if (k < matrixTypeLength - 1) {
                    strcat(json, ",");
                }
                strcat(json, "]");
 
            }
          
 
        
						  if (i < matrixColumnLength - 1) {
                strcat(json, ",");
            }
							
        }
				    strcat(json, "]");
        if (j < matrixRowLength - 1) {
            strcat(json, ",");
        }


    }

    strcat(json, "],\"sd\":[");

    // 将float数组的元素追加到JSON字符串中
    for (int i = 0; i < sTypeLength; i++) {

        char float_str[20]; // 适当选择足够大的缓冲区来存储float的字符串表示
        snprintf(float_str, Float_Str_Size, "%.3f", sdata[i ]); // 格式化float为字符串
        strcat(json, float_str); // 将float字符串追加到JSON字符串中
        if (i < sTypeLength - 1) {
            strcat(json, ",");
        }

    }

    strcat(json, "],\"msg\":\"");
    strcat(	json,errMsg);

    strcat(json, "\"");

    // 完成JSON字符串
    strcat(json, " }&hmnst");


    return  strlen(json);

}
