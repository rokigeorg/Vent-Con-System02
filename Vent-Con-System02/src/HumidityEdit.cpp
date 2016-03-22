/*
 * HumidityEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#include "HumidityEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




HumidityEdit::HumidityEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =32;
	focus =false;
	tol = 3;
	upperL = _upperL;
	lowerL = _lowerL;
	desVal = 32;
}

HumidityEdit::~HumidityEdit() {
	// TODO Auto-generated destructor stub
}


void HumidityEdit::save() {
	// set current value to be same as edit value
	desVal = edit;

}


//this sets the desVal to the user input
void HumidityEdit::setValue(float val){
	edit = val;
	save();
}

//this sets the read value from the sens to the obj
void HumidityEdit::setSensValue(float val){
	value = val;
	//display();

}


void HumidityEdit::increment() {
	edit = edit + 0.5;
	if(edit >=upperL){
		edit = upperL;
	}
}

void HumidityEdit::decrement() {
	edit = edit - 0.5;
	if(edit <=lowerL){
		edit = lowerL;
	}
}


void HumidityEdit::accept() {
	save();
	HumidSensFlag= true;
	TempSensFlag = false;
	PresSensFlag = false;
	AutoFlag = false;
}

void HumidityEdit::cancel() {
	edit = desVal;
}


void HumidityEdit::setFocus(bool focus) {
	this->focus = focus;
}

void HumidityEdit::display() {
	BarGraph barG(lcd, 50);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.Print(title);
	lcd.setCursor(0,1);
	char s[16];
	if(focus) {
		snprintf(s, 16, "[%4.2f]%%", edit);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(edit*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	else {
		snprintf(s, 16, " %4.2f %%", value);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(value*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	lcd.Print(s);
	//delete the char
	//delete s;
}


float HumidityEdit::getValue() {
	return value;
}

float HumidityEdit::getDesValue() {
	return desVal;
}

float HumidityEdit::getTol() {
	return tol;
}

