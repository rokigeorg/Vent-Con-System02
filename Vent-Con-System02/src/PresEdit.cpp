/*
 * PresEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#include "PresEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




PresEdit::PresEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =10;
	tol = 2;
	focus =false;
	upperL = _upperL;
	lowerL = _lowerL;
	desVal=10;
}


PresEdit::~PresEdit() {
	// TODO Auto-generated destructor stub
}


void PresEdit::save() {
	// set current value to be same as edit value
	desVal = edit;
	//setFrequency(node, value);

	// todo: save current value for example to EEPROM for permanent storage
}
void PresEdit::setSensValue(float val){
	value = val;
	//display();
}

void PresEdit::setValue(float val){
	edit = val;
	save();
}


void PresEdit::increment() {
	edit++;
	if(edit >=upperL){
		edit = upperL;
	}
}

void PresEdit::decrement() {
	edit--;
	if(edit <=lowerL){
		edit = lowerL;
	}
}




void PresEdit::accept() {
	save();
	TempSensFlag = false;
	PresSensFlag = true;
	HumidSensFlag= false;
	AutoFlag = false;
}

void PresEdit::cancel() {
	edit = desVal;
}


void PresEdit::setFocus(bool focus) {
	this->focus = focus;
}

void PresEdit::display() {
	BarGraph barG(lcd, 50);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.Print(title);
	lcd.setCursor(0,1);
	char s[16];
	if(focus) {
		snprintf(s, 16, "[%4.2f]P", edit);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(edit*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	else {
		snprintf(s, 16, " %4.2f P", value);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(value*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	lcd.Print(s);
	//delete the char
	//delete s;
}


float PresEdit::getValue() {
	return value;
}

float PresEdit::getDesValue() {
	return desVal;
}

float PresEdit::getTol() {
	return tol;
}

