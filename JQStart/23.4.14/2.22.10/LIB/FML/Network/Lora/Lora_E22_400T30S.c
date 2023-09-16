
#include "lora_e22_400t30s.h"




/*串口建议配置成9600 （虽然模块支持多波特率，但控制寄存器只接受这个波特率，否则在控制该模块时要切回来）  */



















void Lora_E22_400T30S_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port, Lora_E22_400T30S_M0_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port, Lora_E22_400T30S_M1_Pin, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = Lora_E22_400T30S_M0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(Lora_E22_400T30S_M0_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = Lora_E22_400T30S_M1_Pin;
    HAL_GPIO_Init(Lora_E22_400T30S_M1_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =Lora_E22_400T30S_AUX_Pin  ;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(Lora_E22_400T30S_AUX_GPIO_Port, &GPIO_InitStruct);
    lora_WokeModeSwitc(0);

}









/*lora AUX 状态检测引脚  低电平时接收数据
高电平时空闲  空闲2ms后切换工作模式 才会立即生效 否则延迟执行
主要在一些特殊时候要检查 大多时候直接设置 延时就延时
*/
static void lora_waitIdle()
{
    while(!HAL_GPIO_ReadPin(Lora_E22_400T30S_AUX_GPIO_Port,Lora_E22_400T30S_AUX_Pin))
    {
        HAL_Delay(5);
    }
}


//mode工作模式 详情内部注释
//isWaitBusy  1等待 是否等待设置完成  所以模式切换只能在 AUX 输出 1 的时候有效，否则会延迟切换。
//
void lora_WokeModeSwitc(uint8 mode)
{
    static uint8 currentMode=4;

    if(currentMode==mode)
    {
        return;
    } else {
        currentMode=mode;
    }


    switch (mode)
    {
//传输模式M1:0 M0:0 串口打开，无线打开，透明传输 。支持特殊指令空中配置
//发射 用户可以通过串口输入数据，模块会启动无线发射。
//接收 模块无线接收功能打开，收到无线数据后会通过串口 TXD 引脚输出。
    case 0:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_RESET);
        break;
// WOR模式M1:0 M01 可以定义为 WOR 发送方和 WOR 接收方。支持空中唤醒
//发射 当定义为发射方时，发射前会自动增加一定时间的唤醒码
//接收 可以正常接收数据，接收功能等同于模式 0
    case 1:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_SET);
        break;
//配置模式M1:1 M0:0   用户可通过串口对寄存器进行访问，从而控制模块工作状态
//发射 无线发射关闭
//接收 无线接收关闭
//配置 用户可以访问寄存器，从而配置模块工作状态
    case 2:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_RESET);
        break;
//配置模式M1:0 M0:0  模块进入休眠
//发射 无线发射关闭
//接收 无线接收关闭
//注意 当从休眠模式进入到其他模式，模块会重新配置参数，配置过程中， AUX 保持低电平；完毕后输出高电平，所以建议用户检测 T_BUSY 上升沿。
    case 3:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_SET);
        break;
    default:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_RESET);
        break;
    }
    HAL_Delay(100);
    lora_waitIdle();

}


/*模块地址 0-65535 当模块地址等于 FFFF 时，可作为广播和监听地址，即：此时模块将不进行地址过滤
  网络地址(目标地址) 0-255  网络地址，用于区分网络；相互通信时，应设置为相同。
	频率信道CH 0～83  实际频率= 410.125 + CH *1M
  返回数据 用户通过指定串口发送出去
*/
uint8 baseWriteRegisterData[Lora_baseConfigurationDataLength]= {0xC0,0x00,0x07};

void lora_Configuration(uint16 deviceAddress,uint8 networkAddress,uint8 channel)
{

    if(deviceAddress==0xffff)
    {
        deviceAddress-=1;
    }
    channel=channel%84;
    lora_WokeModeSwitc(2);
//同时配置模块地址（ 0x1234）、网络地址(0x00)、串口(9600 8N1)、空速(1.2K)
    baseWriteRegisterData[3]=(uint8)(deviceAddress>>8);
    baseWriteRegisterData[4]=(uint8)deviceAddress;
    baseWriteRegisterData[5]=networkAddress;
//串口配置默认 空速2.4k  0110 0010  19.2K   0110 0101   62.5K  0110 0111
// 1110 0111
    baseWriteRegisterData[6]=0xe7;
//04H保持不变
    baseWriteRegisterData[7]=0x00;
    baseWriteRegisterData[8]=channel;
//启用RSSI  1000 0000
    baseWriteRegisterData[9]=0x80;
 
 utilUart_sendDat_Uart(&utilUart_1_Handle,baseWriteRegisterData,Lora_baseConfigurationDataLength);
}

bool lora_cheakLastConfiguration(uint8 *data,uint8 dataLength)
{
    if(dataLength!=Lora_baseConfigurationDataLength)
    {
        return false;
    }

    if(data[0]!=0xC1)
    {
        return false;
    }
    for(uint8 i=1; i<Lora_baseConfigurationDataLength; i++)
    {
        if(data[i]!=baseWriteRegisterData[i])
        {
            return false;
        }
    }

    return true;
}



















