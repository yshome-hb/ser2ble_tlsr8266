/********************************************************************************************************
 * @file     user_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     May. 12, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *           
 *			 The information contained herein is confidential and proprietary property of Telink 
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms 
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai) 
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in. 
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this 
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided. 
 *           
 *******************************************************************************************************/

#pragma once

#if(__PROJECT_8261_MASTER_KMA_DONGLE__ || __PROJECT_8266_MASTER_KMA_DONGLE__ || __PROJECT_8267_MASTER_KMA_DONGLE__ || __PROJECT_8269_MASTER_KMA_DONGLE__ )
	#include "../826x_master_kma_dongle/app_config.h"
#elif (__PROJECT_8261_MASTER__ || __PROJECT_8266_MASTER__ || __PROJECT_8267_MASTER__ || __PROJECT_8269_MASTER__)
	#include "../826x_master/app_config.h"
#elif (__PROJECT_8261_BTUSB__ || __PROJECT_8266_BTUSB__ || __PROJECT_8267_BTUSB__ || __PROJECT_8269_BTUSB__)
	#include "../826x_btusb/app_config.h"
#elif (__PROJECT_8261_HCI__|__PROJECT_8267_HCI__|__PROJECT_8269_HCI__)
	#include "../826x_hci/app_config.h"
#elif (__PROJECT_8261_MODULE__ || __PROJECT_8267_MODULE__ || __PROJECT_8269_MODULE__ || __PROJECT_8266_MODULE__)
	#include "../826x_module/app_config.h"
#elif (__PROJECT_8261_BLE_REMOTE__ || __PROJECT_8267_BLE_REMOTE__ || __PROJECT_8269_BLE_REMOTE__)
	#include "../826x_ble_remote/app_config.h"
#elif (__PROJECT_8261_FEATURE_TEST__ || __PROJECT_8266_FEATURE_TEST__ || __PROJECT_8267_FEATURE_TEST__ || __PROJECT_8269_FEATURE_TEST__)
	#include "../826x_feature_test/app_config.h"
#elif (__PROJECT_8266_HID_SAMPLE__ || __PROJECT_8267_HID_SAMPLE__)
	#include "../826x_hid_sample/app_config.h"
#elif (__PROJECT_8266_OTA_BOOT__ || __PROJECT_8261_OTA_BOOT__)
	#include "../826x_ota_boot/app_config.h"
#elif (__PROJECT_8261_DRIVER_TEST__||__PROJECT_8266_DRIVER_TEST__||__PROJECT_8267_DRIVER_TEST__||__PROJECT_8269_DRIVER_TEST__)
	#include  "../826x_driver_test/app_config.h"
#else
	#include "../common/default_config.h"
#endif

