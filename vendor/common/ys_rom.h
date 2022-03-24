/********************************************************************************************************
 * @file     ys_rom.h
 *
 * @brief    user data rom application header file
 *
 * @author	 ysshen
 * @date     2021.05
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _YS_ROM_H_
#define _YS_ROM_H_

typedef struct
{
	u8  dev_name[20];	//00~13
	u8	tx_power;		//14
	u8  baudrate;		//15
	u8  reserved0;		//16
	u8  reserved1;		//17

} device_cfg_t;


extern device_cfg_t device_config;

extern void ys_rom_load_device_config(device_cfg_t *cfg);
extern void ys_rom_save_device_config(device_cfg_t *cfg);


#endif /* _YS_ROM_H_ */
