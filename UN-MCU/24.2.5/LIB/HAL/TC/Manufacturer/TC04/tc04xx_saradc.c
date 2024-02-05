/*
 * @Description:
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-01-19 10:17:14
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
#include "tc04xx_saradc.h"

#define     DEFAULT_WAIT_CYCLE              0x15U
#define     PGA_CHOP_CLR_BIT                0x180U
#define     PGA_CHOP_SET_BIT                0x100U


/**
 * @description:
 * @param {uint8_t} clk_div
 * @return {*}
 */
void SARADC_SetFclkSource(uint8_t clk_div)
{
    // FCLK for SAR, default is using 16M and fclk is enable
    SYSCFG->SARADC_CLKRST_CTRL_F.SW_PCLK_EN_SARADC = 1;
    SYSCFG->SARADC_CLKRST_CTRL_F.SW_FCLK_EN_SARADC = 1;
    SYSCFG->SARADC_CLKRST_CTRL_F.SW_FCLK_SEL_SARADC = 0;
    SYSCFG->SARADC_CLKRST_CTRL_F.SW_FCLK_DIV_SARADC = clk_div;
}
/**
 * @description:
 * @param {SARADC_CFG_T} *saradc_init_t
 * @return {*}
 */
void SARADC_Init(SARADC_INIT_T *saradc_init_t)
{
    if(saradc_init_t != NULL)
    {
        // SARADC->ADC_CONF0_F.CH_PAIR_SEL = saradc_init_t->pair_sel;
        SARADC->ADC_CONF0_F.FIFO_FORMAT = saradc_init_t->fifo_format;
        SARADC->ADC_CONF0_F.FIFO_THRHLD = saradc_init_t->fifo_threshold;

        SARADC->ADC_CONF1_F.SMP_CYCLE = saradc_init_t->smp_cycle;
        SARADC->ADC_CONF1_F.WAIT_CYCLE = DEFAULT_WAIT_CYCLE;
        SARADC->ADC_CONF1_F.CHNL_AVG_EN = saradc_init_t->smp_avg_en;
        SARADC->ADC_CONF1_F.TRIG_NUM = saradc_init_t->trig_cnt;
        SARADC->ADC_CONF1_F.CHNL_SAMP_NUM = saradc_init_t->smp_ch_cycle;
        SARADC->ADC_CONF1_F.CHNL_SAMP_NUM_DUMMY = saradc_init_t->smp_dummy_cycle;

        SARADC->ADC_ANA_TOP_F.CTRL_SAR_MUX_INP = saradc_init_t->p_ch;
        SARADC->ADC_ANA_TOP_F.CTRL_SAR_MUX_INN = saradc_init_t->n_ch;
        SARADC->ADC_ANA_TOP_F.CTRL_SAR_MUX_PGA = (saradc_init_t->pga_en != 0) ? (uint8_t)SARADC_OUTPUT_PGA : (uint8_t)SARADC_OUTPUT_PASS_PGA;
    }
}
/**
 * @description:
 * @param {SARADC_PGA_CFG_T} *saradc_pga_init_t
 * @return {*}
 */
void SARADC_PGA_Init(SARADC_PGA_INIT_T *saradc_pga_init_t)
{
    if(saradc_pga_init_t != NULL)
    {
        ADC_ADC_PGA_CHNL_FIELD_T   *pga_chan_cfg = (ADC_ADC_PGA_CHNL_FIELD_T *)SARADC_PGA_CHANNEL_SEL(saradc_pga_init_t->channel);

        pga_chan_cfg->CTRL_PGA_STG1_GAIN = saradc_pga_init_t->pga_stg1_gain;
        pga_chan_cfg->CTRL_PGA_STG2_GAIN = saradc_pga_init_t->pga_stg2_gain;

        SARADC->ADC_PGA_CTRL_F.CTRL_PGA_20_8 |= (saradc_pga_init_t->pga_stg1_power | ((uint16_t)saradc_pga_init_t->pga_stg2_power << 2));

        SARADC->ADC_PGA_CTRL_F.CTRL_PGA_20_8 &= ~PGA_CHOP_CLR_BIT;
        SARADC->ADC_PGA_CTRL_F.CTRL_PGA_20_8 |= PGA_CHOP_SET_BIT; //bug:pga chop resetb control bit, low active
    }
}
/**
 * @description:
 * @param {uint8_t} channel
 * @param {uint8_t} stg1_gain
 * @param {uint8_t} stg2_gain
 * @return {*}
 */
void SARADC_PGA_Config(uint8_t channel, uint8_t stg1_gain, uint8_t stg2_gain)
{
    ADC_ADC_PGA_CHNL_FIELD_T   *pga_chan_cfg = (ADC_ADC_PGA_CHNL_FIELD_T *)SARADC_PGA_CHANNEL_SEL(channel);

    pga_chan_cfg->CTRL_PGA_STG1_GAIN = stg1_gain;
    pga_chan_cfg->CTRL_PGA_STG2_GAIN = stg2_gain;
}
/**
 * @description:
 * @param {uint8_t} level
 * @param {uint8_t} en
 * @return {*}
 */
void SARADC_LDOVS_Config(uint8_t level, uint8_t en)
{
    ANAREG->LDO3VS_TOP_F.CTRL_LDO3VS_VOL_SEL = level;
    ANAREG->LDO3VS_TOP_F.CTRL_LDO3VS_0_ENABLE = en;
}
/**
 * @description:
 * @param {uint8_t} channel
 * @param {SARADC_IDAC_CFG_T *} cfg
 * @return {*}
 */
void SARADC_IDAC_Config(uint8_t channel, SARADC_IDAC_CFG_T *cfg)
{
    if(cfg != NULL)
    {
        ADC_ADC_PGA_CHNL_FIELD_T   *pga_chan_cfg = (ADC_ADC_PGA_CHNL_FIELD_T *)SARADC_PGA_CHANNEL_SEL(channel);

        pga_chan_cfg->CTRL_IDAC1_CURRENT = cfg->idac1Cur;
        pga_chan_cfg->CTRL_IDAC1_CURRENT_DIR = cfg->idac1CurDir;

        pga_chan_cfg->CTRL_IDAC2_CURRENT_DIR = cfg->idac2CurDir;
        pga_chan_cfg->CTRL_IDAC2_CURRENT = cfg->idac2Cur;
    }
}


/**
 * @description:
 * @param {SARADC_SCAN_INIT_T *} p_init_t
 * @return {*}
 */
void SARADC_ScanModeInit(SARADC_SCAN_INIT_T *scan_init_t)
{
    if(scan_init_t != NULL)
    {
        SARADC->ADC_CONF1_F.SCAN_CHNUM = scan_init_t->pair_num;
        SARADC->ADC_CONF0_F.CH_PAIR_SEL = scan_init_t->pair_sel;
        SARADC->ADC_CONF_PAIR_F.CH_PAIR0 = scan_init_t->pair0_cfg;
        SARADC->ADC_CONF_PAIR_F.CH_PAIR1 = scan_init_t->pair1_cfg;
        SARADC->ADC_CONF_PAIR_F.CH_PAIR2 = scan_init_t->pair2_cfg;
        SARADC->ADC_CONF_PAIR_F.CH_PAIR3 = scan_init_t->pair3_cfg;
    }
}

/**
 * @description:
 * @param {uint8_t} level
 * @param {uint8_t} en
 * @return {*}
 */
void SARADC_VSDIV_Config(uint8_t level, uint8_t en)
{
    ANAREG->LDO3VS_TOP_F.CTRL_LDO3VS_DIV_SEL = level;
    ANAREG->LDO3VS_TOP_F.CTRL_LDO3VS_DIV_ENABLE = en;
}
