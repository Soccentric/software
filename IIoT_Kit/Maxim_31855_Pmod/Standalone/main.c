/*
 * Copyright (c) 2016 Avnet, Inc.  All rights reserved.
 *
 * Avnet, Inc.
 * Avnet IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, Avnet IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * Avnet EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/*
 * SPI_TempSensor_MAX31855: simple test application to drive SPI comands
 * to MAX31855 PMOD placed on MicroZed Arduino Carrier PMOD connector J3 top row
 *
  */

#include "PmodUtilities.h"

int main()
{

	int i;
	// TxBuffer is not used to communicate with the MAX31855 but it is still necessary
	//      for the XSPI utilities to function
	u32 TxBuffer[6] = {0,0,0,0,0,0};
	u32 RxBuffer[6];
	int nTemporaryValue=0;
	int nTemporaryValue2=0;
	float MAX31855_internal_temp=0.0f;
	float MAX31855_thermocouple_temp=0.0f;

	print("     - MicroZed Arduino Carrier Card\n\r");
	print("     -    PMOD J3 MAX31855 Temp Sensor\n\r");

	//Set up the ARM PS
	init_platform();
	print("     - Init Platform Done\n\r");

	// Set up the AXI SPI Controller to operate with the MAX31855
	XSpi_MAX31855_INIT(SPI_BASEADDR);

	print("     - MAX31855 Init Done\n\r");

	while(1)
	{
		fflush(stdin);

		// Initialize RxBuffer with all 1's
		for(i = 0; i < 5; i++) RxBuffer[i] = 0xFFFFFFFF; // init RxBuffer with 0xFFs
		// Execute 5-byte read transaction.
		XSpi_LowLevelExecute(SPI_BASEADDR, SPI_CHANNEL_SEL_0, TxBuffer, RxBuffer, 4);
		for(i=0;i<4;i++)
		{
			xil_printf("Byte %d = 0x%x\n\r",i,RxBuffer[i]);
		}
		// Check for various error codes
		if(
				((RxBuffer[3] & 0x01)==0x01) ||		// Open Circuit
				((RxBuffer[3] & 0x02)==0x02) ||		// Short to GND
				((RxBuffer[3] & 0x04)==0x04) ||		// Short to VCC
				((RxBuffer[1] & 0x01)==0x01)
				)		// Fault
			{
			xil_printf("Error Detected\n\r");
			}
		else
		{
			// Internal Temp
			nTemporaryValue = RxBuffer[2];  			// bits 11..4
			nTemporaryValue = nTemporaryValue << 4;		// shift left to make room for bits 3..0
			nTemporaryValue2 = RxBuffer[3];				// bits 3..0 in the most significant spots
			nTemporaryValue2 = nTemporaryValue2 >> 4;	// shift right to get rid of extra bits and position
			nTemporaryValue |= nTemporaryValue2;		// Combine to get bits 11..0
			if((RxBuffer[2] & 0x80)==0x80)				// Check the sign bit and sign-extend if need be
				nTemporaryValue |= 0xFFFFF800;
			MAX31855_internal_temp = (float)nTemporaryValue / 16.0f;

			// Thermocouple Temp
			nTemporaryValue = RxBuffer[0];  			// bits 13..6
			nTemporaryValue = nTemporaryValue << 6;		// shift left to make room for bits 5..0
			nTemporaryValue2 = RxBuffer[1];				// bits 5..0 in the most significant spots
			nTemporaryValue2 = nTemporaryValue2 >> 2;	// shift right to get rid of extra bits and position
			nTemporaryValue |= nTemporaryValue2;		// Combine to get bits 13..0
			if((RxBuffer[0] & 0x80)==0x80)				// Check the sign bit and sign-extend if need be
				nTemporaryValue |= 0xFFFFE000;
			MAX31855_thermocouple_temp = (float)nTemporaryValue / 4.0f;

			printf("MAX31855 Internal Temp     = %.1f deg C\n\r",MAX31855_internal_temp);
			printf("MAX31855 Thermocouple Temp = %.1f deg C\n\r",MAX31855_thermocouple_temp);
		}
		sleep(3);
	}
    return 0;
}
