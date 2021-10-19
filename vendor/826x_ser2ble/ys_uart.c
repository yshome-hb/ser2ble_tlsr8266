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

#define UART_RX_BUFF_SIZE      16
#define UART_TX_BUFF_SIZE      16

unsigned char uart_rx_irq = 0;

__attribute__((aligned(4))) unsigned char uart_rec_buff[UART_RX_BUFF_SIZE] = {0x00,0x00,0x00,0x00,}; // the first four byte is length to receive data.
__attribute__((aligned(4))) unsigned char uart_tx_buff[UART_TX_BUFF_SIZE]  = {0x0b,0x00,0x00,0x00,'t','e','l','i','n','k','-','s','e','m','i'}; // the first four byte is length to send data.


_attribute_ram_code_ void ys_uart_irq_handler(void)
{
	unsigned char irqS = uart_IRQSourceGet(); // get the irq source and clear the irq.
	if(irqS & UARTRXIRQ){
		uart_rx_irq = 1;
	}
}

void ys_uart_init(void)
{
	CLK16M_UART115200;

	uart_RecBuffInit(uart_rec_buff, UART_RX_BUFF_SIZE);  //set uart rev buffer and buffer size
	uart_txBuffInit(UART_TX_BUFF_SIZE);

	UART_GPIO_CFG_PC6_PC7();
}

void ys_uart_process(void)
{
	if(uart_rx_irq){
		uart_rx_irq = 0;
		/*receive buffer,the first four bytes is the length information of received data.send the received data*/
		while(!uart_Send(uart_rec_buff));
		/*transmit buffer, the first four bytes is the length information of transmitting data.the DMA module will send the data based on the length.
		* so the useful data start from the fifth byte and start to send to other device from the fifth byte.*/
		while(!uart_Send(uart_tx_buff));
	}
}

