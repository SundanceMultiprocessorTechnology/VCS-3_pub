/**
 ********************************************************************************
 * @file    sys_i2c.h
 * @author  tim
 * @date    Jun 1, 2022
 * @brief   Abstraction of the i2c driver for simple 1x u8 address - 1x u8 data, register operations
 ********************************************************************************
 */

#ifndef SRC_DRIVERS_SYS_I2C_H_
#define SRC_DRIVERS_SYS_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

#include "xiicps.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

#define TABLE_END_SENTINAL       0xFF

/************************************
 * TYPEDEFS
 ************************************/

// atomar register element
typedef struct  {
     u8 reg;
     u8 val;
}tsClkGenRegValue;

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
s32 sys_i2c_init(XIicPs* IicInstance, u16 deviceId);
s32 sys_i2c_reg_read(XIicPs* IicInstance, const u16 dev_addr, const u8 reg_addr, u8* data);
s32 sys_i2c_reg_write(XIicPs* IicInstance, const u16 dev_addr, const u8 reg_addr, const u8 data);
s32 sys_i2c_set_write(XIicPs* IicInstance, unsigned char dev_addr, tsClkGenRegValue *set);
#ifdef __cplusplus
}
#endif

#endif  // SRC_CAMERA_CAM_I2C_H_
