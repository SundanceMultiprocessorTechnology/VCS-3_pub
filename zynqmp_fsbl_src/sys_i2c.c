/**
 ********************************************************************************
 * @file    sys_i2c.c
 * @author  tim
 * @date    Jun 1, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "sys_i2c.h"
#include "xfsbl_hw.h"
#include "xfsbl_debug.h"
#include "xiicps.h"
#include "xstatus.h"
#include "sleep.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

#define I2C_SCLK_RATE				100000

/************************************
 * STATIC VARIABLES
 ************************************/
/************************************
 * GLOBAL VARIABLES
 ************************************/
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/
/************************************
 * STATIC FUNCTIONS
 ************************************/


/************************************
 * GLOBAL FUNCTIONS
 ************************************/
s32 sys_i2c_init(XIicPs* IicInstance, u16 deviceId)
{
	s32 Status;
	XIicPs_Config *ConfigPtr;	/* Pointer to configuration data */

	/*
	 * Initialize the IIC driver so that it is ready to use.
	 */
	ConfigPtr = XIicPs_LookupConfig(deviceId);
	if (ConfigPtr == NULL) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(IicInstance, ConfigPtr,
					ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built correctly.
	 */
	Status = XIicPs_SelfTest(IicInstance);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Set the IIC serial clock rate.
	 */
	Status = XIicPs_SetSClk(IicInstance, I2C_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XIicPs_SetOptions(IicInstance, XIICPS_7_BIT_ADDR_OPTION);

	return XST_SUCCESS;
}

s32 sys_i2c_reg_read(XIicPs* IicInstance, const u16 dev_addr, const u8 reg_addr, u8* data){
	u8 outbuf;
	s32 Status;
	outbuf 			= reg_addr & 0xFF;

	/*
	 * Enable repeated start option.
	 * This call will give an indication to the driver.
	 * The hold bit is actually set before beginning the following transfer
	 */
	XIicPs_SetOptions(IicInstance, XIICPS_REP_START_OPTION );

	IicInstance->IsSend = 1;

	/* Send the Data */
	Status = XIicPs_MasterSendPolled(IicInstance, &outbuf,
			1, dev_addr);
	if (Status != XST_SUCCESS) {
		XFsbl_Printf(DEBUG_GENERAL, "Error in XIicPs_MasterSendPolled\r\n");
		return Status;
	}

	/*
	 * Disable repeated start option.
	 * This call will give an indication to the driver.
	 * The hold bit is actually reset when the following transfer ends.
	 */
	XIicPs_ClearOptions(IicInstance, XIICPS_REP_START_OPTION);
	IicInstance->IsSend = 0;

	/* Receive the Data */
	Status = XIicPs_MasterRecvPolled(IicInstance, data,
					  1, dev_addr);
	if (Status != XST_SUCCESS) {
		XFsbl_Printf(DEBUG_GENERAL, "Error in XIicPs_MasterRecvPolled\r\n");
		return Status;
	}

	/* Wait until bus is idle to start another transfer */
	while (XIicPs_BusIsBusy(IicInstance));

	return XST_SUCCESS;
}

s32 sys_i2c_reg_write(XIicPs* IicInstance, const u16 dev_addr, const u8 reg_addr, const u8 data)
{
	u8 outbuf[2];
	s32 Status;

	/* The first byte indicates which register we'll write */
	outbuf[0]			= reg_addr & 0xFF;
	outbuf[1]			= data;
	IicInstance->IsSend = 1;

	/* Send the Data */
	Status = XIicPs_MasterSendPolled(IicInstance, outbuf,
					  2, dev_addr);
	if (Status != XST_SUCCESS) {
		XFsbl_Printf(DEBUG_GENERAL, "Error in XIicPs_MasterSendPolled\r\n");
		return Status;
	}

	/* Wait until bus is idle to start another transfer */
	if (!(IicInstance->IsRepeatedStart)) {
		while (XIicPs_BusIsBusy(IicInstance));
	}

	return XST_SUCCESS;
}

s32 sys_i2c_set_write(XIicPs* IicInstance, unsigned char dev_addr, tsClkGenRegValue *set){
	s32 status;

	/* Perform writes till table end */
	for(int i=0; set[i].reg != TABLE_END_SENTINAL; i++){
		status = sys_i2c_reg_write(IicInstance, dev_addr, set[i].reg, set[i].val);
		if (status != XST_SUCCESS) {
			return XST_FAILURE;
		}
		usleep(10);
	}
	/* Check writes till table end */
	for(int i=0; set[i].reg != TABLE_END_SENTINAL; i++){
		u8 data;
		sys_i2c_reg_read(IicInstance, dev_addr, set[i].reg, &data);
		if (status != XST_SUCCESS) {
			return status;
		}
		if (data != set[i].val) {
			XFsbl_Printf(DEBUG_PRINT_ALWAYS, "Error loading register set, wrote 0x%x to register 0x%x, but read back 0x%x\r\n", set[i].val, set[i].reg, data);
			return XST_FAILURE;
		}
	}
	return XST_SUCCESS;
}



