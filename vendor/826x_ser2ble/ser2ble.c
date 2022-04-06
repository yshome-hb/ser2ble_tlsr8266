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
#include "../../proj_lib/ble/ll/ll.h"
#include "ys_uart.h"
#include "ys_rom.h"
#include "ys_switch.h"
#include "ble_drv.h"
#include "ble_app.h"
#include "ser2ble.h"

#define	BAUD1_LED_PIN		GPIO_PB6
#define	BAUD2_LED_PIN		GPIO_PB5
#define	BAUD3_LED_PIN		GPIO_PB0

#define	SER_GPIO0_PIN		GPIO_PA0
#if PRINT_DEBUG_INFO
#define	SER_GPIO1_PIN		GPIO_PF0
#else
#define	SER_GPIO1_PIN		GPIO_PA1
#endif

static u32 key_tick = 0;

int ser2ble_at_process(u8 *cmd, u8 len);

static void ser2ble_show_baudrate(UART_BaudTypeDef baud)
{
	baud++;
	gpio_write(BAUD1_LED_PIN, !(baud & 0x01));
	gpio_write(BAUD2_LED_PIN, !(baud & 0x02));
	gpio_write(BAUD3_LED_PIN, !(baud & 0x04));
}

void ser2ble_init(void)
{
    gpio_set_func(BAUD1_LED_PIN, AS_GPIO);
    gpio_set_input_en(BAUD1_LED_PIN, 0);
    gpio_set_output_en(BAUD1_LED_PIN, 1);

    gpio_set_func(BAUD2_LED_PIN, AS_GPIO);
    gpio_set_input_en(BAUD2_LED_PIN, 0);
    gpio_set_output_en(BAUD2_LED_PIN, 1);

    gpio_set_func(BAUD3_LED_PIN, AS_GPIO);
    gpio_set_input_en(BAUD3_LED_PIN, 0);
    gpio_set_output_en(BAUD3_LED_PIN, 1);

    gpio_set_func(SER_GPIO0_PIN, AS_GPIO);
    gpio_set_input_en(SER_GPIO0_PIN, 0);
    gpio_set_output_en(SER_GPIO0_PIN, 1);

    gpio_set_func(SER_GPIO1_PIN, AS_GPIO);
    gpio_set_input_en(SER_GPIO1_PIN, 0);
    gpio_set_output_en(SER_GPIO1_PIN, 1);

	ble_app_init();
	ys_uart_init(device_config.baudrate);
	ser2ble_show_baudrate(device_config.baudrate);

	ys_switch_register(SW_CFG, SW_CFG_PIN, ser2ble_cfg_key_handler);
}

_attribute_ram_code_ void ser2ble_process(void)
{
	ys_uart_process();

	if(key_tick && clock_time_exceed(key_tick, 3000000)){
		key_tick = 0;
		ble_disconnect();
	}
}

_attribute_ram_code_ int ys_uart_recv_handler(unsigned char *data, unsigned char len)
{
	int ret = 0;
	if(ble_gap_connected()){
		ret = ble_nus_send_data(data, len);
		bls_pm_setManualLatency(0);
	}else{
		ser2ble_at_process(data, len);
	}

	return ret;
}

_attribute_ram_code_ int ble_nus_recv_handler(u8 *data, u8 len)
{
	ys_uart_send(data, len);
	return 0;
}

int ble_nus_cmd_handler(u8 *data, u8 len)
{
	if((len == 2) && (data[0] == 0x0B)){
		if(data[1] <= UART_BAUD_230400){
			ys_uart_set_baud(data[1]);
			if(device_config.baudrate != data[1]){
				device_config.baudrate = data[1];
				ys_rom_save_device_config(&device_config);
			}
			ser2ble_show_baudrate(device_config.baudrate);
		}
	}

	return 0;
}

int ble_nus_gpio_handler(u8 data)
{
	gpio_write(SER_GPIO0_PIN, data & 0x01);
	gpio_write(SER_GPIO1_PIN, data & 0x02);
	return 0;
}

int ser2ble_cfg_key_handler(u8 val)
{
	if(val == 0){
		key_tick = (clock_time() | 1);
	}else if(key_tick){
		key_tick = 0;
		device_config.baudrate++;
		if(device_config.baudrate > UART_BAUD_230400)
			device_config.baudrate = UART_BAUD_9600;
		ys_uart_set_baud(device_config.baudrate);
		ser2ble_show_baudrate(device_config.baudrate);
		ys_rom_save_device_config(&device_config);
	}

	return 0;
}

int ser2ble_at_process(u8 *cmd, u8 len)
{
	u8 sertx_buf[32] = {0};
	u8 sertx_idx = 0;

	if(len < 4)
		return 0;

	if((cmd[0] != 'A') || (cmd[1] != 'T') 
	 ||(cmd[len-2] != '\r') || (cmd[len-1] != '\n'))
		return 0;

	len -= 4;

	if(len == 0){
		sertx_buf[sertx_idx++] = 'O';
		sertx_buf[sertx_idx++] = 'K';
	}

	if(sertx_idx > 0){
		sertx_buf[sertx_idx++] = '\r';
		sertx_buf[sertx_idx++] = '\n';
		ys_uart_send(sertx_buf, sertx_idx);
	}

	return sertx_idx;
}
