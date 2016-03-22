/*
 * TempSensAndhumid.h
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#ifndef TEMPSENSDIG_H_
#define TEMPSENSDIG_H_
#include "SensorGeneral.h"
#include "chip.h"

class TempSensDig : public SensorGeneral {

public:
	TempSensDig(int deviceNumber, uint32_t speed);
	virtual ~TempSensDig() {};
	float readValues();
	bool transaction(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize);
	//virtual void init() = 0;

private:
	LPC_I2C_T *device;
	static const unsigned int I2C_CLK_DIVIDER = 40;
	static const unsigned int I2C_MODE = 0;
	uint32_t Chip_I2CM_XferBlocking(LPC_I2C_T *pI2C, I2CM_XFER_T *xfer);
};




#endif /* TEMPSENSDIG_H_ */
