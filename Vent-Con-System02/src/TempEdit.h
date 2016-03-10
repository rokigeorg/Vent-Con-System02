/*
 * DecimalEdit.h
 *
 *  Created on: 11.02.2016
 *      Author: georgrokita
 */

#ifndef TEMPEDIT_H_
#define TEMPEDIT_H_

#include "PropertyEdit.h"
#include "ModbusMaster.h"
#include <string>
#include "LiquidCrystal.h"
#include "BarGraph.h"

void Sleep(int ms);


class TempEdit:public PropertyEdit {
public:
	TempEdit(LiquidCrystal& lcd_,ModbusMaster& node_, std::string editTitle, float _upperL = 100,float _lowerL =0);
	virtual ~TempEdit();
	void save();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	void display();
	float getValue();
	float getTol();
	void setValue(float value);
	void setSensValue(float val);
	float getDesValue();
	bool setFrequency(ModbusMaster& node, uint16_t freq){};
private:
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

#endif /* TEMPEDIT_H_ */
