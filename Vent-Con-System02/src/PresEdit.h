/*
 * DecimalEdit.h
 *
 *  Created on: 11.02.2016
 *      Author: georgrokita
 */

#ifndef PRESEDIT_H_
#define PRESEDIT_H_

#include "PropertyEdit.h"
#include "ModbusMaster.h"
#include <string>
#include "LiquidCrystal.h"
#include "BarGraph.h"


void Sleep(int ms);


class PresEdit:public PropertyEdit {
public:
	PresEdit(LiquidCrystal& lcd_,ModbusMaster& node_, std::string editTitle, float _upperL = 100,float _lowerL =0);
	virtual ~PresEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	void display();
	float getValue();
	float getTol();
	float getDesValue();
	void setValue(float value);
	bool setFrequency(ModbusMaster& node, uint16_t freq){};
	void setSensValue(float val);
private:
	void save();
	void displayEditValue();
	LiquidCrystal& lcd;
	ModbusMaster& node;
	std::string title;
	float value;
	float desVal;
	float tol;
	float edit;
	bool focus;
	float upperL;
	float lowerL;
};

#endif /* PresEdit_H_ */
