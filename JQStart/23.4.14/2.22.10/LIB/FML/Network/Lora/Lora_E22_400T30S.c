
#include "lora_e22_400t30s.h"




/*���ڽ������ó�9600 ����Ȼģ��֧�ֶನ���ʣ������ƼĴ���ֻ������������ʣ������ڿ��Ƹ�ģ��ʱҪ�л�����  */



















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









/*lora AUX ״̬�������  �͵�ƽʱ��������
�ߵ�ƽʱ����  ����2ms���л�����ģʽ �Ż�������Ч �����ӳ�ִ��
��Ҫ��һЩ����ʱ��Ҫ��� ���ʱ��ֱ������ ��ʱ����ʱ
*/
static void lora_waitIdle()
{
    while(!HAL_GPIO_ReadPin(Lora_E22_400T30S_AUX_GPIO_Port,Lora_E22_400T30S_AUX_Pin))
    {
        HAL_Delay(5);
    }
}


//mode����ģʽ �����ڲ�ע��
//isWaitBusy  1�ȴ� �Ƿ�ȴ��������  ����ģʽ�л�ֻ���� AUX ��� 1 ��ʱ����Ч��������ӳ��л���
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
//����ģʽM1:0 M0:0 ���ڴ򿪣����ߴ򿪣�͸������ ��֧������ָ���������
//���� �û�����ͨ�������������ݣ�ģ����������߷��䡣
//���� ģ�����߽��չ��ܴ򿪣��յ��������ݺ��ͨ������ TXD ���������
    case 0:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_RESET);
        break;
// WORģʽM1:0 M01 ���Զ���Ϊ WOR ���ͷ��� WOR ���շ���֧�ֿ��л���
//���� ������Ϊ���䷽ʱ������ǰ���Զ�����һ��ʱ��Ļ�����
//���� ���������������ݣ����չ��ܵ�ͬ��ģʽ 0
    case 1:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_SET);
        break;
//����ģʽM1:1 M0:0   �û���ͨ�����ڶԼĴ������з��ʣ��Ӷ�����ģ�鹤��״̬
//���� ���߷���ر�
//���� ���߽��չر�
//���� �û����Է��ʼĴ������Ӷ�����ģ�鹤��״̬
    case 2:
        HAL_GPIO_WritePin(Lora_E22_400T30S_M1_GPIO_Port,Lora_E22_400T30S_M1_Pin,GPIO_PIN_SET);
        HAL_GPIO_WritePin(Lora_E22_400T30S_M0_GPIO_Port,Lora_E22_400T30S_M0_Pin,GPIO_PIN_RESET);
        break;
//����ģʽM1:0 M0:0  ģ���������
//���� ���߷���ر�
//���� ���߽��չر�
//ע�� ��������ģʽ���뵽����ģʽ��ģ����������ò��������ù����У� AUX ���ֵ͵�ƽ����Ϻ�����ߵ�ƽ�����Խ����û���� T_BUSY �����ء�
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


/*ģ���ַ 0-65535 ��ģ���ַ���� FFFF ʱ������Ϊ�㲥�ͼ�����ַ��������ʱģ�齫�����е�ַ����
  �����ַ(Ŀ���ַ) 0-255  �����ַ�������������磻�໥ͨ��ʱ��Ӧ����Ϊ��ͬ��
	Ƶ���ŵ�CH 0��83  ʵ��Ƶ��= 410.125 + CH *1M
  �������� �û�ͨ��ָ�����ڷ��ͳ�ȥ
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
//ͬʱ����ģ���ַ�� 0x1234���������ַ(0x00)������(9600 8N1)������(1.2K)
    baseWriteRegisterData[3]=(uint8)(deviceAddress>>8);
    baseWriteRegisterData[4]=(uint8)deviceAddress;
    baseWriteRegisterData[5]=networkAddress;
//��������Ĭ�� ����2.4k  0110 0010  19.2K   0110 0101   62.5K  0110 0111
// 1110 0111
    baseWriteRegisterData[6]=0xe7;
//04H���ֲ���
    baseWriteRegisterData[7]=0x00;
    baseWriteRegisterData[8]=channel;
//����RSSI  1000 0000
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



















