/*
 * PropertyEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef PROPERTYEDIT_H_
#define PROPERTYEDIT_H_

#include "ModbusMaster.h"
#include <string>
#include "LiquidCrystal.h"

extern volatile bool TempSensFlag;
extern volatile bool PresSensFlag;

class PropertyEdit {
public:
	PropertyEdit(){};
	//PropertyEdit();
	virtual ~PropertyEdit() {};
	virtual void increment() = 0;
	virtual void decrement() = 0;
	virtual void accept() = 0;
	virtual void cancel() = 0;
	virtual float getValue()=0;
	virtual float getDesValue()=0;
	virtual float getTol() =0;
	virtual void setFocus(bool focus) = 0;
	virtual void display() = 0;
	virtual void setValue(float value) =0;
	virtual void setSensValue(float val) =0;
	virtual bool setFrequency(ModbusMaster& node, uint16_t freq) =0;
	virtual void save()=0;
};

#endif /* PROPERTYEDIT_H_ */
