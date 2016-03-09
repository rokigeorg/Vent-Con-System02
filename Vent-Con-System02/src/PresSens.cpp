/*
 * PresSens.cpp
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#include "PresSens.h"
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif


PresSens::PresSens(int deviceNumber, uint32_t speed) {
	if(deviceNumber == 0) {
		device = LPC_I2C0;
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | I2C_MODE);
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | I2C_MODE);
		Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
		Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
	}
	else {
		// currently we support only I2C number 0
	}
	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	   do this */
	Chip_I2C_Init(device);

	/* Setup clock rate for I2C */
	Chip_I2C_SetClockDiv(device, I2C_CLK_DIVIDER);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(device, speed);

	/* Enable Master Mode */
	Chip_I2CM_Enable(device);
}



bool PresSens::transaction(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize) {
	I2CM_XFER_T  i2cmXferRec;
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(device, &i2cmXferRec);

	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		return true;
	}
	else {
		return false;
	}
}

float PresSens::readValues()
{
	uint8_t pressureData[3];
	uint8_t readPressureCmd = 0xF1;
	int16_t pressure = 0;


	if (transaction(0x40, &readPressureCmd, 1, pressureData, 3)) {
		/* Output temperature. */
		pressure = (pressureData[0] << 8) | pressureData[1];
		DEBUGOUT("Pressure read over I2C is %.1f Pa\r\n",	pressure/240.0);
	}
	else {
		DEBUGOUT("Error reading pressure.\r\n");
	}
	return pressure/240.0;
}



