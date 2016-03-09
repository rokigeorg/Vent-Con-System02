/*
 * TempSens.h
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#ifndef TEMPSENS_H_
#define TEMPSENS_H_
#include "SensorGeneral.h"
#include "chip.h"
#include <string>

class TempSens : public SensorGeneral {


public:
	TempSens();
	virtual ~TempSens();
	float readValues(); //Chip_ADC_StartSequencer(LPC_ADC0, ADC_SEQA_IDX);
};




#endif /* TEMPSENS_H_ */
