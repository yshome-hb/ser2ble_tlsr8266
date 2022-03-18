/********************************************************************************************************
 * @file     ble_app.h
 *
 * @brief    ble app header file
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#ifndef _BLE_APP_H_
#define _BLE_APP_H_

#include "ys_config.h"

#define REPORT_ID_KEYBOARD_INPUT_BLE       	1   //!< Keyboard input report ID
#define REPORT_ID_CONSUMER_INPUT_BLE		2   //!< Consumer Control input report ID
#define REPORT_ID_SYSTEM_INPUT_BLE			3   //!< System input report ID
#define REPORT_ID_NKRO_INPUT_BLE        	4   //!< Nkro input report ID
#define REPORT_ID_APPLE_FN_INPUT_BLE        5   //!< Apple fn input report ID
#define REPORT_ID_MOUSE_INPUT_BLE       	6   //!< Mouse input report ID

///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
typedef enum
{
	ATT_H_START = 0,

/*********Gap*************************************************************************************/
	GenericAccess_PS_H, 					//UUID: 2800, 	VALUE: uuid 1800
	GenericAccess_DeviceName_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	GenericAccess_DeviceName_DP_H,			//UUID: 2A00,   VALUE: device name
	GenericAccess_Appearance_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	GenericAccess_Appearance_DP_H,			//UUID: 2A01,	VALUE: appearance
	CONN_PARAM_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read
	CONN_PARAM_DP_H,						//UUID: 2A04,   VALUE: connParameter

/*********gatt*************************************************************************************/
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC

/********device information**************************************************************************************/
	DeviceInformation_PS_H,					//UUID: 2800, 	VALUE: uuid 180A
	DeviceInformation_pnpID_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_pnpID_DP_H,			//UUID: 2A50,	VALUE: PnPtrs

/*******battery service***************************************************************************************/
	BATT_PS_H, 								//UUID: 2800, 	VALUE: uuid 180f
	BATT_LEVEL_INPUT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	BATT_LEVEL_INPUT_DP_H,					//UUID: 2A19 	VALUE: batVal
	BATT_LEVEL_INPUT_CCB_H,					//UUID: 2902, 	VALUE: batValCCC

#if (BLE_OTA_ENABLE)
/**********OTA***********************************************************************************/
	OTA_PS_H,								//UUID: 2800,	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803,	VALUE:				Prop: read | write_without_rsp	
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_OUT_DESC_H,						//UUID: 2901,	VALUE: otaName
#endif

/********HID**************************************************************************************/
	HID_PS_H, 								//UUID: 2800, 	VALUE: uuid 1812

	//include
	HID_INCLUDE_H,							//UUID: 2802, 	VALUE: include

	//protocol
	HID_PROTOCOL_MODE_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp	
	HID_PROTOCOL_MODE_DP_H,					//UUID: 2A4E,	VALUE: protocolMode
	
	//normal keyboard input report
	HID_KEYBOARD_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_KEYBOARD_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: bootKeyInReport
	HID_KEYBOARD_REPORT_INPUT_CCB_H,		//UUID: 2902, 	VALUE: bootKeyInReportCCC
	HID_KEYBOARD_REPORT_INPUT_REF_H,		//UUID: 2908    VALUE: REPORT_ID_KEYBOARD, TYPE_INPUT

	//normal keyboard output report
	HID_KEYBOARD_REPORT_OUTPUT_CD_H,		//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
	HID_KEYBOARD_REPORT_OUTPUT_DP_H,		//UUID: 2A4D, 	VALUE: bootKeyOutReport
	HID_KEYBOARD_REPORT_OUTPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_BOOTKEY, TYPE_OUTPUT

	//consumer report in
	HID_CONSUMER_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_CONSUMER_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: reportConsumerIn
	HID_CONSUMER_REPORT_INPUT_CCB_H,		//UUID: 2902, 	VALUE: reportConsumerInCCC
	HID_CONSUMER_REPORT_INPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_CONSUMER, TYPE_INPUT

	//system report in
	HID_SYSTEM_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_SYSTEM_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: reportSystemIn
	HID_SYSTEM_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: reportSystemInInCCC
	HID_SYSTEM_REPORT_INPUT_REF_H, 			//UUID: 2908    VALUE: REPORT_ID_SYSTEM, TYPE_INPUT

	//nkro keyboard report in
	HID_NKRO_REPORT_INPUT_CD_H,				//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_NKRO_REPORT_INPUT_DP_H,				//UUID: 2A4D, 	VALUE: reportKeyIn
	HID_NKRO_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: reportKeyInInCCC
	HID_NKRO_REPORT_INPUT_REF_H, 			//UUID: 2908    VALUE: REPORT_ID_NKRO, TYPE_INPUT

	//mouse report in
	HID_MOUSE_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	HID_MOUSE_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: reportMouseIn
	HID_MOUSE_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: reportMouseInCCC
	HID_MOUSE_REPORT_INPUT_REF_H,			//UUID: 2908    VALUE: REPORT_ID_MOUSE, TYPE_INPUT

	// report map
	HID_REPORT_MAP_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read
	HID_REPORT_MAP_DP_H,					//UUID: 2A4B, 	VALUE: reportKeyIn
	//HID_REPORT_MAP_EXT_REF_H,				//UUID: 2907 	VALUE: extService

	//hid information
	HID_INFORMATION_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read
	HID_INFORMATION_DP_H,					//UUID: 2A4A 	VALUE: hidInformation
	
	//control point
	HID_CONTROL_POINT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: write_without_rsp
	HID_CONTROL_POINT_DP_H,					//UUID: 2A4C 	VALUE: controlPoint

	ATT_END_H,

}BLE_ATT_HANDLE_TYPE;


typedef struct
{
    /** Minimum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMin;
    /** Maximum value for the connection event (interval. 0x0006 - 0x0C80 * 1.25 ms) */
    u16 intervalMax;
    /** Number of LL latency connection events (0x0000 - 0x03e8) */
    u16 latency;
    /** Connection Timeout (0x000A - 0x0C80 * 10 ms) */
    u16 timeout;

} gap_periConnectParams_t;


extern void ble_app_init();
extern void ble_update_battery(u8 val);
extern void ble_send_keyboard(u8* data);
extern void ble_send_consumer(u16 data);
extern void ble_send_system(u16 data);
extern void ble_send_nkro(u8* data);
extern void ble_send_mouse(u8* data);
extern u8 ble_keyboard_leds();


#endif /* _BLE_APP_H_ */
