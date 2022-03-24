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

#include "../../proj/common/types.h"
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

/********nodric nus**************************************************************************************/
	NUS_PS_H, 								//UUID: 2800, 	VALUE: nordic nus service uuid

	//Client2Server
	NUS_Client2Server_OUT_CD_H,				//UUID: 2803, 	VALUE:  			Prop: Read | write_without_rsp
	NUS_Client2Server_OUT_DP_H,				//UUID: NUS_Client2Server uuid,  VALUE: nusClient2ServerData
	NUS_Client2Server_DESC_H,				//UUID: 2901, 	VALUE: nusC2SDescriptor

	//Server2Client
	NUS_Server2Client_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	NUS_Server2Client_INPUT_DP_H,			//UUID: NUS_Server2Client uuid,  VALUE: nusServer2ClientData
	NUS_Server2Client_INPUT_CCB_H,			//UUID: 2902 	VALUE: nusServer2ClientDataCCC
	NUS_Server2Client_INPUT_DESC_H,			//UUID: 2901, 	VALUE: nusS2CDescriptor

	//Command
	NUS_Command_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read | write_without_rsp
	NUS_Command_DP_H,						//UUID: NUS_Command uuid,  VALUE: nusCommandData
	NUS_Command_CCB_H,						//UUID: 2902 	VALUE: nusCommandDataCCC
	NUS_Command_DESC_H,						//UUID: 2901, 	VALUE: nusCommandDescriptor

	//GPIO_Output
	NUS_GPIO_CD_H,							//UUID: 2803, 	VALUE:  			Prop: Read | write_without_rsp
	NUS_GPIO_DP_H,							//UUID: NUS_GPIO uuid,  VALUE: nusIOData
	NUS_GPIO_DESC_H,						//UUID: 2901, 	VALUE: nusGPIODescriptor

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
extern int ble_nus_send_data(u8 *data, u8 len);
extern int ble_nus_recv_handler(u8 *data, u8 len);
extern int ble_nus_cmd_handler(u8 *data, u8 len);
extern int ble_nus_gpio_handler(u8 data);


#endif /* _BLE_APP_H_ */
