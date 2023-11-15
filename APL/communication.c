/**
  ****************************communication.c******************************************
  * @brief     通信相关
  *
    ******************************************************************************
  * @file     communication.c
  * @author   Jyen
  * @version  v1.0
  * @date     2023-09-11
  * @attention
  * This software is supplied under the terms of a license
  * agreement or non-disclosure agreement.
  * Passing on and copying of this document,and communication
  * of its contents is not permitted without prior written
  * authorization.
  *
  *         (C) Copyright 2023,Jyen,China. All Rights Reserved
  ********************************************************************************
  */



#include "communication.h"
#include "gatt.h"




void sendGASVoltage2NetUart (float* vt,u8 length,u8 ads1115Status)

{
    JHAL_uidGetLow();
    char json[350]; // 适当选择足够大的缓冲区

    char emsg[10]="";

    if(ads1115Status!=0)
    {
        sprintf(emsg, "ad%d err",ads1115Status);
    }
    size_t jLength =  JHAL_jsonPack2HmnstHIS (   json,  ProductID, vt,1,3,10, NULL,0,emsg);
    // 打印JSON字符串
    JHAL_uartSendDatas(&uartConfig,(u8 *)json,jLength,JHAL_CRC_Mode_NONE);
}







int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
            strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}



u8  parseJsonData(u8 netUartDataLength)
{

    int r;
    jsmntok_t tokens[10];

    jsmn_parser p;
    r = jsmn_parse(&p,(char *) netUartData,  netUartDataLength, tokens, 10);
    if (r < 0) {
        // printf("Failed to parse JSON: %d\n", r);
        return  0;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || tokens[0].type != JSMN_OBJECT) {
        //  printf("Object expected\n");
        return  0;
    }

    // 遍历令牌以获取字段值
    for (int i = 1; i < r; i += 2) {
        jsmntok_t key = tokens[i];
        jsmntok_t value = tokens[i + 1];

        // 根据字段名称来判断是哪个字段
        if (jsoneq((char *)netUartData, &key, "cmd") == 0) {
            // 用于存储cmd的值，假设不会超过50个字符

            char cmdValue[50];
            snprintf(cmdValue, sizeof(cmdValue), "%.*s", value.end - value.start, netUartData + value.start);

            if (strcmp(cmdValue, "getV") == 0) {
                return  CMD_GetVoltage;

            }

        } else if (jsoneq((char *)netUartData, &key, "ok") == 0) {
            if (netUartData[value.start] == 't') {
                printf("ok: true\n");
            } else {
                printf("ok: false\n");
            }
        } else if (jsoneq((char *)netUartData, &key, "value") == 0) {
            int intValue = atoi((char *)netUartData + value.start);
            printf("value: %d\n", intValue);
        }
    }
    return 0;
}

u8  parseNetUartData()
{
    u8 netUartDataLength=0;
    if( (netUartDataLength=JHAL_uartRxFinsh(&uartConfig))!=0)
    {
        if(netUartDataLength==8 )
        {
            JHAL_bootParseUpdateInfo4App(netUartData,ProductID,BootStatus_Update4Uart);
        } else {
            //   parseJsonData(netUartDataLength);
        }


        JHAL_uartEnableReceiveIT(&uartConfig);
    }
    return 0;
}


