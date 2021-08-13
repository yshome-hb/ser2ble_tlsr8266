/********************************************************************************************************
 * @file     ble_drv.h
 *
 * @brief    ble drivers functions
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#include "../../proj/tl_common.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble/ll/ll.h"
#include "../../proj_lib/ble/ble_smp.h"
#include "ble_drv.h"


MYFIFO_INIT(blt_rxfifo, 64, 8);
MYFIFO_INIT(blt_txfifo, 40, 16);

#define ADV_DEVICE_NAME_POS   		11
#define ADV_DEVICE_NAME_MAX_LEN  	18

static u8 adv_data_raw[31] = {
	0x02, 0x01, 0x05, 			// BLE limited discoverable mode and BR/EDR not supported
	0x03, 0x19, 0x80, 0x01, 	// 384, Generic Remote Control, Generic category
	0x03, 0x03, 0x12, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
	19, 0x09, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };


static void ble_adv_duration_timeout_cb(u8 e, u8 *p, int n)
{
	// bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
	// 					ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
	// 					0,  NULL,
	// 					MY_APP_ADV_CHANNEL,
	// 					ADV_FP_NONE);

	// bls_ll_setAdvEnable(1);  //must: set adv enable
}

static void ble_connect_cb(u8 e, u8 *p, int n)
{
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  //interval=10ms latency=99 timeout=4s
	bls_l2cap_setMinimalUpdateReqSendingTime_after_connCreate(1000);
}

static void ble_terminate_cb(u8 e, u8 *p, int n) //*p is terminate reason
{
	//MW_LOG("ble_terminate_cb %d\r\n", *p);

}

_attribute_ram_code_ static void ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * CLOCK_SYS_CLOCK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_CORE);  //gpio CORE wakeup suspend
	}
}

void ble_drv_init(u8 *addr)
{
////////////////// BLE stack initialization ////////////////////////////////////

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU(addr);   			//mandatory
	blc_ll_initAdvertising_module(addr); 	//adv module: 		 mandatory for BLE slave,
	blc_ll_initSlaveRole_module();			//slave module: 	 mandatory for BLE slave,

	////// Host Initialization  //////////
	blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization

 	//// smp initialization ////
#if (BLE_REMOTE_SECURITY_ENABLE)
	blc_smp_param_setBondingDeviceMaxNumber(4);  	//default is SMP_BONDING_DEVICE_MAX_NUM, can not bigger that this value
														//and this func must call before bls_smp_enableParing
	bls_smp_enableParing (SMP_PARING_CONN_TRRIGER );
#else
	bls_smp_enableParing (SMP_PARING_DISABLE_TRRIGER );
#endif

	rf_set_power_level_index (RF_POWER_8dBm);

	//ble event call back
	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &ble_connect_cb);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &ble_terminate_cb);
	bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &ble_adv_duration_timeout_cb);

	///////////////////// Power Management initialization///////////////////
#if(BLE_PM_ENABLE)
	blc_ll_initPowerManagement_module();
	bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif

}

void ble_start_advertis(u8 *dev_name)
{
	u32 name_len = strlen((char *)dev_name);

	if(name_len > ADV_DEVICE_NAME_MAX_LEN)
		name_len = ADV_DEVICE_NAME_MAX_LEN;

	memcpy(adv_data_raw + ADV_DEVICE_NAME_POS + 2, dev_name, name_len);
	adv_data_raw[ADV_DEVICE_NAME_POS] = name_len + 1;

	bls_ll_setAdvParam(ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
					   ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
					   BLE_ADDR_PUBLIC, NULL,
					   BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

	bls_ll_setAdvData( (u8 *)adv_data_raw, name_len + ADV_DEVICE_NAME_POS + 2);
	bls_ll_setScanRspData(adv_data_raw + ADV_DEVICE_NAME_POS, name_len + 2);
	bls_ll_setAdvEnable(1);  //adv enable
}
