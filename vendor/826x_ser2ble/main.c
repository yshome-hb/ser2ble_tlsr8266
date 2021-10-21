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
#include "ble_app.h"
#include "ys_uart.h"


_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler();

	ys_uart_irq_handler();
}

int main (void) {

	blc_pm_select_internal_32k_crystal();

	cpu_wakeup_init(CRYSTAL_TYPE);

	set_tick_per_us (CLOCK_SYS_CLOCK_HZ/1000000);
	clock_init();

	gpio_init();

	///NOTE:This function must be placed before the following function rf_drv_init().
	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value and tp value

	rf_drv_init(CRYSTAL_TYPE);

	ble_app_init ();

	//ys_uart_init();

	device_led_init(GPIO_PD5, 1);

    irq_enable();

	mini_printf("blt_sdk_main_loop\r\n");
	mini_printf("blt_sdk_main_loop\r\n");
	mini_printf("blt_sdk_main_loop\r\n");

	while (1) {
#if (MODULE_WATCHDOG_ENABLE)
		wd_clear(); //clear watch dog
#endif
		blt_sdk_main_loop();

		//ys_uart_process();

	}
}

