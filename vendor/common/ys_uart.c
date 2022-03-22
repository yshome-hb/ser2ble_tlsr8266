/********************************************************************************************************
 * @file     ys_uart.h
 *
 * @brief    uart application functions
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#include "../../proj/tl_common.h"
#include "../../proj/drivers/uart.h"
#include "ys_uart.h"

#define UART_BUFF_SIZE      64

__attribute__((aligned(4))) unsigned char uart_rx_buff[UART_BUFF_SIZE] = {0x00,0x00,0x00,0x00,}; // the first four byte is length to receive data.
__attribute__((aligned(4))) unsigned char uart_tx_buff[UART_BUFF_SIZE] = {0x01,0x00,0x00,0x00,UART_MAGIC_BYTE}; // the first four byte is length to send data.

__attribute__ ((weak)) int ys_uart_recv_handler(ysu_data_t *data)
{
	memcpy(uart_tx_buff+4, data->data, data->dma_len);
	ys_uart_send(data->dma_len);
	return 0;
}

void ys_uart_init(void)
{
	reg_dma_rx_rdy0 = FLD_DMA_UART_RX | FLD_DMA_UART_TX;
	CLK16M_UART115200;

	uart_RecBuffInit(uart_rx_buff, sizeof(uart_rx_buff));  //set uart rev buffer and buffer size
	uart_txBuffInit(UART_BUFF_SIZE);

	UART_GPIO_CFG_PC6_PC7();

	uart_set_tx_done_delay(2000);
}

_attribute_ram_code_ void ys_uart_process(void)
{
	unsigned char irq_src = reg_dma_rx_rdy0; // get the irq source and clear the irq.	
	if(irq_src & FLD_DMA_UART_RX){
		reg_dma_rx_rdy0 = FLD_DMA_UART_RX;
		ys_uart_recv_handler((ysu_data_t *)uart_rx_buff);
	}
}

unsigned char *ys_uart_get_txaddr(void)
{
	return uart_tx_buff+4;
}

_attribute_ram_code_ unsigned char ys_uart_send(unsigned char length)
{
	ysu_data_t *tx_data = (ysu_data_t *)uart_tx_buff;
	tx_data->dma_len = length;
	return uart_Send_kma(uart_tx_buff);
}
