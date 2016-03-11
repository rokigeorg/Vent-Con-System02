/*
 * AutoEdit.h
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#ifndef AUTOEDIT_H_
#define AUTOEDIT_H_

#include "PropertyEdit.h"
#include "ModbusMaster.h"
#include <string>
#include "LiquidCrystal.h"
#include "BarGraph.h"

void Sleep(int ms);


class AutoEdit:public PropertyEdit {
public:
	AutoEdit(LiquidCrystal& lcd_,ModbusMaster& node_, std::string editTitle, float _upperL = 100,float _lowerL =0, float _desvalue = 22);
	virtual ~AutoEdit();
	void save();
	void increment(){};
	void decrement(){};
	void accept(){};
	void cancel(){};
	void setFocus(bool focus);
	void display();
	float getValue();
	float getTol();
	void setValue(float value);
	void setSensValue(float val);
	float getDesValue();
	bool setFrequency(ModbusMaster& node, uint16_t freq){};
	void setTitle(std::string tit);
	std::string getTitle();
	void setStartUpTitle(std::string openerStr);
private:
	void displayEditValue();
	void drawSpinningVentChar();
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
	bool ventCharFlag;

};

#endif /* AUTOEDIT_H_ */
