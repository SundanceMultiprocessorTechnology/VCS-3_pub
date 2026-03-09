
/*
-- Company: Sundance Multiprocessor Technology Ltd
-- 1. waiting >=10ms every powerup/reset for the clock signal output active
-- 2.SD pin (CLKGEN_OE) high enable the clock signal outputs and SD pin low disable the clock signal outputs
 */

#ifndef RENASAS_5P49V6901_H_
#define RENASAS_5P49V6901_H_

#include "sys_i2c.h"

//-----------------------------------------------------------------------------------------

static tsClkGenRegValue clock1_26MHz_USB_26MHz_regs[] = {

    {0x10, 0x8A},   // Primary Source and Shutdown: [0]SH=0,[1]SP=SD pin active high,[3]en_double_XTAL_freq=1, [5:4]cnf_vreg_sync 1.2V
    {0x11, 0x0C},   // VCO Band:vco_band[4:0]=Select vco_band from I2C, cnf_vreg_xtal[7:6] =1.2V
    {0x12, 0x01},   //  Crystal X1 Load Capacitor:
    {0x13, 0x00},   // Clock source selection: [1:0]PRIMSRC=XIN/REF
    {0x14, 0x00},   // XTAL voltage: [7:4]1.2V
    {0x15, 0x03},   //Reference Divider: Settings(Dec)  Gain
    {0x16, 0x8C},   //VCO Control Register and Pre-Divider: = settingFeedback Fractional Divider
    {0x17, 0x03},   // Feedback Integer Divider
    {0x18, 0x40},   //  Feedback Integer Divider Bits:
    {0x19, 0x00},   // Feedback Fractional Divider
    {0x1A, 0x00},   // Feedback Fractional Divider Bits
    {0x1B, 0x00},   // Feedback Fractional Divider
    {0x1C, 0x9F},   // VCO
    {0x1D, 0xFF},   // Charge Pump:
    {0x1E, 0xF0},   // RC Control
    {0x1F, 0x80},   // RC Control

    {0x21, 0x81},   // Output Divider 1 Control Register Settings
    {0x22, 0x00},   // Output Divider 1 Fractional Settings
    {0x23, 0x00},   // Output Divider 1 Fractional Settings
    {0x24, 0x00},   // FOD1 Offsets
    {0x25, 0x00},   // FOD1 Offsets
    {0x26, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x27, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x28, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x29, 0x00},   // Output Divider 1 Spread Modulation Rate Configuration
    {0x2A, 0x04},   // Output Divider 1 Spread Modulation Rate Configuration
    {0x2B, 0x00},   // Output Divider 1 Skew Integer Part
    {0x2C, 0x00},   // Output Divider 1 Skew Integer Part
    {0x2D, 0x03},   // Output Divider 1 Integer Part
    {0x2E, 0x20},   // Output Divider 1 Integer Part
    {0x2F, 0x00},   //  Output Divider 1 Skew Fractional Part

    {0x51, 0x81},   // Output Divider 4 Control Register Settings
    {0x52, 0x00},   // Output Divider 4 Fractional Settings
    {0x53, 0x00},   // Output Divider 4 Fractional Settings
    {0x54, 0x00},   //FOD4 Offsets
    {0x55, 0x00},   //FOD4 Offsets
    {0x56, 0x00},   //Output Divider 4 Step Spread Configuration
    {0x57, 0x00},   //Output Divider 4 Step Spread Configuration
    {0x58, 0x00},   //Output Divider 4 Step Spread Configuration
    {0x59, 0x00},   //Output Divider 4 Spread Modulation Rate Configuration
    {0x5A, 0x04},   //Output Divider 4 Spread Modulation Rate Configuration
    {0x5B, 0x00},   //Output Divider 4 Skew Integer Part
    {0x5C, 0x00},   //Output Divider 4 Skew Integer Part
    {0x5D, 0x03},   //Output Divider 4 Integer Part
    {0x5E, 0x20},   //Output Divider 4 Integer Part
    {0x5F, 0x00},   //Output Divider 4 Skew Fractional Part

    {0x60, 0x63}, //Clock1 Output Configuration: clk1_pwr_sel[4:3]=00 1.8V
    {0x61, 0x01}, // Clock1 Output Configuration: en_clkbuf1[0]=1 enable clock output
    {0x62, 0x63}, //Clock2 Output Configuration: clk2_pwr_sel[4:3]=00 1.8V
    {0x63, 0x00}, //Clock2 Output Configuration: en_clkbuf2[0]=0 disable clock output
    {0x64, 0x63}, // Clock3 Output Configuration: clk3_pwr_sel[4:3]=00 1.8V
    {0x65, 0x00}, // Clock3 Output Configuration: en_clkbuf3[0]=0 disable clock output
    {0x66, 0x23}, //Clock4 Output Configuration: clk4_pwr_sel[4:3]=00 1.8V
    {0x67, 0x01}, //Clock4 Output Configuration: en_clkbuf4[0]=1 enable clock output

    {0x68, 0xCC}, //CLK_OE/Shutdown: clk0_pwr_sel[1:0]=0 1.8V, CLK4_OE[3] =1 Active, CLK1_OE[6] =1 Active, CLK0_OE[7]=1 active
    {0x69, 0xCC}, //CLK_OS/Shutdown: otp_pwr_sel[1:0]=00 3.3v,

    {TABLE_END_SENTINAL, 0x00}
};

static tsClkGenRegValue clock1_26MHz_USB_60MHz_regs[] = {

    {0x10, 0x8A},   // Primary Source and Shutdown: [0]SH=0,[1]SP=SD pin active high,[3]en_double_XTAL_freq=1, [5:4]cnf_vreg_sync 1.2V
    {0x11, 0x0C},   // VCO Band:vco_band[4:0]=Select vco_band from I2C, cnf_vreg_xtal[7:6] =1.2V
    {0x12, 0x01},   //  Crystal X1 Load Capacitor:
    {0x13, 0x00},   // Clock source selection: [1:0]PRIMSRC=XIN/REF
    {0x14, 0x00},   // XTAL voltage: [7:4]1.2V
    {0x15, 0x03},   //Reference Divider: Settings(Dec)  Gain
    {0x16, 0x8C},   //VCO Control Register and Pre-Divider: = settingFeedback Fractional Divider
    {0x17, 0x03},   // Feedback Integer Divider
    {0x18, 0x40},   //  Feedback Integer Divider Bits:
    {0x19, 0x00},   // Feedback Fractional Divider
    {0x1A, 0x00},   // Feedback Fractional Divider Bits
    {0x1B, 0x00},   // Feedback Fractional Divider
    {0x1C, 0x9F},   // VCO
    {0x1D, 0xFF},   // Charge Pump:
    {0x1E, 0xF0},   // RC Control
    {0x1F, 0x80},   // RC Control

    {0x21, 0x81},   // Output Divider 1 Control Register Settings
    {0x22, 0x00},   // Output Divider 1 Fractional Settings
    {0x23, 0x00},   // Output Divider 1 Fractional Settings
    {0x24, 0x00},   // FOD1 Offsets
    {0x25, 0x00},   // FOD1 Offsets
    {0x26, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x27, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x28, 0x00},   // Output Divider 1 Step Spread Configuration
    {0x29, 0x00},   // Output Divider 1 Spread Modulation Rate Configuration
    {0x2A, 0x04},   // Output Divider 1 Spread Modulation Rate Configuration
    {0x2B, 0x00},   // Output Divider 1 Skew Integer Part
    {0x2C, 0x00},   // Output Divider 1 Skew Integer Part
    {0x2D, 0x03},   // Output Divider 1 Integer Part
    {0x2E, 0x20},   // Output Divider 1 Integer Part
    {0x2F, 0x00},   //  Output Divider 1 Skew Fractional Part

    {0x51, 0x81},   // Output Divider 4 Control Register Settings
    {0x52, 0x02},   // FOD4 Offsets
    {0x53, 0xAA},   //FOD4 Offsets
    {0x54, 0xAA},   //FOD4 Offsets
    {0x55, 0xA8},   //FOD4 Offsets
    {0x56, 0x00},   //FOD4 Offsets
    {0x57, 0x00},   //FOD4 Offsets
    {0x58, 0x00},   //FOD4 Offsets
    {0x59, 0x00},   //FOD4 Offsets
    {0x5A, 0x04},   //FOD4 Offsets
    {0x5B, 0x00},   //FOD4 Offsets
    {0x5C, 0x00},   //FOD4 Offsets
    {0x5D, 0x01},   //Output Divider 4 Integer Part
    {0x5E, 0x50},   //Output Divider 4 Integer Part
    {0x5F, 0x00},   //Output Divider 4 Skew Fractional Part

    {0x60, 0x63}, //Clock1 Output Configuration
    {0x61, 0x01}, // Clock1 Output Configuration
    {0x62, 0x63}, //Clock2 Output Configuration
    {0x63, 0x00}, //Clock2 Output Configuration
    {0x64, 0x63}, // Clock3 Output Configuration
    {0x65, 0x00}, // Clock3 Output Configuration
    {0x66, 0x23}, //Clock4 Output Configuration
    {0x67, 0x01}, //Clock4 Output Configuration

    {0x68, 0xCC}, //CLK_OE/Shutdown
    {0x69, 0xCC}, //CLK_OS/Shutdown

    {TABLE_END_SENTINAL, 0x00},
};




#endif
