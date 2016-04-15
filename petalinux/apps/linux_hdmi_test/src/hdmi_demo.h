//----------------------------------------------------------------------------
//      _____
//     *     *
//    *____   *____
//   * *===*   *==*
//  *___*===*___**  AVNET
//       *======*
//        *====*
//----------------------------------------------------------------------------
//
// This design is the property of Avnet.  Publication of this
// design is not authorized without written consent from Avnet.
//
// Please direct any questions to the PicoZed community support forum:
//    http://www.picozed.org/forum
//
// Product information is available at:
//    http://www.picozed.org/product/picozed
//
// Disclaimer:
//    Avnet, Inc. makes no warranty for the use of this code or design.
//    This code is provided  "As Is". Avnet, Inc assumes no responsibility for
//    any errors, which may appear in this code, nor does it make a commitment
//    to update the information contained herein. Avnet, Inc specifically
//    disclaims any implied warranties of fitness for a particular purpose.
//                     Copyright(c) 2015 Avnet, Inc.
//                             All rights reserved.
//
//----------------------------------------------------------------------------
//
// Create Date:         Nov 30, 2015
// Design Name:         PicoZed FMC2 Carrier HDMI Test
// Module Name:         hdmi_demo.h
// Project Name:        PicoZed FMC2 Carrier HDMI Test
// Target Devices:      Xilinx Zynq-7000
// Hardware Boards:     PicoZed, PicoZed FMC2 Carrier
//
// Tool versions:       Xilinx Vivado 2015.2
//
// Description:         HDMI test for PicoZed FMC2 Carrier
//
// Dependencies:
//
// Revision:            Nov 30, 2015: 1.00 Initial version
//
//----------------------------------------------------------------------------

#ifndef __HDMI_DEMO_H__
#define __HDMI_DEMO_H__

#include "platform.h"
#include "types.h"

// I2C related.
#include "zed_iic.h"

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#define GPIO_DEVICE_ID  	XPAR_XGPIOPS_0_DEVICE_ID

#define HDMI_SLAVE_ADDRESS                    0x39
#define HDMI_CHIP_ID                          0x7511

// This structure contains the context for the
// Zed Real Time Clock Demonstration
struct struct_iic_eeprom_demo_t
{
   int32u bVerbose;

   ////////////////////////////////
   // RTC I2C related context
   ////////////////////////////////
   int32u uBaseAddr_IIC_RTC;

   zed_iic_t eeprom_iic;
};
typedef struct struct_iic_eeprom_demo_t iic_eeprom_demo_t;

int hdmi_demo_init(iic_eeprom_demo_t *pDemo);
int validate_hdmi(iic_eeprom_demo_t *pDemo);

#endif // __HDMI_DEMO_H__
