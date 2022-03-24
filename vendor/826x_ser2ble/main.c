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
#include "ble_app.h"
#include "ys_uart.h"

#define UART_FIFO_SIZE	22
#define UART_FIFO_NUM	16

MYFIFO_INIT(uart_rxfifo, UART_FIFO_SIZE, UART_FIFO_NUM);
MYFIFO_INIT(uart_txfifo, UART_FIFO_SIZE, UART_FIFO_NUM);

_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler();
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

	rf_drv_init(CRYSTAL_TYPE);

	ble_app_init();

	ys_uart_init();

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

		ys_uart_process();

		u8 *fp = my_fifo_get(&uart_rxfifo);
		if(fp != NULL)
		{
			if(ble_nus_send_data(fp+2, fp[0]) == 0)
				my_fifo_pop(&uart_rxfifo);
			bls_pm_setManualLatency(0);
		}

		device_led_process();

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

_attribute_ram_code_ int ys_uart_recv_handler(ysu_data_t *rx_data)
{
	u8 *recv_ptr = rx_data->data;
	u8 recv_len = 0;

	while(rx_data->dma_len){
		recv_len = min(20, rx_data->dma_len);
		my_fifo_push(&uart_rxfifo, recv_ptr, recv_len);
		rx_data->dma_len -= recv_len;
		recv_ptr += recv_len;
	}
	
	return 0;
}

_attribute_ram_code_ int ble_nus_recv_handler(u8 *data, u8 len)
{
	u8 *uart_data = ys_uart_get_txaddr();
	memcpy(uart_data, data, len);
	ys_uart_send(len);
	return 0;
}
