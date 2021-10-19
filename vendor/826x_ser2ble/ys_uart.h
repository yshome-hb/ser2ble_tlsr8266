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


extern void ys_uart_irq_handler(void);
extern void ys_uart_init();
extern void ys_uart_process(void);


#endif /* _YS_UART_H_ */
