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
#include "ys_uart.h"
#include "ys_rom.h"

device_cfg_t device_config;

void ys_rom_load_device_config(device_cfg_t *cfg)
{
	u8 recover_flag = 0;

	flash_read_page(CFG_ADDR_DEVICE, sizeof(device_cfg_t), (u8*)cfg);

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

