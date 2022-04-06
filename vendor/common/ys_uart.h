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

#define UART_MAGIC_BYTE		0xA5

typedef enum {
	UART_BAUD_9600 = 0,
	UART_BAUD_19200,
	UART_BAUD_38400,
	UART_BAUD_57600,
	UART_BAUD_115200,
	UART_BAUD_230400,
}UART_BaudTypeDef;

extern void ys_uart_iqr_handler(void);
extern int ys_uart_recv_handler(unsigned char *data, unsigned char len);
extern void ys_uart_set_baud(UART_BaudTypeDef baud);
extern void ys_uart_init(UART_BaudTypeDef baud);
extern void ys_uart_process(void);
extern int ys_uart_send(unsigned char *data, unsigned char len);


#endif /* _YS_UART_H_ */
