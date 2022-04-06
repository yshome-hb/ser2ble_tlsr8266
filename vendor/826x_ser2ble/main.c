/********************************************************************************************************
 * @file     main.c
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

#include "../../proj/tl_common.h"
#include "../../proj/mcu/watchdog_i.h"
#include "../../vendor/common/user_config.h"
#include "../../proj_lib/rf_drv.h"
#include "../../proj_lib/pm.h"
#include "../../proj_lib/ble/ll/ll.h"
#include "blt_led.h"
#include "ys_rom.h"
#include "ys_switch.h"
#include "ser2ble.h"


_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler();
	ys_uart_iqr_handler();
}

int main (void)
{
	blc_pm_select_internal_32k_crystal();

	cpu_wakeup_init(CRYSTAL_TYPE);

	set_tick_per_us(CLOCK_SYS_CLOCK_HZ/1000000);
	clock_init();

	gpio_init();

	///NOTE:This function must be placed before the following function rf_drv_init().
	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value and tp value
	ys_rom_load_device_config(&device_config);

	rf_drv_init(CRYSTAL_TYPE);

	ys_switch_init();

	ser2ble_init();

#if (MODULE_WATCHDOG_ENABLE)
    wd_stop();
    wd_set_interval_ms(WATCHDOG_INIT_TIMEOUT, CLOCK_SYS_CLOCK_1MS);
    wd_start();
#endif

    irq_enable();

	YS_LOG("main start");

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		blt_sdk_main_loop();

		ser2ble_process();

		device_led_process();

		ys_switch_scan();

    #if 0 //PRINT_DEBUG_INFO
		static u32 tick = 0;
		if (clock_time_exceed(tick, 500000))
		{
			tick = clock_time();
			YS_LOG("status=%x,interval=%x,latency=%x,timeout=%x\r\n", blc_ll_getCurrentState(), bls_ll_getConnectionInterval(), bls_ll_getConnectionLatency(), bls_ll_getConnectionTimeout());
		}
    #endif
	}
}

