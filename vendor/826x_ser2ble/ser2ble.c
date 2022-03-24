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
#include "ys_uart.h"
#include "ble_app.h"
#include "ser2ble.h"


#define UART_FIFO_SIZE	22
#define UART_FIFO_NUM	16

MYFIFO_INIT(uart_rxfifo, UART_FIFO_SIZE, UART_FIFO_NUM);
MYFIFO_INIT(uart_txfifo, UART_FIFO_SIZE, UART_FIFO_NUM);


void ser2ble_init(void)
{
	ble_app_init();
	ys_uart_init();
}

_attribute_ram_code_ void ser2ble_process(void)
{
	ys_uart_process();

	u8 *fp = my_fifo_get(&uart_rxfifo);
	if(fp != NULL)
	{
		if(ble_nus_send_data(fp+2, fp[0]) == 0)
			my_fifo_pop(&uart_rxfifo);
		bls_pm_setManualLatency(0);
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
		ys_uart_set_baud(data[1]);
	}

	return 0;
}
