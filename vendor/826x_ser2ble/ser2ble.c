/********************************************************************************************************
 * @file     ser2ble.h
 *
 * @brief    serial to ble application functions
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/
#include "../../proj/tl_common.h"
#include "../../proj/common/string.h"
#include "../../proj_lib/ble/ll/ll.h"
#include "ys_uart.h"
#include "ys_rom.h"
#include "ble_drv.h"
#include "ble_app.h"
#include "ser2ble.h"


#define UART_FIFO_SIZE	22
#define UART_FIFO_NUM	16

MYFIFO_INIT(uart_rxfifo, UART_FIFO_SIZE, UART_FIFO_NUM);
MYFIFO_INIT(uart_txfifo, UART_FIFO_SIZE, UART_FIFO_NUM);


static u32 key_tick = 0;
static u8 *sertx_ptr = NULL;

int ser2ble_at_process(u8 *cmd, u8 len);

void ser2ble_init(void)
{
	ble_app_init();
	ys_uart_init(device_config.baudrate);
	sertx_ptr = ys_uart_get_txaddr();
}

_attribute_ram_code_ void ser2ble_process(void)
{
	ys_uart_process();

	u8 *fp = my_fifo_get(&uart_rxfifo);
	if(fp != NULL){
		if(ble_gap_connected()){
			if(ble_nus_send_data(fp+2, fp[0]) == 0)
				my_fifo_pop(&uart_rxfifo);
			bls_pm_setManualLatency(0);
		}else if(fp[0] >= 4 && fp[2] == 'A' && fp[3] == 'T'){
			ser2ble_at_process(fp+4, fp[0]-2);
		}
	}

	if(key_tick && clock_time_exceed(key_tick, 3000000)){
		key_tick = 0;
		YS_LOG("long pressed");
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

int ble_nus_cmd_handler(u8 *data, u8 len)
{
	if((len == 2) && (data[0] == 0x0B)){
		if(data[1] <= UART_BAUD_230400){
			ys_uart_set_baud(data[1]);
			device_config.baudrate = data[1];
			ys_rom_save_device_config(&device_config);
		}
	}

	return 0;
}

int ble_nus_gpio_handler(u8 data)
{

	return 0;
}

int ser2ble_cfg_key_handler(u8 val)
{
	YS_LOG("key val %d", val);
	if(val == 0){
		key_tick = (clock_time() | 1);
	}else if(key_tick){
		key_tick = 0;
		device_config.baudrate++;
		if(device_config.baudrate > UART_BAUD_230400)
			device_config.baudrate = UART_BAUD_9600;
		ys_uart_set_baud(device_config.baudrate);
		ys_rom_save_device_config(&device_config);
		YS_LOG("short pressed");
	}

	return 0;
}

int ser2ble_at_process(u8 *cmd, u8 len)
{
	int sertx_idx = 0;

	if(memcmp(cmd, "\r\n", 2) == 0){
		sertx_ptr[sertx_idx++] = 'O';
		sertx_ptr[sertx_idx++] = 'K';
	}

	if(sertx_idx > 0){
		sertx_ptr[sertx_idx++] = '\r';
		sertx_ptr[sertx_idx++] = '\n';
		ys_uart_send(sertx_idx);
	}

	return sertx_idx;
}
