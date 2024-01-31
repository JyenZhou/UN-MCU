/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-02-23 14:12:32
 * @Disclaimer:
 *  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 *  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 *  * TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 *  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 *  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 *  * Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved.
 */
#include "tc04xx_dma.h"

#define DEFAULT_DRCMR_VALUE         0x0
#define DEFAULT_DCSR_VALUE          0x8
#define DEFAULT_DCMD_VALUE          0x0

/**
 * @description: 
 * @param {uint8_t} channel
 * @param {uint8_t} request
 * @return {*}
 */
void DMA_DeInit(uint8_t channel,uint8_t request)
{
    if(request <= DMA_REQ_SARADC)
    {
        DMA->DRCMR[request] = DEFAULT_DRCMR_VALUE;
    }

    if(channel <=DMA_CHANNEL_2)
    {
        DMA->DCSR[channel] = DEFAULT_DCSR_VALUE;
        DMA->DESC_F[channel].DCMD = DEFAULT_DCMD_VALUE;

        DMA->DESC_F[channel].DSADR = 0x0;
        DMA->DESC_F[channel].DTADR = 0x0;
    }
}

/**
 * @description: 
 * @param {DMA_INIT_T} *dma_init_t
 * @return {*}
 */
void DMA_Init(DMA_INIT_T *dma_init_t)
{
    if(dma_init_t->request <= DMA_REQ_SARADC)
    {
        DMA->DRCMR_F[dma_init_t->request].DRCMR_MAPVLD = 1;
        DMA->DRCMR_F[dma_init_t->request].DRCMR_CHLNUM = dma_init_t->channel;
    }
    if(dma_init_t->channel <=DMA_CHANNEL_2)
    {
        // clear all interrupt bits
        DMA->DCSR_F[dma_init_t->channel].DCSR_BUSERRINTR = 1;
        DMA->DCSR_F[dma_init_t->channel].DCSR_ENDINTR = 1;
        DMA->DCSR_F[dma_init_t->channel].DCSR_STOPINTR = 1;
        DMA->DCSR_F[dma_init_t->channel].DCSR_RASINTR = 1;

        DMA->DCSR_F[dma_init_t->channel].DCSR_ENABLE = 1;

        DMA->DALGN_F[dma_init_t->channel].DALGN_DALGN = dma_init_t->align;


        if (dma_init_t->dma_dir == DMA_PERIPH_TO_MEM)
        {
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWTRG = FALSE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWSRC = TRUE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCTRGADDR = TRUE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCSRCADDR = FALSE;
        }
        else if (dma_init_t->dma_dir == DMA_MEM_TO_PERIPH)
        {
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWTRG = TRUE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWSRC = FALSE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCTRGADDR = FALSE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCSRCADDR = TRUE;
        }
        else
        {
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWTRG = FALSE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_FLOWSRC = FALSE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCTRGADDR = TRUE;
            DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_INCSRCADDR = TRUE;
        }

        DMA->DESC_F[dma_init_t->channel].DSADR = dma_init_t->src_addr;
        DMA->DESC_F[dma_init_t->channel].DTADR = dma_init_t->tgt_addr;
        DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_LEN = dma_init_t->len & 0x1FFFU;
        DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_WIDTH = dma_init_t->periph_width;
        DMA->DESC_F[dma_init_t->channel].DCMD_F.DESC_BURST = dma_init_t->burst;   
    }
}
