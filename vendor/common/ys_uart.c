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

#define UART_BUFF_SIZE      256

__attribute__((aligned(4))) unsigned char uart_rx_buff[UART_BUFF_SIZE] = {0x00,0x00,0x00,0x00,}; // the first four byte is length to receive data.
__attribute__((aligned(4))) unsigned char uart_tx_buff[UART_BUFF_SIZE] = {0x01,0x00,0x00,0x00,UART_MAGIC_BYTE}; // the first four byte is length to send data.

__attribute__ ((weak)) int ys_uart_recv_handler(ysu_data_t *data)
{
	memcpy(uart_tx_buff+4, data->data, data->dma_len);
	ys_uart_send(data->dma_len);
	return 0;
}

void ys_uart_set_baud(UART_BaudTypeDef baud)
{
	switch(baud){
		case UART_BAUD_9600:
			CLK16M_UART9600;
			break;
		case UART_BAUD_19200:
			CLK16M_UART19200;
			break;
		case UART_BAUD_38400:
			CLK16M_UART38400;
			break;
		case UART_BAUD_57600:
			CLK16M_UART57600;
			break;
		case UART_BAUD_115200:
			CLK16M_UART115200;
			break;
		case UART_BAUD_230400:
			CLK16M_UART230400;
			break;	
		default:
			break;
	}
}

void ys_uart_init(UART_BaudTypeDef baud)
{
	reg_dma_rx_rdy0 = FLD_DMA_UART_RX | FLD_DMA_UART_TX;
	ys_uart_set_baud(baud);

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
