/*
 * PresEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: georgrokita
 */

#include "PresEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




PresEdit::PresEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =0;
	tol = 0.5;
	focus =false;
	upperL = _upperL;
	lowerL = _lowerL;
}


/*

bool PresEdit::setFrequency(ModbusMaster& node, uint16_t freq) {
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
*/
PresEdit::~PresEdit() {
	// TODO Auto-generated destructor stub
}


void PresEdit::save() {
	// set current value to be same as edit value
	desVal = edit;
	TempSensFlag = false;
	PresSensFlag = true;
	//setFrequency(node, value);

	// todo: save current value for example to EEPROM for permanent storage
}

void PresEdit::setSensValue(float val){
	value = val;
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
}

void PresEdit::cancel() {
	edit = desVal;
}


void PresEdit::setFocus(bool focus) {
	this->focus = focus;
}

void PresEdit::display() {
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.Print(title);
	lcd.setCursor(0,1);
	char s[16];
	if(focus) {
		snprintf(s, 16, "     [%4.2f]     ", edit);
	}
	else {
		snprintf(s, 16, "      %4.2f      ", value);
	}
	lcd.Print(s);
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

