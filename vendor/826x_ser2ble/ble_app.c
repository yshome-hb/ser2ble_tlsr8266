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

static const u16 appearance = GAP_APPEARE_UNKNOWN;

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

//spp Service ......................................................................

static u8 TelinkSppServiceUUID[16]	      	 = TELINK_SPP_UUID_SERVICE;
static u8 TelinkSppDataServer2ClientUUID[16] = TELINK_SPP_DATA_SERVER2CLIENT;
static u8 TelinkSppDataClient2ServerUUID[16] = TELINK_SPP_DATA_CLIENT2SERVER;
// SPP attribute values
static const u8 sppS2CChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_NOTIFY,
	U16_LO(SPP_Server2Client_INPUT_DP_H), U16_HI(SPP_Server2Client_INPUT_DP_H),
	TELINK_SPP_DATA_SERVER2CLIENT,
};
static const u8 sppC2SChar[19] = {
	CHAR_PROP_READ | CHAR_PROP_WRITE_WITHOUT_RSP,
	U16_LO(SPP_Client2Server_OUT_DP_H), U16_HI(SPP_Client2Server_OUT_DP_H),
	TELINK_SPP_DATA_CLIENT2SERVER,
};
static u8 SppDataServer2ClientData[ATT_MTU_SIZE - 3];
static u8 SppDataServer2ClientDataCCC[2] = {0};
static u8  SppDataClient2ServerData[ATT_MTU_SIZE - 3];
//SPP data descriptor
const u8 TelinkSPPS2CDescriptor[] = "Telink SPP: Module->Phone";
const u8 TelinkSPPC2SDescriptor[] = "Telink SPP: Phone->Module";

_attribute_ram_code_ static int spp_onReceiveData(rf_packet_att_write_t *p);

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
static const attribute_t blespp_attributes[] = {

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

////////////////////////////////////// SPP Service /////////////////////////////////////////////////////
	{SPP_Client2Server_DESC_H - SPP_PS_H + 1, ATT_PERMISSIONS_READ, 2, 16, (u8*)(&primaryServiceUUID), (u8*)(&TelinkSppServiceUUID), 0},

	// Server2Client
	{0, ATT_PERMISSIONS_READ, 2, sizeof(sppS2CChar), (u8*)(&characterUUID), (u8*)(sppS2CChar), 0},	//prop
	{0, ATT_PERMISSIONS_READ,16, sizeof(SppDataServer2ClientData), (u8*)(&TelinkSppDataServer2ClientUUID), (u8*)(SppDataServer2ClientData), 0},	//value
	{0, ATT_PERMISSIONS_RDWR, 2, sizeof(SppDataServer2ClientDataCCC), (u8*)(&clientCharacterCfgUUID), 	(u8*)(SppDataServer2ClientDataCCC), 0},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(TelinkSPPS2CDescriptor), (u8*)(&userDescUUID),(u8*)(&TelinkSPPS2CDescriptor)},

	// Client2Server
	{0, ATT_PERMISSIONS_READ, 2, sizeof(sppC2SChar), (u8*)(&characterUUID), (u8*)(sppC2SChar), 0},	//prop
	{0, ATT_PERMISSIONS_RDWR,16, sizeof(SppDataClient2ServerData), (u8*)(&TelinkSppDataClient2ServerUUID), (u8*)(SppDataClient2ServerData), &spp_onReceiveData},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(TelinkSPPC2SDescriptor), (u8*)(&userDescUUID),(u8*)(&TelinkSPPC2SDescriptor)},

#if (BLE_OTA_ENABLE)
	////////////////////////////////////// OTA ////////////////////////////////////////////////////////////////
	// 002e - 0031
	{4, ATT_PERMISSIONS_READ, 2, sizeof(otaServiceUUID), (u8*)(&primaryServiceUUID), (u8*)(otaServiceUUID), 0},
	{0, ATT_PERMISSIONS_READ, 2, sizeof(otaDataChar),(u8*)(&characterUUID), (u8*)(otaDataChar), 0},			//prop
	{0, ATT_PERMISSIONS_RDWR, 16, sizeof(otaData), (u8*)(otaDataChar+3), (&otaData), &otaWrite, &otaRead},	//value
	{0, ATT_PERMISSIONS_READ, 2, sizeof(otaName), (u8*)(&userDescUUID), (u8*)(otaName), 0},
#endif

};

static void ascii_to_hex(u8 *rp, u8 *sp, u8 len)
{
    static const u8 hex[16] = { 
		'0', '1', '2', '3', '4', '5', '6', '7', 
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    u8 n = 0;
    for(u8 i = 0; i < len; i++)
    {
        rp[n++] = hex[sp[i] >> 4];
        rp[n++] = hex[sp[i] & 0x0F];
    }
}

void ble_app_init ()
{
#define ADV_DEVICE_NAME_POS   		7
#define ADV_DEVICE_NAME_MAX_LEN  	18
	u8 adv_data_raw[31] = {
		0x02, 0x01, 0x05, 		// BLE limited discoverable mode and BR/EDR not supported
		0x03, 0x03, 0x10, 0x19,	// incomplete list of service class UUIDs (0x1910)
		19, 0x09, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, };

	u8  tbl_mac[] = {0xe1, 0xe1, 0xe2, 0xe3, 0xe4, 0xc7};
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
	bls_att_setAttributeTable((u8 *)blespp_attributes);

	u8 name_len = strlen(devName);
	memcpy(adv_data_raw + ADV_DEVICE_NAME_POS + 2, devName, name_len);
	ascii_to_hex(adv_data_raw + ADV_DEVICE_NAME_POS + name_len + 2, tbl_mac, 2);
	adv_data_raw[ADV_DEVICE_NAME_POS] = name_len + 5;

	ble_start_advertis(adv_data_raw, name_len+ADV_DEVICE_NAME_POS+6, adv_data_raw+ADV_DEVICE_NAME_POS, name_len+6);
	//blc_att_registerMtuSizeExchangeCb((att_mtuSizeExchange_callback_t *)&mtu_size_exchange_func);
}

_attribute_ram_code_ static int spp_onReceiveData(rf_packet_att_write_t *p)
{
	ble_spp_recv_handler(&p->value, p->l2capLen - 3);
	return 0;
}

_attribute_ram_code_ int ble_spp_send_data(u8 *data, u8 len)
{
   	if(blc_ll_getCurrentState() != BLS_LINK_STATE_CONN)
		return 0;

	return bls_att_pushNotifyData(SPP_Server2Client_INPUT_DP_H, data, len);
}

__attribute__ ((weak)) int ble_spp_recv_handler(u8 *data, u8 len)
{
	YS_LOG("spp recv data %d", len);
	return 0;
}
