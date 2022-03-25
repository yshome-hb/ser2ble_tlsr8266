/********************************************************************************************************
 * @file     ys_switch.c
 *
 * @brief    switch functions
 *
 * @author	 ysshen
 * @date     2021.05
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/
#include "../../proj/tl_common.h"
#include "ys_switch.h"

switch_sts_t sw_tbl[SW_MAX];

void ys_switch_init()
{
	memset(sw_tbl, 0, sizeof(sw_tbl));
}

int ys_switch_register(u8 idx, u32 pin, int (*action_cb)(u8))
{
	if(idx >= SW_MAX)
		return 0;

	gpio_set_func(pin, AS_GPIO);
	gpio_set_output_en(pin, 0);
	gpio_set_input_en(pin, 1);
    gpio_setup_up_down_resistor(pin, PM_PIN_PULLUP_10K);

	sleep_us(50);
	sw_tbl[idx].pin = pin;
	sw_tbl[idx].action_cb = action_cb;
	sw_tbl[idx].value = (gpio_read(sw_tbl[idx].pin) != 0);
	sw_tbl[idx].pre_value = sw_tbl[idx].value;
	sw_tbl[idx].filter = sw_tbl[idx].value ? 0xFFFF: 0;

	return 1;
}

void ys_switch_scan(void)
{
	for(u8 i = 0; i < SW_MAX; i++)
	{
		if(sw_tbl[i].pin == 0)
			continue;

		sw_tbl[i].pre_value = sw_tbl[i].value;
		sw_tbl[i].filter <<= 1;
		sw_tbl[i].filter |= (gpio_read(sw_tbl[i].pin) > 0);
		sw_tbl[i].filter &= 0xFF;
		if(sw_tbl[i].filter == 0xFF)
			sw_tbl[i].value = 0x01;
		else if(sw_tbl[i].filter == 0x00)
			sw_tbl[i].value = 0x00;

		if(sw_tbl[i].action_cb && (sw_tbl[i].pre_value ^ sw_tbl[i].value))
		{
			sw_tbl[i].action_cb(sw_tbl[i].value);
		}
	}
}

