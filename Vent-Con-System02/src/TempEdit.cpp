/*
 * TempEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: georgrokita
 */

#include "TempEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




TempEdit::TempEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =0;
	focus =false;
	tol = 0.1;
	upperL = _upperL;
	lowerL = _lowerL;
}

TempEdit::~TempEdit() {
	// TODO Auto-generated destructor stub
}


void TempEdit::save() {
	// set current value to be same as edit value
	desVal = edit;

}


//this sets the desVal to the user input
void TempEdit::setValue(float val){
	edit = val;
	save();
}

//this sets the read value from the sens to the obj
void TempEdit::setSensValue(float val){
	value = val;
	//display();

}


void TempEdit::increment() {
	edit = edit + 0.2;
	if(edit >=upperL){
		edit = upperL;
	}
}

void TempEdit::decrement() {
	edit = edit - 0.2;
	if(edit <=lowerL){
		edit = lowerL;
	}
}


void TempEdit::accept() {
	save();
	TempSensFlag = true;
	PresSensFlag = false;
	AutoFlag = false;
}

void TempEdit::cancel() {
	edit = desVal;
}


void TempEdit::setFocus(bool focus) {
	this->focus = focus;
}

void TempEdit::display() {
	BarGraph barG(lcd, 50);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.Print(title);
	lcd.setCursor(0,1);
	char s[16];
	if(focus) {
		snprintf(s, 16, "[%4.2f]C", edit);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(edit*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	else {
		snprintf(s, 16, " %4.2f C", value);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(value*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	lcd.Print(s);
}


float TempEdit::getValue() {
	return value;
}

float TempEdit::getDesValue() {
	return desVal;
}

float TempEdit::getTol() {
	return tol;
}

