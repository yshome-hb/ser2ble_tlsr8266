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

static inline void ascii_to_hex(u8 *rp, u8 *sp, u8 len)
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

extern void ys_rom_load_device_config(device_cfg_t *cfg);
extern void ys_rom_save_device_config(device_cfg_t *cfg);


#endif /* _YS_ROM_H_ */
