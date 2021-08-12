/********************************************************************************************************
 * @file     adc.h
 *
 * @brief    This is the ADC driver header file for TLSR8232
 *
 * @author   junwei.lu
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#if(__TL_LIB_8266__ || MCU_CORE_TYPE == MCU_CORE_8266)
#include "../mcu_spec/adc_8266.h"
#elif(__TL_LIB_8267__ || MCU_CORE_TYPE == MCU_CORE_8267 || \
	  __TL_LIB_8261__ || MCU_CORE_TYPE == MCU_CORE_8261 || \
	  __TL_LIB_8269__ || MCU_CORE_TYPE == MCU_CORE_8269 )
#include "../mcu_spec/adc_8267.h"
#else
#endif



#endif /* ADC_H_ */
