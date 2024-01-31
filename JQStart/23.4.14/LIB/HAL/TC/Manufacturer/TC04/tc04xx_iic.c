/*
 * @Description: 
 * @Author: wangshi@tinychip.com.cn
 * @Date: 2023-03-08 13:55:27
 * @Disclaimer: 
 * 	* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
 * 	* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
 * 	* TIME. AS A RESULT, TINY CHIP SHALL NOT BE HELD LIABLE FOR ANY
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
 * 	* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
 * 	* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * 	* Copyright (c) 2020 Tinychip Microelectronics Co.,Ltd, All Rights Reserved. 
 */
#include "tc04xx_iic.h"

#define     DEFAULT_SDA_SETUP_TIME      0x04U
#define     DEFAULT_SDA_HOLD_TIME       0x04U
#define     DEFAULT_SPIKE_FILTER_CNUM   0x07U

#define     IIC_MASTER_RWN_WRITE        0x00U
#define     IIC_MASTER_RWN_READ         0x01U

#define     IIC_MAX_TIMEOUT_CNT         3000U

#define     IIC_BYTE_NUM_SET(n)         (((uint32_t)(n) << 23 ) & 0xFF800000U)
#define     IIC_STOP_SET(n)             (((uint32_t)(n) << 17 ) & 0x00020000U)
#define     IIC_RESTART_SET(n)          (((uint32_t)(n) << 16 ) & 0x00010000U)
#define     IIC_START_SET(n)            (((uint32_t)(n) << 15 ) & 0x00008000U)
#define     IIC_TARGET_RWN_SET(n)       (((uint32_t)(n) << 14 ) & 0x00004000U)
#define     IIC_TARGET_SLAVE_ADDR_MODE_SET(n)    (((uint32_t)(n) << 13 ) & 0x00002000U)
#define     IIC_TARGET_SLAVE_ADDR_SET(n)  (((uint32_t)(n) << 0  ) & 0x000003FFU)

/**
 * @description: 
 * @param {uint8_t} clk_src
 * @param {uint8_t} clk_div
 * @return {*}
 */
void IIC_SetFclkSource(uint8_t clk_src, uint8_t clk_div)
{
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_RST_I2C0 = 1;
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_PCLK_EN_I2C0 = 1;
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_FCLK_EN_I2C0 = 1;
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_FCLK_SEL_I2C0 = clk_src;
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_FCLK_DIV_I2C0 = clk_div;
    SYSCFG->I2C0_CLKRST_CTRL_F.SW_RST_I2C0 = 0;
}

/**
 * @description: 
 * @param {IIC_INIT_T *} iic_init_t
 * @return {*}
 */
void IIC_Init(IIC_INIT_T * iic_init_t)
{
    uint32_t freq = 0;
    uint8_t scl_period = 0;


    if(SYSCFG->I2C0_CLKRST_CTRL_F.SW_FCLK_SEL_I2C0 == CLOCK_SEL_16M)
    {
        freq = 16000000;
    }
    else if(SYSCFG->I2C0_CLKRST_CTRL_F.SW_FCLK_SEL_I2C0 == CLOCK_SEL_32M)
    {
        freq = 32000000;
    }
    else
    {
        return;
    }

    if(iic_init_t != NULL)
    {
        if (iic_init_t->mode == IIC_MODE_MASTER)
        {
            if(iic_init_t->rate == IIC_RATE_400K)
            {
                scl_period = (uint8_t)((freq / 400000 - 2*(DEFAULT_SPIKE_FILTER_CNUM+3))/2);
            }
            else if(iic_init_t->rate == IIC_RATE_100K)
            {
                scl_period = (uint8_t)((freq / 100000 - 2*(DEFAULT_SPIKE_FILTER_CNUM+3))/2);
            }
            else 
            {
                scl_period = (uint8_t)(((freq / 20000 - 2 * (DEFAULT_SPIKE_FILTER_CNUM + 3)) / 2) & 0xFFU);
            }
            IIC->MCR_F.CONTROL_MODE = iic_init_t->auto_mode;
            
            IIC->TPR_F.SCLL = scl_period;
            IIC->TPR_F.SCLH = scl_period;
            IIC->TPR_F.SDA_SETUP_TIME = DEFAULT_SDA_SETUP_TIME;
            IIC->TPR_F.SDA_HOLD_TIME = DEFAULT_SDA_HOLD_TIME;
            IIC->TPR_F.SPIKE_FILTER_CNUM = DEFAULT_SPIKE_FILTER_CNUM;

        }
        else 
        {
            if(iic_init_t->addr_mode == IIC_ADDR_MODE_7BIT)
            {
                IIC->SAR_F.SLAVE_ADDR = (iic_init_t->addr&0x7FU) << 1;
            }
            else
            {
                IIC->SAR_F.SLAVE_ADDR = iic_init_t->addr&0x3FFFU;
            }
            IIC->SAR_F.SLAVE_ADDR_MODE = iic_init_t->addr_mode;
            IIC->SAR_F.SLAVE_ADDR_EN = 1;
            IIC->MCR_F.CONTROL_MODE = iic_init_t->auto_mode;
            IIC->MCR_F.DISABLE_MASTER = 1;
        }

    }

}

/**
 * @description: 
 * @param {uint16_t} slave_addr
 * @param {uint8_t} addr_mode
 * @param {uint8_t} *data
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_Master_Send(uint16_t slave_addr,uint8_t addr_mode,uint8_t *data,uint16_t size)
{
    uint16_t idx = 0;
    uint16_t timeout = 0;

    if(addr_mode == IIC_ADDR_MODE_7BIT)
    {
        slave_addr = (slave_addr&0x7FU)<<1; 
    }
    else 
    {
        slave_addr = (slave_addr&0x3FFU);
    } 

    IIC->SCR = IIC_BYTE_NUM_SET(size-1) | IIC_STOP_SET(0x01) | IIC_RESTART_SET(0x01) | \
    IIC_START_SET(0x01) | IIC_TARGET_RWN_SET(IIC_MASTER_RWN_WRITE) |\
    IIC_TARGET_SLAVE_ADDR_MODE_SET(addr_mode) | IIC_TARGET_SLAVE_ADDR_SET(slave_addr);

    while(idx < size)
    {
        if(IIC->FSR_F.TX_FIFO_FREE_NUM > 0)
        {

            IIC->TRDR_F.DATA = data[idx++];
            timeout = 0;
        }
        else
        {
            if(timeout++ > IIC_MAX_TIMEOUT_CNT)
            {
                break;
            }
        }

    }
}
/**
 * @description: 
 * @param {uint16_t} slave_addr
 * @param {uint8_t} addr_mode
 * @param {uint8_t} *data
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_Master_Recv(uint16_t slave_addr,uint8_t addr_mode,uint8_t *data,uint16_t size)
{
    uint16_t idx = 0;
    uint16_t timeout = 0;

    if(addr_mode == IIC_ADDR_MODE_7BIT)
    {
        slave_addr = (slave_addr&0x7FU)<<1; 
    }
    else 
    {
        slave_addr = (slave_addr&0x3FFU);
    }

    IIC->SCR = IIC_BYTE_NUM_SET(size-1) | IIC_STOP_SET(0x01) | IIC_RESTART_SET(0x01) | \
    IIC_START_SET(0x01) | IIC_TARGET_RWN_SET(IIC_MASTER_RWN_READ) | \
    IIC_TARGET_SLAVE_ADDR_MODE_SET(addr_mode) | IIC_TARGET_SLAVE_ADDR_SET(slave_addr);


    while(idx < size)
    {
        if(IIC->FSR_F.RX_FIFO_DATA_NUM > 0)
        {
            data[idx++] = IIC->TRDR_F.DATA;
            timeout = 0;
        }
        else
        {
            if(timeout++ > IIC_MAX_TIMEOUT_CNT)
            {
                break;
            }
        }

    }
}
/**
 * @description: 
 * @param {uint8_t} *data
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_Slave_Send(uint8_t *data,uint16_t size)
{
    uint16_t idx = 0;
    uint16_t timeout = 0;

    while(idx < size)
    {
        if(IIC->FSR_F.TX_FIFO_FREE_NUM > 0)
        {
            IIC->TRDR_F.DATA = data[idx++];
            timeout = 0;
        }
        else
        {
            if(timeout++ > IIC_MAX_TIMEOUT_CNT)
            {
                break;
            }
        }
    }
}
/**
 * @description: 
 * @param {uint8_t} *data
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_Slave_Recv(uint8_t *data,uint16_t size)
{
    uint16_t idx = 0;
    uint16_t timeout = 0;

    while(idx < size)
    {
        if(IIC->FSR_F.RX_FIFO_DATA_NUM > 0)
        {
            data[idx++] = IIC->TRDR_F.DATA;
            timeout = 0;
        }
        else
        {
            if(timeout++ > IIC_MAX_TIMEOUT_CNT)
            {
                break;
            }
        }
    }
}
/**
 * @description: 
 * @param {uint16_t} slave_addr
 * @param {uint8_t} addr_mode
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_DmaTxSetup(uint16_t slave_addr,uint8_t addr_mode,uint16_t size)
{
    if(addr_mode == IIC_ADDR_MODE_7BIT)
    {
        slave_addr = (slave_addr&0x7FU) <<1;
    }
    else
    {
        slave_addr = (slave_addr&0x3FFU);
    }
    
    IIC->SCR = IIC_BYTE_NUM_SET(size-1) | IIC_STOP_SET(0x01) | IIC_RESTART_SET(0x01) | \
    IIC_START_SET(0x01) | IIC_TARGET_RWN_SET(IIC_MASTER_RWN_WRITE) |\
    IIC_TARGET_SLAVE_ADDR_MODE_SET(addr_mode) | IIC_TARGET_SLAVE_ADDR_SET(slave_addr);

    IIC_TXDMA_ENABLE();
}
/**
 * @description: 
 * @param {uint16_t} slave_addr
 * @param {uint8_t} addr_mode
 * @param {uint16_t} size
 * @return {*}
 */
void IIC_DmaRxSetup(uint16_t slave_addr,uint8_t addr_mode,uint16_t size)
{
    if(addr_mode == IIC_ADDR_MODE_7BIT)
    {
        slave_addr = (slave_addr&0x7FU) <<1;
    }
    else
    {
        slave_addr = (slave_addr&0x3FFU);
    }

    IIC->SCR = IIC_BYTE_NUM_SET(size-1) | IIC_STOP_SET(0x01) | IIC_RESTART_SET(0x01) | \
    IIC_START_SET(0x01) | IIC_TARGET_RWN_SET(IIC_MASTER_RWN_READ) | \
    IIC_TARGET_SLAVE_ADDR_MODE_SET(addr_mode) | IIC_TARGET_SLAVE_ADDR_SET(slave_addr);

    IIC_RXDMA_ENABLE();
}
