/*
 * TempSensAndhumid.h
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#ifndef TEMPSENSANDHUMID_H_
#define TEMPSENSANDHUMID_H_
#include "SensorGeneral.h"
#include "chip.h"

class TempSensAndhumid : public SensorGeneral {

public:
	TempSensAndhumid(int deviceNumber, uint32_t speed);
	virtual ~TempSensAndhumid() {};
	float readValues();
	bool transaction(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize);
	//virtual void init() = 0;

private:
	LPC_I2C_T *device;
	static const unsigned int I2C_CLK_DIVIDER = 40;
	static const unsigned int I2C_MODE = 0;
	uint32_t Chip_I2CM_XferBlocking(LPC_I2C_T *pI2C, I2CM_XFER_T *xfer);
};




#endif /* TEMPSENSANDHUMID_H_ */
