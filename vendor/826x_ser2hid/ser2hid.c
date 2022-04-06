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
#include "ser2hid.h"

#define	CAPLOCK_LED_PIN		GPIO_PB6
#define	NUMLOCK_LED_PIN		GPIO_PB5
#define SCRLOCK_LED_PIN		GPIO_PB0

static u32 key_tick = 0;

void ser2hid_init(void)
{
    gpio_set_func(CAPLOCK_LED_PIN, AS_GPIO);
    gpio_set_input_en(CAPLOCK_LED_PIN, 0);
    gpio_set_output_en(CAPLOCK_LED_PIN, 1);

    gpio_set_func(NUMLOCK_LED_PIN, AS_GPIO);
    gpio_set_input_en(NUMLOCK_LED_PIN, 0);
    gpio_set_output_en(NUMLOCK_LED_PIN, 1);

    gpio_set_func(SCRLOCK_LED_PIN, AS_GPIO);
    gpio_set_input_en(SCRLOCK_LED_PIN, 0);
    gpio_set_output_en(SCRLOCK_LED_PIN, 1);

	ble_app_init();
	ys_uart_init(UART_BAUD_115200);

	ys_switch_register(SW_CFG, SW_CFG_PIN, ser2hid_cfg_key_handler);
}

_attribute_ram_code_ void ser2hid_process(void)
{
	ys_uart_process();

	if(key_tick && clock_time_exceed(key_tick, 3000000)){
		ser2hid_cfg_key_handler(1);
		//key_tick = 0;
		ble_disconnect();
		bls_smp_eraseAllParingInformation();
	}

	u8 led_value = ble_keyboard_leds();
	gpio_write(NUMLOCK_LED_PIN, !(led_value & 0x01));
	gpio_write(CAPLOCK_LED_PIN, !(led_value & 0x02));
	gpio_write(SCRLOCK_LED_PIN, !(led_value & 0x04));	
}

_attribute_ram_code_ int ys_uart_recv_handler(unsigned char *data, unsigned char len)
{
	static const u8 hid_release[16] = {0};
    if((len < 2) || (data[0] != UART_MAGIC_BYTE))
        return 0;

	if(data[1] != (len - 2))
		return 0;

	// if(!ys_protocol_checksum(data+2, data[1]))
	// 	return 0;

	if(data[1] == 1)
	{
		ble_send_system(data[2]);
	}
	else if(data[1] == 2)
	{
		ble_send_consumer((((u16)data[3])<<8) | data[2]);
	}
	else if(data[1] == 8)
	{
		ble_send_keyboard(data);
	}
	else if(data[1] == 15)
	{
		ble_send_nkro(data);
	}
	else if(data[1] == 4)
	{
		ble_send_mouse(data);
	}
	else if(data[1] == 0)
	{
		ble_send_keyboard(hid_release);
		ble_send_mouse(hid_release);
		ble_send_nkro(hid_release);
		ble_send_consumer(0);
		ble_send_system(0);
	}

	ys_uart_send(data, len);

	return 0;
}

int ser2hid_cfg_key_handler(u8 val)
{
	u8 kb_data[8] = {0};
	if(val == 0){
		key_tick = (clock_time() | 1);
		kb_data[2] = 0x04;
		ble_send_keyboard(kb_data);
	}else if(key_tick){
		key_tick = 0;
		ble_send_keyboard(kb_data);
	}

	return 0;
}

