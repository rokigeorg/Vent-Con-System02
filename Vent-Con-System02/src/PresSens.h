/*
 * PresSens.h
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#ifndef PRESSENS_H_
#define PRESSENS_H_
#include "SensorGeneral.h"
#include "chip.h"

class PresSens : public SensorGeneral {

public:
	PresSens(int deviceNumber, uint32_t speed);
	virtual ~PresSens() {};
	float readValues();
	bool transaction(uint8_t devAddr, uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize);
	//virtual void init() = 0;

private:
	LPC_I2C_T *device;
	static const unsigned int I2C_CLK_DIVIDER = 40;
	static const unsigned int I2C_MODE = 0;
};




#endif /* PRESSENS_H_ */
