#include "stateanalysis.h"
#include "system.h"
/*�豸��¼ID��У����*/
//��char* ���ܽ�ֱֹ��ͨ����ַ��������  �����ֶ�����һ�������ڴ�  ���߲���ָ���ַ  �鷳  ����Ҫuidת�ַ���������
//��Ϊuid��96λ�� ���Դ�СΪ24��8λ��Ƭ��һ�ξ�ֻ�ܴ���8λ������0x00������λ (96/8*2)�� ������24b�͹���  ����
//2b������֮ǰ�����AA֮��� ת�ַ������Ҫ��һ��  ����������� ��24*3/2 =36 �������һ����"/0"
char DeviceName[40]="HMNST_RD20201001";
char* DeviceKey="HMNST_8004_TEST001_CRC";
char* ProjectName="RD20201001" ;
uint8 ISLogin=0;//��¼״̬
char* UserName ="Unattended";

/*-----------------------------------------------------------------------*/
/**
  *@ �������ܻ���: ��Ϣ����֮��¼״̬
  * @�������:�ַ���  �Ѿ��Ǵ�json��������ȡ��������
  * @�� �� ֵ: 0������Ҫ��¼
  * @�ص�˵������
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
uint8 LoginState (char* MsgType)
{
//	   message_Json=cJSON_Parse(RxBuffer1);//JSon����������
//    if (NULL == message_Json)
//    {
//        return ;
//    }
//	 //1.��ȡ��Ϣ����  ��ϵͳ��  �����û���͸��
//    MsgType=jxjsonObjectToString(message_Json,"MsgType");
    //��¼�й�

    if((strcmp(MsgType,"loginOK")==0))
    {
        //�ѵ�¼
        return 1;
    }
    else  //((strcmp(MsgType,"loginError")==0))
    {
        //��Ҫ��¼
        return 0;
    }



}


/*
/////////////////�����ǽ��ս���ʱ�õ�  �����Ƿ���ʱ���õ�
*/





void Login()
{
    char str[180]= {0}; //�������Ҫ���ڵ���ʵ���ַ������� ��Ȼ�ᶪʧ��   
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

        //    __breakpoint(0);  //����Խ��

    }

    /*��¼��ʱ3��  1��Ϊ�˵ȴ�����˻�Ӧ  û��Ҫ�ظ���¼ 2.�ڴ���DMAģʽ����ʱ���ܼ��ڵ��÷������з��򷢳�ȥ���ݲ��������쳣��δ����ԭ�� */
    sendDataForUarttoNetworkport(str);

}

//��������������λ��
void sendHeartbeat()
{
    char str[60]= {0}; //�������Ҫ���ڵ���ʵ���ַ������� ��Ȼ�ᶪʧ��

    char* str_heartbeat="{\"ConnectionType\":\"device\",\"Device\":\"sendHeartbeat\"}";
    strcpy (str,str_heartbeat);
    if(( strlen(str))>(sizeof(str)))
    {

        //    __breakpoint(0);  //����Խ��

    }

    sendDataForUarttoNetworkport(str);
}



/*-----------------------------------------------------------------------*/
/**
*@ �������ܻ���: jsonǶ������ƴ��--�����豸�ɼ����ݲ���
* @�������: Ҫ���͵��豸����
  * @�� �� ֵ: ��
  * @�ص�˵�����ú�������Ҫ�ظ����õ� �����json��Ƕ��
  * @���ߣ�Jyen Z
  *
  ******************************************************************************
  *
  *----------------------����Jyen---------------------����Jyen--------------------------*/
void json_dataSynthesis (char data[])
{
    /*���͵��ַ��� �ܻ����� �ͳ���  ע��������Ȳ�ҪС�ڴ��������ݳ���*/
    char str[530]= {0};
    //ƴ������ͷ
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
    //ƴ������
    strcat(str,sensorData5);
    strcat(str,data);
    strcat(str,"\"}");



    sendDataForUarttoNetworkport(str);
}

void sendDataForUarttoNetworkport(char dat[])
{

    char* Encode="_$Jyen&_";
    strcat(dat,Encode);
    //�ַ����ĳ���  ȥ��δ��ֵ�Ŀ������ĳ���
    uint16 stringleng=strlen(dat);
 //   sendDat_uart2(dat,stringleng);
//  dma_sendDat(str,stringleng);
//	HAL_UART_AbortReceive(&huart1);
//	printf(str);
//  UART_Receive3_IT_EN();
//	HAL_Delay(100);

}







