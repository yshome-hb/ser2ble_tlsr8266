/********************************************************************************************************
 * @file     ser2ble.h
 *
 * @brief    serial to ble header file
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _SER_TO_BLE_H_
#define _SER_TO_BLE_H_

#include "ys_config.h"


extern void ser2ble_init(void);
extern void ser2ble_process(void);
extern int ser2ble_cfg_key_handler(u8 val);


#endif /* _SER_TO_BLE_H_ */
