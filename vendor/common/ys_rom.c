/********************************************************************************************************
 * @file     ys_rom.c
 *
 * @brief    user data rom application functions
 *
 * @author	 ysshen
 * @date     2021.05
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/
#include "../../proj/tl_common.h"
#include "../../proj_lib/rf_drv_8266.h"
#include "../../proj_lib/ble/blt_config.h"
#include "ys_uart.h"
#include "ys_rom.h"

device_cfg_t device_config;

static void ascii_to_hex(u8 *rp, u8 *sp, u8 len)
{
    static const u8 hex[16] = { 
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    u8 n = 0;
    for(u8 i = 0; i < len; i++)
    {
        rp[n++] = hex[sp[i] >> 4];
        rp[n++] = hex[sp[i] & 0x0F];
    }
}

void ys_rom_load_device_config(device_cfg_t *cfg)
{
	u8 recover_flag = 0;

	flash_read_page(CFG_ADDR_DEVICE, sizeof(device_cfg_t), (u8*)cfg);

	if(cfg->dev_name[0] >= U8_MAX)
	{
		recover_flag++;
		memset(cfg->dev_name, 0, sizeof(cfg->dev_name));

		u16 mac16 = *(volatile u16 *)CFG_ADR_MAC;
        strncpy((char *)cfg->dev_name, STRINGIFY(PRODUCT_NAME), strlen(STRINGIFY(PRODUCT_NAME)));
        ascii_to_hex(cfg->dev_name + strlen(STRINGIFY(PRODUCT_NAME)), (u8 *)&mac16, 2);
		cfg->dev_name[19] = 0;		
	}
	
	if(cfg->tx_power > RF_POWER_8dBm)
	{
		recover_flag++;
		cfg->tx_power = RF_POWER_8dBm;
	}

	if(cfg->baudrate > UART_BAUD_230400)
	{
		recover_flag++;
		cfg->baudrate = UART_BAUD_115200;
	}

	if(recover_flag)
	{
    	flash_erase_sector(CFG_ADDR_DEVICE);
    	sleep_us(10);
		flash_write_page(CFG_ADDR_DEVICE, sizeof(device_cfg_t), (u8*)cfg);
	}
}

void ys_rom_save_device_config(device_cfg_t *cfg)
{
	flash_erase_sector(CFG_ADDR_DEVICE);
    flash_write_page(CFG_ADDR_DEVICE, sizeof(device_cfg_t), (u8 *)cfg);
}

