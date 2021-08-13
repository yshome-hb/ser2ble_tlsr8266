/********************************************************************************************************
 * @file     ble_drv.h
 *
 * @brief    ble drivers header file
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _BLE_DRV_H_
#define _BLE_DRV_H_


extern void ble_drv_init(u8 *addr);
extern void ble_start_advertis(u8 *dev_name);


#endif /* _BLE_DRV_H_ */
