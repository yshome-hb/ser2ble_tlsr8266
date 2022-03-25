/********************************************************************************************************
 * @file     app_config.h
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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

/////////////////// MODULE /////////////////////////////////
#define BLE_REMOTE_SECURITY_ENABLE      1
#define BLE_PM_ENABLE					0
#define BLE_OTA_ENABLE                  1

#define BLE_LED_LEVEL                   1  //1 indicate high level turn on LED; 0 indicate low level will turn off LED. based on actual spec.
#define	BLE_LED_PIN		                GPIO_PA5

/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_TYPE  		        CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC
#define CLOCK_SYS_CLOCK_HZ  	        16000000

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		    0
#define WATCHDOG_INIT_TIMEOUT		    500  //ms

/////////////////// rf tx power //////////////////////////////
#define DEFAULT_NORMAL_TX_POWER   	RF_POWER_8dBm

/////////////////// cfg address in flash //////////////////////////////
#define CFG_ADDR_DEVICE   		    0x7A000

/////////////////// product information //////////////////////////////
#define ID_VENDOR				    0x248A
#define ID_PRODUCT				    0x8266
#define ID_VERSION				    0x0100

#define PRODUCT_NAME 			    SER2HID

/////////////////// PRINT DEBUG INFO ///////////////////////
/* 826x module's pin simulate as a uart tx, Just for debugging */
#define PRINT_DEBUG_INFO            0//open/close myprintf

#if PRINT_DEBUG_INFO
#define YS_LOG(fmt, ...)	        do { mini_printf("<%s> "fmt"\r\n", __FUNCTION__, ##__VA_ARGS__); } while(0)
#else
#define YS_LOG(fmt, ...)            do {} while(0)
#endif

#if PRINT_DEBUG_INFO
//defination debug printf pin
#define PRINT_BAUD_RATE             115200 //1M baud rate,should Not bigger than 1M, when system clock is 16M.
#if	(__PROJECT_8261_FEATURE_TEST__ || __PROJECT_8267_FEATURE_TEST__ || __PROJECT_8269_FEATURE_TEST__)
#define DEBUG_INFO_TX_PIN           GPIO_PC6//G0 for 8267/8269 EVK board(C1T80A30_V1.0)
//#define PC6_OUTPUT_ENABLE	        1       //mini_printf function contain this
#define PULL_WAKEUP_SRC_PC6         PM_PIN_PULLUP_1M
#else//__PROJECT_8266_FEATURE_TEST__
#define DEBUG_INFO_TX_PIN           GPIO_PC6//G9 for 8266 EVK board(C1T53A20_V2.0)
//#define PD3_OUTPUT_ENABLE	        1       //mini_printf function contain this
#define PULL_WAKEUP_SRC_PC6         PM_PIN_PULLUP_1M
#endif
#endif


/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
