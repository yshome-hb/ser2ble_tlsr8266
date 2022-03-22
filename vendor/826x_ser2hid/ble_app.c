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
#include "../../proj_lib/ble/service/ble_ll_ota.h"
#include "ble_drv.h"
#include "ble_app.h"


#define RX_FIFO_SIZE	64
#define RX_FIFO_NUM		8

#define TX_FIFO_SIZE	40
#define TX_FIFO_NUM		16

MYFIFO_INIT(blt_rxfifo, RX_FIFO_SIZE, RX_FIFO_NUM);
MYFIFO_INIT(blt_txfifo, TX_FIFO_SIZE, TX_FIFO_NUM);

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

static const u8	devName[] = STRINGIFY(PRODUCT_NAME);

static const u16 appearance = 0x03c1;

static gap_periConnectParams_t periConnParameters = {
	BLE_CONN_INTERVAL_MIN, 
	BLE_CONN_INTERVAL_MAX, 
	BLE_CONN_LATENCY, 
	BLE_CONN_TIMEOUT
};

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

static const u8	pnPtrs [] = {
	0x02, 
	U16_LO(ID_VENDOR), U16_HI(ID_VENDOR),
	U16_LO(ID_PRODUCT), U16_HI(ID_PRODUCT),
	U16_LO(ID_VERSION), U16_HI(ID_VERSION)};


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

static const u16 hidServiceUUID = SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
static const u16 hidIncludeUUID = GATT_UUID_INCLUDE;
static const u16 hidReportUUID  = CHARACTERISTIC_UUID_HID_REPORT;
static const u16 reportRefUUID = GATT_UUID_REPORT_REF;
static const u16 extReportRefUUID = GATT_UUID_EXT_REPORT_REF;

// HID attribute values
static const u8 protocolModeChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_PROTOCOL_MODE_DP_H), U16_HI(HID_PROTOCOL_MODE_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_PROTOCOL_MODE), U16_HI(CHARACTERISTIC_UUID_HID_PROTOCOL_MODE)
};
static const u8 reportKeyboardInChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_KEYBOARD_REPORT_INPUT_DP_H), U16_HI(HID_KEYBOARD_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 reportKeyboardOutChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(HID_KEYBOARD_REPORT_OUTPUT_DP_H), U16_HI(HID_KEYBOARD_REPORT_OUTPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};

static const u8 reportConsumerChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_CONSUMER_REPORT_INPUT_DP_H), U16_HI(HID_CONSUMER_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 reportSystemChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_SYSTEM_REPORT_INPUT_DP_H), U16_HI(HID_SYSTEM_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 reportNkroChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_NKRO_REPORT_INPUT_DP_H), U16_HI(HID_NKRO_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};
static const u8 reportMouseChar[5] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(HID_MOUSE_REPORT_INPUT_DP_H), U16_HI(HID_MOUSE_REPORT_INPUT_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT), U16_HI(CHARACTERISTIC_UUID_HID_REPORT)
};

static const u8 hidReportMapChar[5] = {
	CHAR_PROP_READ,
	U16_LO(HID_REPORT_MAP_DP_H), U16_HI(HID_REPORT_MAP_DP_H),
	U16_LO(CHARACTERISTIC_UUID_HID_REPORT_MAP), U16_HI(CHARACTERISTIC_UUID_HID_REPORT_MAP)
};
static const u8 hidInformationChar[5] = {
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
static u16 hidInclude[3] = {BATT_PS_H, BATT_LEVEL_INPUT_CCB_H, SERVICE_UUID_BATTERY};

static u8 protocolMode = DFLT_HID_PROTOCOL_MODE;

static u8 reportKeyboardIn[8] = {0};
static u8 reportKeyboardInCCC[2] = {0};
static u8 reportRefKeyboardIn[2] = { REPORT_ID_KEYBOARD_INPUT_BLE, HID_REPORT_TYPE_INPUT };

static u8 reportKeyboardOut[1] = {0};
static u8 reportRefKeyboardOut[2] = { REPORT_ID_KEYBOARD_INPUT_BLE, HID_REPORT_TYPE_OUTPUT };

static u8 reportConsumerIn[2] = {0};
static u8 reportConsumerInCCC[2] = {0};
static u8 reportRefConsumerIn[2] = { REPORT_ID_CONSUMER_INPUT_BLE, HID_REPORT_TYPE_INPUT };

static u8 reportSystemIn[2] = {0};
static u8 reportSystemInCCC[2] = {0};
static u8 reportRefSystemIn[2] = { REPORT_ID_SYSTEM_INPUT_BLE, HID_REPORT_TYPE_INPUT };

static u8 reportNkroIn[8] = {0};
static u8 reportNkroInCCC[2] = {0};
static u8 reportRefNkroIn[2] = { REPORT_ID_NKRO_INPUT_BLE, HID_REPORT_TYPE_INPUT };

static u8 reportMouseIn[5];
static u8 reportMouseInCCC[2] = {0};
static u8 reportRefMouseIn[2] = { REPORT_ID_MOUSE_INPUT_BLE, HID_REPORT_TYPE_INPUT };

static int keyOutWrite(void* p)
{
	reportKeyboardOut[0] = ((rf_packet_att_write_t *)p)->value;
	return 0;
}

const u8 hidInformation[] =
{
	U16_LO(0x0111), U16_HI(0x0111),    // bcdHID (USB HID version)
	0x00,                              // bCountryCode
	0x01                               // Flags
};

static u8 hidControlPoint;

// HID Report Map characteristic
static const u8 hidReportMap[] =
{
/***************** keyboard report in *****************/
	0x05, 0x01,	 // Usage Page (Generic Desktop)
	0x09, 0x06,	 // Usage (Keyboard)
	0xA1, 0x01,	 // Collection: (Application)
	0x85, REPORT_ID_KEYBOARD_INPUT_BLE,	 // Report Id (keyboard)
				 // Modifier byte
	0x05, 0x07,	 // Usage Page (Key Codes)
	0x19, 0xE0,	 // Usage Min (224)  VK_CTRL:0xe0
	0x29, 0xE7,	 // Usage Max (231)  VK_RWIN:0xe7
	0x15, 0x00,	 // Log Min (0)
	0x25, 0x01,	 // Log Max (1)
	0x95, 0x08,	 // Report Count (8)
	0x75, 0x01,	 // Report Size (1)   1 bit * 8
	0x81, 0x02,	 // Input: (Data, Variable, Absolute)
				 // Reserved byte
	0x95, 0x01,	 // Report Count (1)
	0x75, 0x08,	 // Report Size (8)
	0x81, 0x01,	 // Input: (Constant)
	//5 bit led ctrl: NumLock CapsLock ScrollLock Compose kana
				 // LED byte
	0x05, 0x08,	 // Usage Page (LEDs)
	0x19, 0x01,	 // Usage Min
	0x29, 0x05,	 // Usage Max
	0x95, 0x05,	 // Report Count (5)
	0x75, 0x01,	 // Report Size (1)
	0x91, 0x02,	 // Output (Data, Variable, Absolute)
				 // 3 bit reserved
	0x95, 0x01,	 // Report Count (1)
	0x75, 0x03,	 // Report Size (3)
	0x91, 0x01,	 // Output (Constant)
				 // Key arrays (6 bytes)
	0x05, 0x07,	 // Usage Page (Key Codes)
	0x19, 0x00,	 // Usage Min (0)
	0x29, 0xF1,  // Usage Max (241)
	0x15, 0x00,	 // Log Min (0)
	0x25, 0xF1,	 // Log Max (241)
	0x95, 0x06,	 // Report Count (6)
	0x75, 0x08,	 // Report Size (8)
	0x81, 0x00,	 // Input: (Data, Array)
	0xC0,		 // End Collection

/***************** consumer report in *****************/
	0x05, 0x0C,  // Usage Page (Consumer)
	0x09, 0x01,  // Usage (Consumer Control)
	0xA1, 0x01,  // Collection (Application)
	0x85, REPORT_ID_CONSUMER_INPUT_BLE,  //Report Id
	0x19, 0x01, 	  // Usage Min (0x01)
	0x2a, 0x8c, 0x02, // Usage Max (0x28c)
	0x15, 0x01, 	  // Log Min (0x01)
	0x26, 0x8c, 0x02, // Log Max (0x28c)
	0x95, 0x01,  // Report Count (1)
	0x75, 0x10,  // Report Size (16 bits)
	0x81, 0x00,  // Input: (Data, Array)
	0xC0,		 // End Collection

/***************** system report in *****************/
	0x05, 0x01,  // Usage Page (Generic Desktop Control)
	0x09, 0x80,	 // Usage (System Control)
	0xA1, 0x01,	 // Collection (Application)
	0x85, REPORT_ID_SYSTEM_INPUT_BLE,		// Report ID (ACPI)
	0x19, 0x01,  // Usage Min (0x01)
	0x29, 0xB7,  // Usage Max (0xB7)
	0x15, 0x01,	 // Log Min (0x01)
	0x26, 0xB7, 0x00, // Log Max (0xB7)
	0x95, 0x01,	 // Report Count (1)
	0x75, 0x10,	 // Report Size (16)
	0x81, 0x00,	 // Input (Data, Variable, Absolute)
	0xC0,		 // End Collection

/***************** nkro report in *****************/
	0x05, 0x01,	 // Usage Page (Generic Desktop)
	0x09, 0x06,	 // Usage (Keyboard)
	0xA1, 0x01,	 // Collection: (Application)
	0x85, REPORT_ID_NKRO_INPUT_BLE,	 // Report Id (nkro)
				 // Reserved byte
	0x95, 0x01,	 // Report Count (1)
	0x75, 0x08,	 // Report Size (8)
	0x81, 0x01,	 // Input: (Constant)
	0x05, 0x07,	 // Usage Page (Key Codes)
	0x15, 0x00,	 // Log Min (0)
	0x25, 0x01,	 // Log Max (1)
	0x19, 0x00,	 // Usage Min (0)
	0x29, 119,  // Usage Max (119)
	0x95, 120,	// Report Count (120)
	0x75, 0x01,	 // Report Size (1)
	0x81, 0x02,	 // Input: (Data, Array)	
	0xC0,		 // End Collection

/***************** mouse report in *****************/
	0x05, 0x01,   // Usage Page (Generic Desktop)
	0x09, 0x02,   // Usage (Mouse)
	0xA1, 0x01,   // Collection (Application)
	0x85, REPORT_ID_MOUSE_INPUT_BLE,  // Report Id
	0x09, 0x01,   // Usage (Pointer)
	0xA1, 0x00,   // Collection (Physical)
	// ----------------------------  Buttons
	0x05, 0x09,   // Usage Page (Buttons)
	0x19, 0x01,   // Usage Minimum (01) - Button 1
	0x29, 0x05,   // Usage Maximum (05) - Button 5
	0x15, 0x00,   // Logical Minimum (0)
	0x25, 0x01,   // Logical Maximum (1)
	0x75, 0x01,   // Report Size (1)
	0x95, 0x05,   // Report Count (5)
	0x81, 0x02,   // Input (Data, Variable, Absolute) - Button states
	0x75, 0x03,   // Report Size (3)
	0x95, 0x01,   // Report Count (1)
	0x81, 0x01,   // Input (Constant) - Padding or Reserved bits
	// ----------------------------  X,Y position
	0x05, 0x01,   // Usage Page (Generic Desktop Control)
	0x09, 0x30,   // Usage (X)
	0x09, 0x31,   // Usage (Y)
	0x15, 0x81,   // Logical Minimum (-127)
	0x25, 0x7F,   // Logical Maximum (127)
	0x75, 0x08,   // Report Size (8)
	0x95, 0x02,   // Report Count (2)
	0x81, 0x06,   // Input (Data, Variable, Relative)
	// ----------------------------  Vertical wheel
	0x09, 0x38,	  // Usage (Wheel)
	0x15, 0x81,   // Logical Minimum (-127)
	0x25, 0x7F,   // Logical Maximum (127)
  	0x35, 0x00,   // Physical Minimum (0) - reset physical
  	0x45, 0x00,   // Physical maximum (0)	
	0x75, 0x08,   // Report Size (8)
	0x95, 0x01,   // Report Count (1)
	0x81, 0x06,   // Input (Data, Variable, Relative)

	0xC0,		  // End Collection
	0xC0,		  // End Collection
};

#if (BLE_OTA_ENABLE)
//ota Service ......................................................................

static const u8 otaServiceUUID[16] = TELINK_OTA_UUID_SERVICE;
static const u16 userDescUUID = GATT_UUID_CHAR_USER_DESC;

static const u8 otaDataChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
 	U16_LO(OTA_CMD_OUT_DP_H), U16_HI(OTA_CMD_OUT_DP_H),
	//TELINK_SPP_DATA_OTA,
	0x12,0x2B,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00
};

static u8 otaData = 0x00;
static const u8 otaName[] = {'O', 'T', 'A'};

#endif

// TM : to modify
static const attribute_t hidkb_attributes[] = {

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
	{0, ATT_PERMISSIONS_READ, 2, sizeof(batteryChar), (u8*)(&characterUUID), (u8*)(batteryChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(batteryValue),(u8*)(batteryChar+3), (u8*)(batteryValue), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(batteryValueInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(batteryValueInCCC), 0},	//value

#if (BLE_OTA_ENABLE)
	////////////////////////////////////// OTA ////////////////////////////////////////////////////////////////
	// 002e - 0031
	{4, ATT_PERMISSIONS_READ, 2, sizeof(otaServiceUUID), (u8*)(&primaryServiceUUID), (u8*)(otaServiceUUID), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(otaDataChar),(u8*)(&characterUUID), (u8*)(otaDataChar), 0},			//prop
	{0, ATT_PERMISSIONS_RDWR, 16, sizeof(otaData), (u8*)(otaDataChar+3), (&otaData), &otaWrite, &otaRead},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(otaName), (u8*)(&userDescUUID), (u8*)(otaName), 0},
#endif

	/////////////////////////////////// HID Service /////////////////////////////////////////////////////////
	{HID_CONTROL_POINT_DP_H - HID_PS_H + 1, ATT_PERMISSIONS_READ, 2, 2,(u8*)(&primaryServiceUUID), (u8*)(&hidServiceUUID), 0},

	// 0010  include battery service
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidInclude), (u8*)(&hidIncludeUUID), (u8*)(hidInclude), 0},

	// 0011 - 0012  protocol mode
	{0, ATT_PERMISSIONS_READ, 2, sizeof(protocolModeChar), (u8*)(&characterUUID), (u8*)(protocolModeChar), 0},		//prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(protocolMode), (u8*)(protocolModeChar+3), (u8*)(&protocolMode), 0},	//value

	// 0013 - 0015  normal keyboard input report (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyboardInChar), (u8*)(&characterUUID), (u8*)(reportKeyboardInChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyboardIn), (u8*)(reportKeyboardInChar+3), (u8*)(reportKeyboardIn), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyboardInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportKeyboardInCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefKeyboardIn), (u8*)(&reportRefUUID), (u8*)(reportRefKeyboardIn), 0},   //value

	// 0016 - 0017   normal keyboard output report (char-val-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyboardOutChar), (u8*)(&characterUUID), (u8*)(reportKeyboardOutChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyboardOut), (u8*)(reportKeyboardOutChar+3), (u8*)(reportKeyboardOut), &keyOutWrite},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefKeyboardOut), (u8*)(&reportRefUUID), (u8*)(reportRefKeyboardOut), 0}, 	//value

	// 0018 - 001b. consumer report in: 4 (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportConsumerChar), (u8*)(&characterUUID), (u8*)(reportConsumerChar), 0},			//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportConsumerIn), (u8*)(reportConsumerChar+3), (u8*)(reportConsumerIn), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportConsumerInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportConsumerInCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefConsumerIn), (u8*)(&reportRefUUID), (u8*)(reportRefConsumerIn), 0},	//value

	// 001e - 0021 . system report in : 4 (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportSystemChar), (u8*)(&characterUUID), (u8*)(reportSystemChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportSystemIn), (u8*)(reportSystemChar+3),	(u8*)(reportSystemIn), 0}, 	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportSystemInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportSystemInCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefSystemIn), (u8*)(&reportRefUUID), (u8*)(reportRefSystemIn), 0}, //value

	// 001c - 001f . nkro report in : 4 (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportNkroChar), (u8*)(&characterUUID), (u8*)(reportNkroChar), 0},		//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportNkroIn), (u8*)(reportNkroChar+3), (u8*)(reportNkroIn), 0},		//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportNkroInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportNkroInCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefNkroIn), (u8*)(&reportRefUUID), (u8*)(reportRefNkroIn), 0},	//value

    // 002a - 002d . mouse report in : 4 (char-val-client-ref)
    {0, ATT_PERMISSIONS_READ, 2, sizeof(reportMouseChar), (u8*)(&characterUUID), (u8*)(reportMouseChar), 0},	//prop
    {0, ATT_PERMISSIONS_READ, 2, sizeof(reportMouseIn), (u8*)(reportMouseChar+3), (u8*)(reportMouseIn), 0},	//value
    {0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportMouseInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportMouseInCCC), 0}, //value
    {0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefMouseIn), (u8*)(&reportRefUUID), (u8*)(reportRefMouseIn), 0},	//value

	// 0023 - 0025 . report map: 3
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportMapChar), (u8*)(&characterUUID), (u8*)(hidReportMapChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidReportMap), (u8*)(hidReportMapChar+3), (u8*)(hidReportMap), 0},		//value
	//{0, ATT_PERMISSIONS_RDWR, 2, sizeof(extServiceUUID),(u8*)(&extReportRefUUID), (u8*)(&extServiceUUID), 0},	//value

	// 0026 - 0027 . hid information: 2
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidInformationChar), (u8*)(&characterUUID), (u8*)(hidInformationChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidInformation), (u8*)(hidInformationChar+3), (u8*)(hidInformation), 0},	//value

	// 0028 - 0029 . control point: 2
	{0, ATT_PERMISSIONS_READ, 2, sizeof(hidCtrlPointChar), (u8*)(&characterUUID), (u8*)(hidCtrlPointChar), 0},	//prop
	{0, ATT_PERMISSIONS_WRITE, 2, sizeof(hidControlPoint), (u8*)(hidCtrlPointChar+3), (u8*)(&hidControlPoint), 0},	//value

};

void ble_app_init ()
{
	u8  tbl_mac [] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};
	u32 *pmac = (volatile u32 *) CFG_ADR_MAC;
	if (*pmac == U32_MAX)
	{
		generateRandomNum(sizeof(tbl_mac), tbl_mac);
		flash_write_page(CFG_ADR_MAC, sizeof(tbl_mac), tbl_mac);
	}
	else
	{
		flash_read_page(CFG_ADR_MAC, sizeof(tbl_mac), tbl_mac);
	}

	ble_drv_init(tbl_mac);

	bls_att_setAttributeTable((u8 *)hidkb_attributes);
	ble_start_advertis(devName);
}

void ble_update_battery (u8 val)
{
	if(batteryValue[0] == val)
		return;
	
	batteryValue[0] = val;
    if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(BATT_LEVEL_INPUT_DP_H, &val, 1);
}

void ble_send_keyboard(u8* data)
{
   	if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(HID_KEYBOARD_REPORT_INPUT_DP_H, data, 8);
}

void ble_send_consumer(u16 data)
{	
   	if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(HID_CONSUMER_REPORT_INPUT_DP_H, (u8 *)&data, sizeof(data));
}

void ble_send_system(u16 data)
{
   	if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(HID_SYSTEM_REPORT_INPUT_DP_H, (u8 *)&data, sizeof(data));
}

void ble_send_nkro(u8* data)
{
   	if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(HID_NKRO_REPORT_INPUT_DP_H, data, 15);
}

void ble_send_mouse(u8* data)
{
   	if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		bls_att_pushNotifyData(HID_MOUSE_REPORT_INPUT_DP_H, data, 4);
}

u8 ble_keyboard_leds()
{
	return reportKeyboardOut[0];
}


