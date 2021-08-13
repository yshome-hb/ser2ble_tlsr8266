/********************************************************************************************************
 * @file     ble_app.h
 *
 * @brief    ble application functions
 *
 * @author	 ysshen
 * @date     2021.08
 *
 * @par      Copyright (c) 2021, MIIIW Co., Ltd.
 *           All rights reserved.
 *           
 *******************************************************************************************************/

#include "../../proj_lib/ble/ll/ll.h"
#include "ble_drv.h"
#include "ble_app.h"


//GenericAccess Service ......................................................................

static const u16 primaryServiceUUID = GATT_UUID_PRIMARY_SERVICE;
static const u16 gapServiceUUID = SERVICE_UUID_GENERIC_ACCESS;
static const u16 characterUUID = GATT_UUID_CHARACTER;
static const u16 gattServiceUUID = SERVICE_UUID_GENERIC_ATTRIBUTE;
static const u16 clientCharacterCfgUUID = GATT_UUID_CLIENT_CHAR_CFG;

static const u8 devNameChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(GenericAccess_DeviceName_DP_H), U16_HI(GenericAccess_DeviceName_DP_H),
	U16_LO(GATT_UUID_DEVICE_NAME), U16_HI(GATT_UUID_DEVICE_NAME)
};

static const u8 appearanceChar[5] = {
	CHAR_PROP_READ,
	U16_LO(GenericAccess_Appearance_DP_H), U16_HI(GenericAccess_Appearance_DP_H),
	U16_LO(GATT_UUID_APPEARANCE), U16_HI(GATT_UUID_APPEARANCE)
};

static const u8 connParamChar[5] = {
	CHAR_PROP_READ,
	U16_LO(CONN_PARAM_DP_H), U16_HI(CONN_PARAM_DP_H),
	U16_LO(GATT_UUID_PERI_CONN_PARAM), U16_HI(GATT_UUID_PERI_CONN_PARAM)
};

static const u8	devName[] = {'t','H','I','D','\0'};

static const u16 appearance = GAP_APPEARE_UNKNOWN;

static gap_periConnectParams_t periConnParameters = {20, 40, 0, 1000};


//Gatt Service ......................................................................

static const u8 serviceChangeChar[5] = {
	CHAR_PROP_INDICATE,
	U16_LO(GenericAttribute_ServiceChanged_DP_H), U16_HI(GenericAttribute_ServiceChanged_DP_H),
	U16_LO(GATT_UUID_SERVICE_CHANGE), U16_HI(GATT_UUID_SERVICE_CHANGE)
};

static u16 serviceChangeVal[2] = {0};
static u8 serviceChangeCCC[2] = {0, 0};


//Device Service ......................................................................

static const u16 deviceServiceUUID = SERVICE_UUID_DEVICE_INFORMATION;

static const u8 pnPtrsChar[5] = {
	CHAR_PROP_READ,
	U16_LO(DeviceInformation_pnpID_DP_H), U16_HI(DeviceInformation_pnpID_DP_H),
	U16_LO(CHARACTERISTIC_UUID_PNP_ID), U16_HI(CHARACTERISTIC_UUID_PNP_ID)
};

static const u8	pnPtrs [] = {0x02, 0x8a, 0x24, 0x66, 0x82, 0x01, 0x00};


//Battery Service ......................................................................

static const u16 batServiceUUID = SERVICE_UUID_BATTERY;

static const u8 batteryChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(BATT_LEVEL_INPUT_DP_H), U16_HI(BATT_LEVEL_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_BATTERY_LEVEL), U16_HI(CHARACTERISTIC_UUID_BATTERY_LEVEL)
};

static u8 batteryValue[1] = {99};
static u8 batteryValueInCCC[2];

//HID Service ......................................................................

static const u16 hidServiceUUID           = SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
static const u16 hidProtocolModeUUID      = CHARACTERISTIC_UUID_HID_PROTOCOL_MODE;
static const u16 hidReportUUID            = CHARACTERISTIC_UUID_HID_REPORT;
static const u16 hidReportMapUUID         = CHARACTERISTIC_UUID_HID_REPORT_MAP;
static const u16 hidbootKeyInReportUUID   = CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT;
static const u16 hidbootKeyOutReportUUID  = CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT;
static const u16 hidbootMouseInReportUUID = CHARACTERISTIC_UUID_HID_BOOT_MOUSE_INPUT;
static const u16 hidinformationUUID       = CHARACTERISTIC_UUID_HID_INFORMATION;
static const u16 hidCtrlPointUUID         = CHARACTERISTIC_UUID_HID_CONTROL_POINT;
static const u16 hidIncludeUUID           = GATT_UUID_INCLUDE;

static const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;
static const u16 reportRefUUID = GATT_UUID_REPORT_REF;

// HID attribute values
static const u8 hidProtocolModeChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_PROTOCOL_MODE_DP_H), U16_HI(HID_PROTOCOL_MODE_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_PROTOCOL_MODE), U16_HI(CHARACTERISTIC_UUID_HID_PROTOCOL_MODE)
};
static const u8 hidbootKeyInReporChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_BOOT_KB_REPORT_INPUT_DP_H), U16_HI(HID_BOOT_KB_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT), U16_HI(CHARACTERISTIC_UUID_HID_BOOT_KEY_INPUT)
};
static const u8 hidbootKeyOutReporChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_BOOT_KB_REPORT_OUTPUT_DP_H), U16_HI(HID_BOOT_KB_REPORT_OUTPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT), U16_HI(CHARACTERISTIC_UUID_HID_BOOT_KEY_OUTPUT)
};
static const u8 hidReportCCinChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_CONSUME_REPORT_INPUT_DP_H), U16_HI(HID_CONSUME_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 hidReportKEYinChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_NORMAL_KB_REPORT_INPUT_DP_H), U16_HI(HID_NORMAL_KB_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 hidReportKEYoutChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_NORMAL_KB_REPORT_OUTPUT_DP_H), U16_HI(HID_NORMAL_KB_REPORT_OUTPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 hidReportMapChar[5] = {
	CHAR_PROP_READ,
	U16_LO(HID_REPORT_MAP_DP_H), U16_HI(HID_REPORT_MAP_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT_MAP), U16_HI(CHARACTERISTIC_UUID_HID_REPORT_MAP)
};
static const u8 hidinformationChar[5] = {
	CHAR_PROP_READ,
	U16_LO(HID_INFORMATION_DP_H), U16_HI(HID_INFORMATION_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_INFORMATION), U16_HI(CHARACTERISTIC_UUID_HID_INFORMATION)
};
static const u8 hidCtrlPointChar[5] = {
	CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_CONTROL_POINT_DP_H), U16_HI(HID_CONTROL_POINT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_CONTROL_POINT), U16_HI(CHARACTERISTIC_UUID_HID_CONTROL_POINT)
};

// Include attribute (Battery service)
static u16 include[3] = {BATT_PS_H, BATT_LEVEL_INPUT_CCB_H, SERVICE_UUID_BATTERY};

static u8 protocolMode = DFLT_HID_PROTOCOL_MODE;

static u8 bootKeyInReport;
static u8 bootKeyInReportCCC[2];

static u8 bootKeyOutReport;

static u8 reportConsumerControlIn[2];
static u8 reportConsumerControlInCCC[2];
static u8 reportRefConsumerControlIn[2] = { HID_REPORT_ID_CONSUME_CONTROL_INPUT, HID_REPORT_TYPE_INPUT };

static u8 reportKeyIn[8];
static u8 reportKeyInCCC[2];
static u8 reportRefKeyIn[2] = { HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_INPUT };

static u8 reportKeyOut[1];
static u8 reportKeyOutCCC[2];
static u8 reportRefKeyOut[2] = { HID_REPORT_ID_KEYBOARD_INPUT, HID_REPORT_TYPE_OUTPUT };

static u16 extServiceUUID;

const u8 hidInformation[] =
{
	U16_LO(0x0111), U16_HI(0x0111),    // bcdHID (USB HID version)
	0x00,                              // bCountryCode
	0x01                               // Flags
};

static u8 controlPoint;


// HID Report Map characteristic
// Keyboard report descriptor (using format for Boot interface descriptor)

static const u8 reportMap[] =
{
	//keyboard report in
	0x05, 0x01,     // Usage Pg (Generic Desktop)
	0x09, 0x06,     // Usage (Keyboard)
	0xA1, 0x01,     // Collection: (Application)
	0x85, HID_REPORT_ID_KEYBOARD_INPUT,     // Report Id (keyboard)
				  //
	0x05, 0x07,     // Usage Pg (Key Codes)
	0x19, 0xE0,     // Usage Min (224)  VK_CTRL:0xe0
	0x29, 0xE7,     // Usage Max (231)  VK_RWIN:0xe7
	0x15, 0x00,     // Log Min (0)
	0x25, 0x01,     // Log Max (1)
				  //
				  // Modifier byte
	0x75, 0x01,     // Report Size (1)   1 bit * 8
	0x95, 0x08,     // Report Count (8)
	0x81, 0x02,     // Input: (Data, Variable, Absolute)
				  //
				  // Reserved byte
	0x95, 0x01,     // Report Count (1)
	0x75, 0x08,     // Report Size (8)
	0x81, 0x01,     // Input: (Constant)

	//keyboard output
	//5 bit led ctrl: NumLock CapsLock ScrollLock Compose kana
	0x95, 0x05,    //Report Count (5)
	0x75, 0x01,    //Report Size (1)
	0x05, 0x08,    //Usage Pg (LEDs )
	0x19, 0x01,    //Usage Min
	0x29, 0x05,    //Usage Max
	0x91, 0x02,    //Output (Data, Variable, Absolute)
	//3 bit reserved
	0x95, 0x01,    //Report Count (1)
	0x75, 0x03,    //Report Size (3)
	0x91, 0x01,    //Output (Constant)

	// Key arrays (6 bytes)
	0x95, 0x06,     // Report Count (6)
	0x75, 0x08,     // Report Size (8)
	0x15, 0x00,     // Log Min (0)
	0x25, 0xF1,     // Log Max (241)
	0x05, 0x07,     // Usage Pg (Key Codes)
	0x19, 0x00,     // Usage Min (0)
	0x29, 0xf1,     // Usage Max (241)
	0x81, 0x00,     // Input: (Data, Array)

	0xC0,            // End Collection




	//consumer report in
	0x05, 0x0C,   // Usage Page (Consumer)
	0x09, 0x01,   // Usage (Consumer Control)
	0xA1, 0x01,   // Collection (Application)
	0x85, HID_REPORT_ID_CONSUME_CONTROL_INPUT,   //     Report Id
	0x75,0x10,     //global, report size 16 bits
	0x95,0x01,     //global, report count 1
	0x15,0x01,     //global, min  0x01
	0x26,0x8c,0x02,  //global, max  0x28c
	0x19,0x01,     //local, min   0x01
	0x2a,0x8c,0x02,  //local, max    0x28c
	0x81,0x00,     //main,  input data varible, absolute
	0xc0,        //main, end collection

};


// TM : to modify
static const attribute_t ble_attributes[] = {

	{ATT_END_H - 1, 0,0,0,0,0},	// total num of attribute

	// 0001 - 0007  gap
	{7, ATT_PERMISSIONS_READ, 2, 2, (u8*)(&primaryServiceUUID), (u8*)(&gapServiceUUID), 0},

	{0, ATT_PERMISSIONS_READ, 2, sizeof(devNameChar), (u8*)(&characterUUID), (u8*)(devNameChar), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(devName), (u8*)(devNameChar+3), (u8*)(devName), 0},

	{0, ATT_PERMISSIONS_READ, 2, sizeof(appearanceChar), (u8*)(&characterUUID), (u8*)(appearanceChar), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(appearance), (u8*)(appearanceChar+3), (u8*)(&appearance), 0},

	{0, ATT_PERMISSIONS_READ, 2, sizeof(connParamChar), (u8*)(&characterUUID), (u8*)(connParamChar), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(periConnParameters), (u8*)(connParamChar+3), (u8*)(&periConnParameters), 0},

	// 0008 - 000b gatt
	{4, ATT_PERMISSIONS_READ, 2, 2, (u8*)(&primaryServiceUUID), (u8*)(&gattServiceUUID), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(serviceChangeChar), (u8*)(&characterUUID), (u8*)(serviceChangeChar), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(serviceChangeVal), (u8*)(serviceChangeChar+3), (u8*)(&serviceChangeVal), 0},
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(serviceChangeCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(serviceChangeCCC), 0},

	// 000c - 000e  device Information Service
	{3, ATT_PERMISSIONS_READ, 2, 2, (u8*)(&primaryServiceUUID), (u8*)(&deviceServiceUUID), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(pnPtrsChar), (u8*)(&characterUUID), (u8*)(pnPtrsChar), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(pnPtrs),(u8*)(pnPtrsChar+3), (u8*)(pnPtrs), 0},

	////////////////////////////////////// Battery Service /////////////////////////////////////////////////////
	// 002a - 002d
	{4, ATT_PERMISSIONS_READ, 2, 2, (u8*)(&primaryServiceUUID), (u8*)(&batServiceUUID), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(batteryChar), (u8*)(&characterUUID), (u8*)(batteryChar), 0},				//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(batteryValue),(u8*)(batteryChar+3), (u8*)(batteryValue), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(batteryValueInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(batteryValueInCCC), 0},	//value

	/////////////////////////////////// HID Service /////////////////////////////////////////////////////////
	{HID_CONTROL_POINT_DP_H - HID_PS_H + 1, ATT_PERMISSIONS_READ, 2, 2,(u8*)(&primaryServiceUUID), (u8*)(&hidServiceUUID), 0},

	// 0010  include battery service
	{0, ATT_PERMISSIONS_READ, 2, sizeof(include), (u8*)(&hidIncludeUUID), (u8*)(include), 0},

	// 0011 - 0012  protocol mode
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidProtocolModeChar), (u8*)(&characterUUID), (u8*)(hidProtocolModeChar), 0},		//prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(protocolMode), (u8*)(&hidProtocolModeUUID), (u8*)(&protocolMode), 0},	//value

	// 0013 - 0015  boot keyboard input report (char-val-client)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidbootKeyInReporChar), (u8*)(&characterUUID), (u8*)(hidbootKeyInReporChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(bootKeyInReport), (u8*)(&hidbootKeyInReportUUID), (u8*)(&bootKeyInReport), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(bootKeyInReportCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(bootKeyInReportCCC), 0},	//value

	// 0016 - 0017   boot keyboard output report (char-val)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidbootKeyOutReporChar), (u8*)(&characterUUID), (u8*)(hidbootKeyOutReporChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(bootKeyOutReport), (u8*)(&hidbootKeyOutReportUUID), (u8*)(&bootKeyOutReport), 0},	//value

	// 0018 - 001b. consume report in: 4 (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportCCinChar), (u8*)(&characterUUID), (u8*)(hidReportCCinChar), 0},			//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportConsumerControlIn), (u8*)(&hidReportUUID), (u8*)(reportConsumerControlIn), 0},	//value
	{0, ATT_PERMISSIONS_RDWR|ATT_PERMISSIONS_AUTHEN_WRITE, 2, sizeof(reportConsumerControlInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportConsumerControlInCCC), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportRefConsumerControlIn), (u8*)(&reportRefUUID), (u8*)(reportRefConsumerControlIn), 0},	//value

	// 001c - 001f . keyboard report in : 4 (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportKEYinChar), (u8*)(&characterUUID), (u8*)(hidReportKEYinChar), 0},		//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyIn), (u8*)(&hidReportUUID), (u8*)(reportKeyIn), 0},		//value
	{0, ATT_PERMISSIONS_RDWR|ATT_PERMISSIONS_AUTHEN_WRITE, 2, sizeof(reportKeyInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportKeyInCCC), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportRefKeyIn), (u8*)(&reportRefUUID), (u8*)(reportRefKeyIn), 0},	//value

	// 0020 - 0022 . keyboard report out: 3 (char-val-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportKEYoutChar), (u8*)(&characterUUID), (u8*)(hidReportKEYoutChar), 0}, //prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyOut), (u8*)(&hidReportUUID), (u8*)(reportKeyOut), 0},			//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportRefKeyOut), (u8*)(&reportRefUUID), (u8*)(reportRefKeyOut), 0},	//value

	// 0023 - 0025 . report map: 3
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportMapChar), (u8*)(&characterUUID), (u8*)(hidReportMapChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportMap), (u8*)(&hidReportMapUUID), (u8*)(reportMap), 0},				//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(extServiceUUID),(u8*)(&extReportRefUUID), (u8*)(&extServiceUUID), 0},	//value

	// 0026 - 0027 . hid information: 2
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidinformationChar), (u8*)(&characterUUID), (u8*)(hidinformationChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidInformation), (u8*)(&hidinformationUUID), (u8*)(hidInformation), 0},		//value

	// 0028 - 0029 . control point: 2
	{0,ATT_PERMISSIONS_READ, 2,sizeof(hidCtrlPointChar),(u8*)(&characterUUID), (u8*)(hidCtrlPointChar), 0},	//prop
	{0,ATT_PERMISSIONS_WRITE,2,sizeof(controlPoint),(u8*)(&hidCtrlPointUUID), (u8*)(&controlPoint), 0},		//value

};

void ble_app_init ()
{
	u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};
	u32 *pmac = (u32 *) CFG_ADR_MAC;
	if (*pmac != 0xffffffff)
	{
		memcpy (tbl_mac, pmac, 6);
	}
	else{
		tbl_mac[0] = (u8)rand();
		flash_write_page (CFG_ADR_MAC, 6, tbl_mac);
	}

	ble_drv_init(tbl_mac);

	bls_att_setAttributeTable((u8 *)ble_attributes);
	ble_start_advertis(devName);
}

