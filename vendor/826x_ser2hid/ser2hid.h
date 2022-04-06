/********************************************************************************************************
 * @file     ser2hid.h
 *
 * @brief    serial to hid header file
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _SER_TO_HID_H_
#define _SER_TO_HID_H_

#include "ys_config.h"


extern void ser2hid_init(void);
extern void ser2hid_process(void);
extern int ser2hid_cfg_key_handler(u8 val);


#endif /* _SER_TO_HID_H_ */
