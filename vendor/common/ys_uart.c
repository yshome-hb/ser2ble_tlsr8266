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

MYFIFO_INIT(uart_rx_fifo, 22, 64);
MYFIFO_INIT(uart_tx_fifo, 24, 8);

static unsigned char *wptr = NULL;
static unsigned char *wlen = NULL;
static u32 uart_rx_tick = 0;
static u32 uart_rx_idle = 0;

_attribute_ram_code_ void ys_uart_iqr_handler(void)
{
    if( GET_UART_NOT_DMA_IRQ() ){
		*wptr = uart_notDmaModeRevData();
		(*wlen)++;
		if((*wlen) < uart_rx_fifo.size){
			wptr++;
			uart_rx_tick = clock_time() | 1;
		}else{
			uart_rx_tick = 0;
			if(((uart_rx_fifo.wptr - uart_rx_fifo.rptr) & 0xFF) < uart_rx_fifo.num){
				uart_rx_fifo.wptr++;
				wlen = uart_rx_fifo.p + (uart_rx_fifo.wptr & (uart_rx_fifo.num-1)) * uart_rx_fifo.size;
				wptr = wlen+2;
				*wlen = 2;
			}
		}
	}

	unsigned char irq_src = reg_dma_rx_rdy0;
    if(irq_src & FLD_DMA_UART_TX) {
    	reg_dma_rx_rdy0 = FLD_DMA_UART_TX;
 		uart_clr_tx_busy_flag();
    }
}

__attribute__ ((weak)) int ys_uart_recv_handler(unsigned char *data, unsigned char len)
{
	ys_uart_send(data, len);
	return 0;
}

void ys_uart_set_baud(UART_BaudTypeDef baud)
{
	switch(baud){
		case UART_BAUD_9600:
			CLK16M_UART9600;
			uart_rx_idle = 1000000/96;
			break;
		case UART_BAUD_19200:
			CLK16M_UART19200;
			uart_rx_idle = 1000000/192;
			break;
		case UART_BAUD_38400:
			CLK16M_UART38400;
			uart_rx_idle = 1000000/384;
			break;
		case UART_BAUD_57600:
			CLK16M_UART57600;
			uart_rx_idle = 1000000/576;
			break;
		case UART_BAUD_115200:
			CLK16M_UART115200;
			uart_rx_idle = 1000000/1152;
			break;
		case UART_BAUD_230400:
			CLK16M_UART230400;
			uart_rx_idle = 1000000/2304;
			break;	
		default:
			break;
	}
	uart_notDmaModeInit(1, 0, 1, 0);
}


void ys_uart_init(UART_BaudTypeDef baud)
{
	wptr = uart_rx_fifo_b+2;
	wlen = uart_rx_fifo_b;
	*wlen = 2;

	reg_dma_rx_rdy0 = FLD_DMA_UART_TX;
	ys_uart_set_baud(baud);

	UART_GPIO_CFG_PC6_PC7();
	uart_txBuffInit(256);
	uart_set_tx_done_delay(uart_rx_idle << 1);
}

_attribute_ram_code_ void ys_uart_process(void)
{
	uart_ErrorCLR();

	u8 r = irq_disable();
	if(uart_rx_tick && clock_time_exceed(uart_rx_tick, uart_rx_idle)){
		uart_rx_tick = 0;
		if(((uart_rx_fifo.wptr - uart_rx_fifo.rptr) & 0xFF) < uart_rx_fifo.num){
			uart_rx_fifo.wptr++;
			wlen = uart_rx_fifo.p + (uart_rx_fifo.wptr & (uart_rx_fifo.num-1)) * uart_rx_fifo.size;
			wptr = wlen+2;
			*wlen = 2;
		}
	}
	irq_restore(r);

	u8* fp = my_fifo_get(&uart_rx_fifo);
	if (fp != NULL){
		if(ys_uart_recv_handler(fp+2, fp[0]-2) == 0)
			my_fifo_pop(&uart_rx_fifo);
	}

	fp = my_fifo_get (&uart_tx_fifo);
	if (fp && !uart_tx_is_busy()){
		uart_Send_kma(fp);
		my_fifo_pop(&uart_tx_fifo);
	}
}

_attribute_ram_code_ int ys_uart_send(unsigned char *data, unsigned char len)
{
	if(((uart_tx_fifo.wptr - uart_tx_fifo.rptr) & 0xFF) >= uart_tx_fifo.num){
		return -1;
	}

	if(len > (uart_tx_fifo.size - 4)){
		len = uart_tx_fifo.size - 4;
	}

	u8 *pd = uart_tx_fifo.p + (uart_tx_fifo.wptr++ & (uart_tx_fifo.num-1)) * uart_tx_fifo.size;
	*pd++ = len;
	*pd++ = 0;
	*pd++ = 0;
	*pd++ = 0;
	memcpy(pd, data, len);
	return 0;
}
