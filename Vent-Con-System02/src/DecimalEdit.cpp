/*
 * DecimalEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#include "DecimalEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




DecimalEdit::DecimalEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =0;
	focus =false;
	upperL = _upperL;
	lowerL = _lowerL;
}


bool DecimalEdit::setFrequency(ModbusMaster& node, uint16_t freq) {
	uint8_t result;
	int ctr;
	bool atSetpoint;
	const int delay = 500;

	node.writeSingleRegister(1, freq); // set motor frequency

	printf("Set freq = %d\n", freq/40); // for debugging

	// wait until we reach set point or timeout occurs
	ctr = 0;
	atSetpoint = false;
	do {
		Sleep(delay);
		// read status word
		result = node.readHoldingRegisters(3, 1);
		// check if we are at setpoint
		if (result == node.ku8MBSuccess) {
			if(node.getResponseBuffer(0) & 0x0100) atSetpoint = true;
		}
		ctr++;
	} while(ctr < 20 && !atSetpoint);

	printf("Elapsed: %d\n", ctr * delay); // for debugging

	return atSetpoint;
}

DecimalEdit::~DecimalEdit() {
	// TODO Auto-generated destructor stub
}

float DecimalEdit::getTol() {
	return tol;
}


void DecimalEdit::save() {
	// set current value to be same as edit value
	value = edit;

	//needs to be uncommented when working with ModBus
	setFrequency(node, value);

}

void DecimalEdit::setValue(float value){
	edit = value;
	save();
}


void DecimalEdit::increment() {
	edit = edit + 400;
	if(edit >=upperL){
		edit = upperL;
	}
}

void DecimalEdit::decrement() {
	edit = edit - 400;
	if(edit <=lowerL){
		edit = lowerL;
	}
}


void DecimalEdit::accept() {
	save();
}

void DecimalEdit::cancel() {
	edit = value;
}


void DecimalEdit::setFocus(bool focus) {
	this->focus = focus;
}

void DecimalEdit::display() {
	BarGraph barG(lcd, 50);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.Print(title);
	lcd.setCursor(0,1);
	char s[16];
	if(focus) {
		snprintf(s, 16, "[%4.2f]%", edit);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(edit*50)/((upperL-lowerL)*2);
		barG.draw(result);

	}
	else {
		snprintf(s, 16, " %4.2f %", value);
		lcd.Print(s);
		lcd.setCursor(9,1);
		int result =(edit*50)/((upperL-lowerL)*2);
		barG.draw(result);
	}
	lcd.Print(s);
}


float DecimalEdit::getValue() {
	return value;
}

