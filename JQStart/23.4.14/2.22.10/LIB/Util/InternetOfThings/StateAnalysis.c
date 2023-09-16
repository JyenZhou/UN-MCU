#include "stateanalysis.h"
#include "system.h"
/*设备登录ID和校验码*/
//用char* 不能禁止直接通过地址操作内容  必须手动申请一个连续内存  或者操作指针地址  麻烦  这里要uid转字符这样方便
//因为uid是96位的 所以大小为24（8位单片机一次就只能处理8位二进制0x00数组两位 (96/8*2)） 理论上24b就够了  但是
//2b的里面之前如果是AA之类的 转字符串后就要多一个  这里以最坏的算 即24*3/2 =36 其中最后一个是"/0"
char DeviceName[40]="HMNST_RD20201001";
char* DeviceKey="HMNST_8004_TEST001_CRC";
char* ProjectName="RD20201001" ;
uint8 ISLogin=0;//登录状态
char* UserName ="Unattended";

/*-----------------------------------------------------------------------*/
/**
  *@ 函数功能或简介: 消息解析之登录状态
  * @输入参数:字符串  已经是从json对象中提取出来的了
  * @返 回 值: 0代表需要登录
  * @重点说明：无
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
uint8 LoginState (char* MsgType)
{
//	   message_Json=cJSON_Parse(RxBuffer1);//JSon整体对象解释
//    if (NULL == message_Json)
//    {
//        return ;
//    }
//	 //1.获取消息类型  是系统的  还是用户的透传
//    MsgType=jxjsonObjectToString(message_Json,"MsgType");
    //登录有关

    if((strcmp(MsgType,"loginOK")==0))
    {
        //已登录
        return 1;
    }
    else  //((strcmp(MsgType,"loginError")==0))
    {
        //需要登录
        return 0;
    }



}


/*
/////////////////上面是接收解析时用的  下面是发送时候用的
*/





void Login()
{
    char str[180]= {0}; //这个长度要大于等于实际字符串长度 不然会丢失的   
    char* str_login1="{\"ConnectionType\":\"device\",\"Device\":\"login\",\"DeviceName\":\"";
    char* str_login2=	DeviceName;
    char* str_login3=	"\",\"DeviceKey\":\"";
    char* str_login4=	DeviceKey;
    char* str_login5=	"\",\"ProjectName\":\"";
    char* str_login6=	ProjectName;

    char* str_login="\"}";
    strcpy (str,str_login1);
    strcat(str,str_login2);
    strcat(str,str_login3);
    strcat(str,str_login4);
    strcat(str,str_login5);
    strcat(str,str_login6);
    strcat(str,str_login);

    if(( strlen(str))>(sizeof(str)))
    {

        //    __breakpoint(0);  //数组越界

    }

    /*登录延时3秒  1是为了等待服务端回应  没必要重复登录 2.在串口DMA模式下延时不能加在调用方函数中否则发出去数据不完整且异常（未解其原因） */
    sendDataForUarttoNetworkport(str);

}

//发送心跳包给上位机
void sendHeartbeat()
{
    char str[60]= {0}; //这个长度要大于等于实际字符串长度 不然会丢失的

    char* str_heartbeat="{\"ConnectionType\":\"device\",\"Device\":\"sendHeartbeat\"}";
    strcpy (str,str_heartbeat);
    if(( strlen(str))>(sizeof(str)))
    {

        //    __breakpoint(0);  //数组越界

    }

    sendDataForUarttoNetworkport(str);
}



/*-----------------------------------------------------------------------*/
/**
*@ 函数功能或简介: json嵌套数据拼接--发送设备采集数据部分
* @输入参数: 要发送的设备数据
  * @返 回 值: 无
  * @重点说明：该函数是需要重复调用的 已完成json的嵌套
  * @作者：Jyen Z
  *
  ******************************************************************************
  *
  *----------------------作者Jyen---------------------作者Jyen--------------------------*/
void json_dataSynthesis (char data[])
{
    /*发送的字符串 总缓冲区 和长度  注意这个长度不要小于传来的数据长度*/
    char str[530]= {0};
    //拼接数据头
    char* sensorData1="{\"ConnectionType\":\"device\",\"Device\":\"deviceData\",\"DeviceName\":\"";
    char* sensorData2=	DeviceName;
    char* data6=	"\",\"ProjectName\":\"";
    char* data7=	ProjectName;
    char* data8=	"\",\"UserName\":\"";
    char* sensorData5=	"\",\"DeviceData\":\"";

    strcpy (str,sensorData1);

    strcat(str,sensorData2);
    strcat(str,data6);
    strcat(str,data7);
		   strcat(str,data8);
    strcat(str,UserName);
    //拼接数据
    strcat(str,sensorData5);
    strcat(str,data);
    strcat(str,"\"}");



    sendDataForUarttoNetworkport(str);
}

void sendDataForUarttoNetworkport(char dat[])
{

    char* Encode="_$Jyen&_";
    strcat(dat,Encode);
    //字符串的长度  去除未赋值的空数组后的长度
    uint16 stringleng=strlen(dat);
 //   sendDat_uart2(dat,stringleng);
//  dma_sendDat(str,stringleng);
//	HAL_UART_AbortReceive(&huart1);
//	printf(str);
//  UART_Receive3_IT_EN();
//	HAL_Delay(100);

}







