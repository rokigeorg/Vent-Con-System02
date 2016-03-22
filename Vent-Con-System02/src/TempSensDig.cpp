/*
 * TempSensAndhumid.cpp
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#include "TempSensDig.h"
#include <iostream>


#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif



TempSensDig::TempSensDig(int deviceNumber, uint32_t speed) {
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


uint32_t TempSensDig::Chip_I2CM_XferBlocking(LPC_I2C_T *pI2C, I2CM_XFER_T *xfer)
{
	uint32_t ret = 0;
	/* start transfer */
	Chip_I2CM_Xfer(pI2C, xfer);
	xfer->txSz = 0;
	while (ret == 0) {
		/* wait for status change interrupt */
		while (!Chip_I2CM_IsMasterPending(pI2C)) {}
		/* call state change handler */
		ret = Chip_I2CM_XferHandler(pI2C, xfer);
	}
	//Sleep(40);
	return ret;
}


bool TempSensDig::transaction(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize) {
	I2CM_XFER_T  i2cmXferRec;
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(device, &i2cmXferRec);
	//Sleep(40);
	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		return true;
	}
	else {
		return false;
	}
}

float TempSensDig::readValues()
{

	uint8_t HumiTempData[4];
	uint8_t readPressureCmd = 0xF1;
	float Temprature = 0.0;


	if (transaction(0x27, &readPressureCmd, 1, HumiTempData, 4)) {

		Temprature = ((float)((((HumiTempData[2] << 8) | HumiTempData[3]))>>2)*165/16382) - 41;

		//DEBUGOUT("Humidity read over I2C is %.3f RaH Temp one is %.3f\r\n", Hiumidity, Temprature, "\n");

	}
	else {
		DEBUGOUT("Error reading pressure.\r\n");
	}

	return Temprature;
}



