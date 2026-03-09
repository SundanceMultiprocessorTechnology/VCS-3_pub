/**
 ********************************************************************************
 * @file    vcs3_board_init.c
 * @author  tim
 * @date    Sep 23, 2022
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "vcs3_board_init.h"
#include "xfsbl_hw.h"
#include "xfsbl_debug.h"
#include "sys_i2c.h"
#include "clk_gen_cfg.h"
//#include "opteran_ext_version.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
// Define the USB Phy reset GPIO
#define GPIO_DATA_2    ( ( GPIO_BASEADDR ) + 0x00000048U )
#define GPIO_DIRM_2    ( ( GPIO_BASEADDR ) + 0x00000284U )
#define GPIO_OEN_2     ( ( GPIO_BASEADDR ) + 0x00000288U )

#define GPIO_MIO77_MASK  0x2000000
#define DELAY_5_US		 0x5U
#define DELAY_USB_US	 200000U
#define DELAY_RESET_US	 100000U


#define IIC_DEVICE_ID		0 //XPAR_XIICPS_0_DEVICE_ID
#define CLK_GEN_I2C_ADDR    0x6a

#define SYS_CONTROLLER_I2C_ADDR 0x25
#define SC_VERSION_REG 			0x15

// registers for the usb c logic
#define USB_LOGIC_I2C_ADDR  0x1D

#define VERSION_REG         0x01
#define CONTROL_REG         0x02
#define ISR_STATUS_REG      0x03
#define CC_STATUS           0x04
#define CON_DET             0x09

#define CTRL_REG_RP_80UA        0x00
#define CTRL_REG_RP_180UA       0x08
#define CTRL_REG_RP_330UA       0x10
#define CTRL_REG_PORT_UFP       0x00
#define CTRL_REG_RORT_DFP       0x02
#define CTRL_REG_PORT_DRP       0x04
#define CTRL_REG_IRQ_MASK       0x01
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

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

static void vUsbLogicInit(XIicPs* IicInstance){
	s32 i2cState;
	u8 u8VendVer;
	u8 u8ControlState;
	u8 u8IsrStatus;
	u8 u8CCStatus;
	u8 u8ConDet;

	u8ConDet = 0; // Disable connection detect
	i2cState = sys_i2c_reg_write( IicInstance, USB_LOGIC_I2C_ADDR, CON_DET, u8ConDet);
	usleep(DELAY_5_US);

	u8ControlState = CTRL_REG_RP_330UA | CTRL_REG_PORT_DRP;
	i2cState = sys_i2c_reg_write( IicInstance, USB_LOGIC_I2C_ADDR, CONTROL_REG, u8ControlState);

	usleep(DELAY_5_US);
	u8ControlState = 0;
	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, CONTROL_REG, &u8ControlState);
	if( (u8ControlState != (CTRL_REG_RP_330UA | CTRL_REG_PORT_DRP) ) || (i2cState != XST_SUCCESS)){
		XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - USB Logic init error\r\n");
	}


	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, VERSION_REG, &u8VendVer);
	usleep(DELAY_5_US);
	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, CONTROL_REG, &u8ControlState);
	usleep(DELAY_5_US);
	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, ISR_STATUS_REG, &u8IsrStatus);
	usleep(DELAY_5_US);
	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, CC_STATUS, &u8CCStatus);
	usleep(DELAY_5_US);
	i2cState = sys_i2c_reg_read( IicInstance, USB_LOGIC_I2C_ADDR, CON_DET, &u8ConDet);
	usleep(DELAY_5_US);

	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - VERSION_REG %x\r\n",u8VendVer );
	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - CONTROL_REG %x\r\n",u8ControlState );
	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - ISR_STATUS_REG %x\r\n",u8IsrStatus );
	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - CC_STATUS %x\r\n",u8CCStatus );
	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - CON_DET %x\r\n",u8ConDet );

}

static void vClkGenInit(XIicPs* IicInstance){


	sys_i2c_set_write(IicInstance,CLK_GEN_I2C_ADDR, clock1_26MHz_USB_26MHz_regs );

	XFsbl_Printf(DEBUG_GENERAL,"SUN FSBL - Clock generator init complete\r\n");
}

static void vSysContVer (XIicPs* IicInstance){
	s32 i2cState;
	u8 u8SysControllVer=0xFF;
	i2cState = sys_i2c_reg_read( IicInstance, SYS_CONTROLLER_I2C_ADDR, SC_VERSION_REG, &u8SysControllVer);
	if(i2cState != XST_SUCCESS){
			XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - USB Logic init error\r\n");
	}

	u8 versionMinor = 0x0f & (u8SysControllVer);
	u8 versionMajor = 0x0f & (u8SysControllVer >> 4);

	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"System Controller - VERSION v%u.%u\r\n", versionMajor, versionMinor );
}

static void vUSBReset(void){

	u32 RegVal;

	//USB Phy RSTn on MIO77
	/* Set MIO77 direction as output */
	XFsbl_Out32(GPIO_DIRM_2, GPIO_MIO77_MASK);

	/* Set MIO77 output enable */
	XFsbl_Out32(GPIO_OEN_2, GPIO_MIO77_MASK);

	/* Set MIO77 to HIGH */
	RegVal = XFsbl_In32(GPIO_DATA_2) | GPIO_MIO77_MASK;
	XFsbl_Out32(GPIO_DATA_2, RegVal);
 
	(void)usleep(DELAY_USB_US);

	/* Set MIO77 to LOW */
	RegVal = XFsbl_In32(GPIO_DATA_2) & ~(GPIO_MIO77_MASK);
	XFsbl_Out32(GPIO_DATA_2, RegVal);

	(void)usleep(DELAY_USB_US);

	/* Set MIO77 to HIGH */
	RegVal = XFsbl_In32(GPIO_DATA_2) | GPIO_MIO77_MASK;
	XFsbl_Out32(GPIO_DATA_2, RegVal);

	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - Starting reset propogation delay.\r\n");
	(void)usleep(DELAY_RESET_US);
	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - Reset USB Phy is complete.\r\n");
}



static void vPrintFullResetCode(void){
	u32 Val = XFsbl_In32(CRL_APB_RESET_REASON);
	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - reset register code 0x%x \r\n", Val);
}


/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void VCS3_vBoardInit(void)
{
	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - Board Init Start \r\n");

	vPrintFullResetCode();

	XIicPs i2cInstance;
	s32 status;
	status = sys_i2c_init(&i2cInstance, IIC_DEVICE_ID);
	if(status != XFSBL_SUCCESS)
	{
		XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL -i2c Init Failure\r\n");
	}
	vSysContVer(&i2cInstance);
	vUsbLogicInit(&i2cInstance);
	vClkGenInit(&i2cInstance);
	vUSBReset();
	XFsbl_Printf(DEBUG_PRINT_ALWAYS,"SUN FSBL - Board Init Complete \r\n");

}
