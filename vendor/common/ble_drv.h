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

#define BLE_CONN_INTERVAL_MIN			6
#define BLE_CONN_INTERVAL_MAX			9
#define BLE_CONN_LATENCY				60
#define BLE_CONN_TIMEOUT				300

typedef struct
{
    /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMin;
    /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMax;
    /** Number of LL latency connection events (0x0000 - 0x03e8) */
    u16 latency;
    /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
    u16 timeout;

} gap_periConnectParams_t;

extern void ble_drv_init(void);
extern void ble_start_advertis(u8 *adv_data, u8 adv_len, u8 *rsp_data, u8 rsp_len);


#endif /* _BLE_DRV_H_ */
