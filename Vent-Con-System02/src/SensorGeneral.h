/*
 * SensorGeneral.h
 *
 *  Created on: 2.3.2016
 *      Author: abdullai
 */

#include <stdint.h>
#ifndef SENSORGENERAL_H_
#define SENSORGENERAL_H_


class SensorGeneral {
	//extern volatile bool TempFlag = false;
	//extern volatile bool PresFlag = false;

public:
	SensorGeneral(){};
	virtual ~SensorGeneral() {};
	virtual float readValues() = 0;
	//virtual void init() = 0;
};



#endif /* SENSORGENERAL_H_ */
