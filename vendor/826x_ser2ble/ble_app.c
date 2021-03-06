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
#include "ys_rom.h"
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

//nus Service ......................................................................

#define NUS_UUID_SERVICE   			{0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E}	//!< nordic_nus service
#define NUS_DATA_CLIENT2SERVER   	{0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x02,0x00,0x40,0x6E}	//!< nordic_nus client2server char
#define NUS_DATA_SERVER2CLIENT   	{0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x03,0x00,0x40,0x6E}	//!< nordic_nus service2client char
#define NUS_DATA_COMMAND   			{0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x04,0x00,0x40,0x6E}	//!< nordic_nus command char
#define NUS_DATA_GPIO   			{0x9E,0xCA,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x05,0x00,0x40,0x6E}	//!< nordic_nus gpio char

static const u8 nusServiceUUID[16]	      = NUS_UUID_SERVICE;
static const u8 nusDataClient2ServerUUID[16] = NUS_DATA_CLIENT2SERVER;
static const u8 nusDataServer2ClientUUID[16] = NUS_DATA_SERVER2CLIENT;
static const u8 nusDataCommandUUID[16] = NUS_DATA_COMMAND;
static const u8 nusDataGPIOUUID[16] = NUS_DATA_GPIO;

// NUS attribute values
static const u8 nusC2SChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(NUS_Client2Server_OUT_DP_H), U16_HI(NUS_Client2Server_OUT_DP_H),
	NUS_DATA_CLIENT2SERVER,
};
static const u8 nusS2CChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(NUS_Server2Client_INPUT_DP_H), U16_HI(NUS_Server2Client_INPUT_DP_H),
	NUS_DATA_SERVER2CLIENT,
};
static const u8 nusCmdChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(NUS_Command_DP_H), U16_HI(NUS_Command_DP_H),
	NUS_DATA_COMMAND,
};
static const u8 nusGPIOChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(NUS_GPIO_DP_H), U16_HI(NUS_GPIO_DP_H),
	NUS_DATA_GPIO,
};

static u8 nusClient2ServerData[ATT_MTU_SIZE - 3];
static u8 nusServer2ClientData[ATT_MTU_SIZE - 3];
static u8 nusServer2ClientDataCCC[2] = {0};
static u8 nusCommandData[ATT_MTU_SIZE - 3];
static u8 nusCommandDataCCC[2] = {0};
static u8 nusIOData;
//NUS data descriptor
const u8 nusC2SDescriptor[] = "NUS: Phone->Module";
const u8 nusS2CDescriptor[] = "NUS: Module->Phone";
const u8 nusCommandDescriptor[] = "NUS: Command";
const u8 nusGPIODescriptor[] = "NUS: GPIO";

_attribute_ram_code_ static int nus_onReceiveData(rf_packet_att_write_t *p);
_attribute_ram_code_ static int nus_onCommandData(rf_packet_att_write_t *p);
_attribute_ram_code_ static int nus_onGpioData(rf_packet_att_write_t *p);

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

//HID Service ......................................................................

static const u16 hidServiceUUID = SERVICE_UUID_HUMAN_INTERFACE_DEVICE;
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

static u8 protocolMode = DFLT_HID_PROTOCOL_MODE;

static u8 reportKeyboardIn[8] = {0};
static u8 reportKeyboardInCCC[2] = {0};
static u8 reportRefKeyboardIn[2] = { REPORT_ID_KEYBOARD_INPUT_BLE, HID_REPORT_TYPE_INPUT };

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
};

// TM : to modify
static const attribute_t blenus_attributes[] = {

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

////////////////////////////////////// NUS Service /////////////////////////////////////////////////////
	{NUS_GPIO_DESC_H - NUS_PS_H + 1, ATT_PERMISSIONS_READ, 2, 16, (u8*)(&primaryServiceUUID), (u8*)(&nusServiceUUID), 0},

	// Client2Server
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusC2SChar), (u8*)(&characterUUID), (u8*)(nusC2SChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR,16, sizeof(nusClient2ServerData), (u8*)(&nusDataClient2ServerUUID), (u8*)(nusClient2ServerData), &nus_onReceiveData},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusC2SDescriptor), (u8*)(&userDescUUID),(u8*)(&nusC2SDescriptor)},

	// Server2Client
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusS2CChar), (u8*)(&characterUUID), (u8*)(nusS2CChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ,16, sizeof(nusServer2ClientData), (u8*)(&nusDataServer2ClientUUID), (u8*)(nusServer2ClientData), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(nusServer2ClientDataCCC), (u8*)(&clientCharacterCfgUUID), 	(u8*)(nusServer2ClientDataCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusS2CDescriptor), (u8*)(&userDescUUID),(u8*)(&nusS2CDescriptor)},

	// Command
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusCmdChar), (u8*)(&characterUUID), (u8*)(nusCmdChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR,16, sizeof(nusCommandData), (u8*)(&nusDataCommandUUID), (u8*)(nusCommandData), &nus_onCommandData},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(nusCommandDataCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(nusCommandDataCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusCommandDescriptor), (u8*)(&userDescUUID),(u8*)(&nusCommandDescriptor)},

	// GPIO
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusGPIOChar), (u8*)(&characterUUID), (u8*)(nusGPIOChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR,16, sizeof(nusIOData), (u8*)(&nusDataGPIOUUID), (u8*)(&nusIOData), &nus_onGpioData},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(nusGPIODescriptor), (u8*)(&userDescUUID),(u8*)(&nusGPIODescriptor)},

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

	// 0011 - 0012  protocol mode
	{0, ATT_PERMISSIONS_READ, 2, sizeof(protocolModeChar), (u8*)(&characterUUID), (u8*)(protocolModeChar), 0},		//prop
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(protocolMode), (u8*)(protocolModeChar+3), (u8*)(&protocolMode), 0},	//value

	// 0013 - 0015  normal keyboard input report (char-val-client-ref)
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyboardInChar), (u8*)(&characterUUID), (u8*)(reportKeyboardInChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportKeyboardIn), (u8*)(reportKeyboardInChar+3), (u8*)(reportKeyboardIn), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(reportKeyboardInCCC), (u8*)(&clientCharacterCfgUUID), (u8*)(reportKeyboardInCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(reportRefKeyboardIn), (u8*)(&reportRefUUID), (u8*)(reportRefKeyboardIn), 0},   //value

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
#define ADV_DEVICE_NAME_POS   		11
#define ADV_DEVICE_NAME_MAX_LEN  	18
	u8 adv_data_raw[31] = {
		0x02, 0x01, 0x05, 		// BLE limited discoverable mode and BR/EDR not supported
		0x03, 0x19, 0xc1, 0x03, // 384, Generic Remote Control, Generic category
		0x03, 0x03, 0x12, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
		19, 0x09, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

	ble_drv_init();
	bls_att_setAttributeTable((u8 *)blenus_attributes);

	u16 mac16 = *(volatile u16 *)CFG_ADR_MAC;
	u8 name_len = strlen(device_config.dev_name);
	memcpy(adv_data_raw + ADV_DEVICE_NAME_POS + 2, device_config.dev_name, name_len);
    ascii_to_hex(adv_data_raw + ADV_DEVICE_NAME_POS + 2 + name_len, (u8 *)&mac16, 2);
	adv_data_raw[ADV_DEVICE_NAME_POS] = name_len + 5;

	ble_start_advertis(adv_data_raw, name_len+ADV_DEVICE_NAME_POS+6, adv_data_raw+ADV_DEVICE_NAME_POS, name_len+6);
	//blc_att_registerMtuSizeExchangeCb((att_mtuSizeExchange_callback_t *)&mtu_size_exchange_func);
}

_attribute_ram_code_ static int nus_onReceiveData(rf_packet_att_write_t *p)
{
	ble_nus_recv_handler(&p->value, p->l2capLen - 3);
	return 0;
}

_attribute_ram_code_ static int nus_onCommandData(rf_packet_att_write_t *p)
{
	ble_nus_cmd_handler(&p->value, p->l2capLen - 3);
	return 0;
}

_attribute_ram_code_ static int nus_onGpioData(rf_packet_att_write_t *p)
{
	ble_nus_gpio_handler(p->value);
	return 0;
}

_attribute_ram_code_ int ble_nus_send_data(u8 *data, u8 len)
{
   	if(blc_ll_getCurrentState() != BLS_LINK_STATE_CONN)
		return 0;

	return bls_att_pushNotifyData(NUS_Server2Client_INPUT_DP_H, data, len);
}

__attribute__ ((weak)) int ble_nus_recv_handler(u8 *data, u8 len)
{
	YS_LOG("nus recv data %d", len);
	return 0;
}

__attribute__ ((weak)) int ble_nus_cmd_handler(u8 *data, u8 len)
{
	YS_LOG("nus cmd data %d", len);
	return 0;
}

__attribute__ ((weak)) int ble_nus_gpio_handler(u8 data)
{
	YS_LOG("nus gpio data 0x%x", data);
	return 0;
}
