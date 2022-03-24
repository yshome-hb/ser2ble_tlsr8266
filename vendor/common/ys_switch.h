/********************************************************************************************************
 * @file     ys_switch.h
 *
 * @brief    switch functions header file
 *
 * @author	 ysshen
 * @date     2021.05
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _YS_SWITCH_H_
#define _YS_SWITCH_H_

#include "../../proj/common/types.h"

typedef enum
{
	SW_CFG,
	SW_MAX,

}SWITCH_IDX_ENUM;

typedef struct {
	u32 pin;
	int (*action_cb)(u8);
	u8 pre_value;
	u8 value;
	u16 filter;
}switch_sts_t;

extern switch_sts_t sw_tbl[SW_MAX];

extern void ys_switch_init();
extern int ys_switch_register(u8 idx, u32 pin, int (*action_cb)(u8));
extern void ys_switch_scan();

static inline u8 ys_switch_value(u8 idx)
{
	return sw_tbl[idx].value;
}

#endif /* _YS_SWITCH_H_ */
