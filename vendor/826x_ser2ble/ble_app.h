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

/********telink spp**************************************************************************************/
	SPP_PS_H, 								//UUID: 2800, 	VALUE: telink audio service uuid

	//Server2Client
	SPP_Server2Client_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	SPP_Server2Client_INPUT_DP_H,			//UUID: TELIK_SPP_Server2Client uuid,  VALUE: SppDataServer2ClientData
	SPP_Server2Client_INPUT_CCB_H,			//UUID: 2902 	VALUE: SppDataServer2ClientDataCCC
	SPP_Server2Client_INPUT_DESC_H,			//UUID: 2901, 	VALUE: TelinkSPPS2CDescriptor

	//Client2Server
	SPP_Client2Server_OUT_CD_H,				//UUID: 2803, 	VALUE:  			Prop: Read | write_without_rsp
	SPP_Client2Server_OUT_DP_H,				//UUID: TELIK_SPP_Client2Server uuid,  VALUE: SppDataClient2ServerData
	SPP_Client2Server_DESC_H,				//UUID: 2901, 	VALUE: TelinkSPPC2SDescriptor

#if (BLE_OTA_ENABLE)
/**********OTA***********************************************************************************/
	OTA_PS_H,								//UUID: 2800,	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803,	VALUE:				Prop: read | write_without_rsp	
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_OUT_DESC_H,						//UUID: 2901,	VALUE: otaName
#endif

	ATT_END_H,

}BLE_ATT_HANDLE_TYPE;


extern void ble_app_init();
extern void ble_spp_send_data(u8 *data, u8 len);
extern void ble_spp_recv_handler(u8 *data, u8 len);


#endif /* _BLE_APP_H_ */
