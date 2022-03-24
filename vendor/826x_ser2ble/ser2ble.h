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


extern void ble_app_init();
extern int ble_nus_send_data(u8 *data, u8 len);
extern int ble_nus_recv_handler(u8 *data, u8 len);
extern int ble_nus_cmd_handler(u8 *data, u8 len);
extern int ble_nus_gpio_handler(u8 *data, u8 len);


#endif /* _SER_TO_BLE_H_ */
