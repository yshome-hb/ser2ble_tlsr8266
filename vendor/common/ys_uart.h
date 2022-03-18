/********************************************************************************************************
 * @file     ys_uart.h
 *
 * @brief    uart application header file
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _YS_UART_H_
#define _YS_UART_H_


typedef struct{
	unsigned int dma_len;       // dma len must be 4 byte
	unsigned char data[1];
}ysu_data_t;

extern void ys_uart_init();
extern void ys_uart_process(void);
extern int ys_uart_recv_handler(ysu_data_t *data);
extern unsigned char *ys_uart_get_txaddr(void);
extern unsigned char ys_uart_send(unsigned char length);


#endif /* _YS_UART_H_ */
