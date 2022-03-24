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
#include "../../proj_lib/ble/service/ble_ll_ota.h"
#include "blt_led.h"
#include "ble_drv.h"

enum{
	LED_ON,	//1
	LED_OFF,	//2
	LED_SHINE_SLOW, //3
	LED_SHINE_FAST, //4
};

const led_cfg_t led_cfg[] = {
	    {100,	  0 ,	  0xff,	  0x00,  },    //long on
	    {0,	      100 ,   0xff,	  0x00,  },    //long off
	    {500,	  500 ,   0x01,	  0x02,	 },    //1Hz blink
	    {250,	  250 ,   0x01,	  0x04,  },    //2Hz blink
};

static void ble_adv_duration_timeout_cb(u8 e, u8 *p, int n)
{
	bls_ll_setAdvParam(ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
					ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
					BLE_ADDR_PUBLIC, NULL,
					BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

	bls_ll_setAdvEnable(1);  //must: set adv enable
}

static void ble_connect_cb(u8 e, u8 *p, int n)
{
	bls_l2cap_requestConnParamUpdate(BLE_CONN_INTERVAL_MIN, BLE_CONN_INTERVAL_MAX, 
									BLE_CONN_LATENCY, BLE_CONN_TIMEOUT);
	bls_l2cap_setMinimalUpdateReqSendingTime_after_connCreate(1000);

	YS_LOG("ble_connect_cb");
	device_led_setup(led_cfg[LED_ON]);
}

static void ble_terminate_cb(u8 e, u8 *p, int n) //*p is terminate reason
{
	YS_LOG("ble_terminate_cb %d", *p);
	device_led_setup(led_cfg[LED_SHINE_SLOW]);
}

_attribute_ram_code_ static void ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * CLOCK_SYS_CLOCK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_CORE);  //gpio CORE wakeup suspend
	}
}

static void ble_ota_start_cb(void)
{
	bls_ota_setTimeout(60*1000000);
}

static void ble_ota_end_cb(int result)
{

}

void ble_drv_init(void)
{
////////////////// Initialize MAC /////////////////////////////
	u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};
	u32 *pmac = (volatile u32 *) CFG_ADR_MAC;
	if (*pmac == U32_MAX)
	{
		generateRandomNum(sizeof(tbl_mac), tbl_mac);
		flash_write_page(CFG_ADR_MAC, sizeof(tbl_mac), tbl_mac);
	}
	else
	{
		flash_read_page(CFG_ADR_MAC, sizeof(tbl_mac), tbl_mac);
	}

////////////////// BLE stack initialization ////////////////////////////////////

	////// Controller Initialization  //////////
	blc_ll_initBasicMCU(tbl_mac);   			//mandatory
	blc_ll_initAdvertising_module(tbl_mac); 	//adv module: 		 mandatory for BLE slave,
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

	rf_set_power_level_index (DEFAULT_NORMAL_TX_POWER);

	//ble event call back
	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &ble_connect_cb);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &ble_terminate_cb);
	bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &ble_adv_duration_timeout_cb);
	//bls_app_registerEventCallback (BLT_EV_FLAG_DATA_LENGTH_EXCHANGE, &ble_exchange_cb);

	///////////////////// Power Management initialization///////////////////
#if(BLE_PM_ENABLE)
	blc_ll_initPowerManagement_module();
	bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif

#if (BLE_OTA_ENABLE)
	////////////////// OTA relative ////////////////////////
	bls_ota_clearNewFwDataArea(); //must
	bls_ota_registerStartCmdCb(ble_ota_start_cb);
	bls_ota_registerResultIndicateCb(ble_ota_end_cb);
#endif

	device_led_init(BLE_LED, 1);
}

void ble_start_advertis(u8 *adv_data, u8 adv_len, u8 *rsp_data, u8 rsp_len)
{
#if (BLE_REMOTE_SECURITY_ENABLE)
	u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
	smp_param_save_t  bondInfo;
	if(bond_number)   //at least 1 bonding device exist
	{
		blc_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )
		bls_ll_setAdvParam(ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
						ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY, OWN_ADDRESS_PUBLIC,
						bondInfo.peer_addr_type,  bondInfo.peer_addr,
						BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

		//it is recommended that direct adv only last for several seconds, then switch to indirect adv
		bls_ll_setAdvDuration(2000000, 1);
	}
	else
#endif
	{
		bls_ll_setAdvParam(ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
					   	ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
					   	BLE_ADDR_PUBLIC, NULL,
					   	BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
	}

	bls_ll_setAdvData(adv_data, adv_len);
	bls_ll_setScanRspData(rsp_data, rsp_len);
	bls_ll_setAdvEnable(1);  //adv enable

	device_led_setup(led_cfg[LED_SHINE_SLOW]);
}

void ble_disconnect()
{
	if(blc_ll_getCurrentState() != BLS_LINK_STATE_CONN)
		return;

	bls_ll_terminateConnection(HCI_ERR_REMOTE_USER_TERM_CONN);
}

bool ble_gap_connected()
{
	return (blc_ll_getCurrentState() == BLS_LINK_STATE_CONN);
}
