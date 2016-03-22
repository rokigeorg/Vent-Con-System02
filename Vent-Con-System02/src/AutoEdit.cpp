/*
 * AutoEdit.cpp
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#include "AutoEdit.h"
#include "ModbusMaster.h"
#include <cstdio>




AutoEdit::AutoEdit(LiquidCrystal& lcd_, ModbusMaster& node_, std::string editTitle, float _upperL,float _lowerL , float _desvalue): lcd(lcd_),node(node_), title(editTitle){

	value =0;
	edit =22.0;
	focus =false;
	tol = 0.5;
	upperL = _upperL;
	lowerL = _lowerL;
	desVal = 22.0;

	ventCharFlag= true;
}

AutoEdit::~AutoEdit() {
	// TODO Auto-generated destructor stub
}


void AutoEdit::save() {
	// set current value to be same as edit value
	desVal = edit;

}

//this sets the desVal to the user input

void AutoEdit::setValue(float val){
	edit = val;
	save();
}


//this sets the read value from the sens to the obj
void AutoEdit::setSensValue(float val){
	value = val;
	//display();

}


void AutoEdit::accept() {
	save();
	TempSensFlag = false;
	PresSensFlag = false;
	AutoFlag = true;
}

void AutoEdit::cancel() {
	edit = desVal;
}


void AutoEdit::setFocus(bool focus) {
	this->focus = false;
	accept();
}

void AutoEdit::setTitle(std::string tit) {
	title = tit;
	display();
}

std::string AutoEdit::getTitle() {
	return title;
}

void AutoEdit::display() {
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
	//delete s;
}


float AutoEdit::getValue() {
	DEBUGOUT("Tol is %.3f Value is %.3f and DesValue is %.3f\r\n", tol, value, desVal ,"\n");
	return value;
}

float AutoEdit::getDesValue() {
	return desVal;
}

float AutoEdit::getTol() {
	return tol;
}

void AutoEdit::setStartUpTitle(std::string openerStr){
	BarGraph barG(lcd, 50);
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.Print(openerStr);

	drawSpinningVentChar();
}
void AutoEdit::drawSpinningVentChar(){
	std::string plusSymb= "+";
	uint8_t charSpecX[8] = {0x00,0x11,0x0A,0x04,0x0A,0x11,0x00,0x00};
	static int i;
	i++;
	if(ventCharFlag){
		//write it to the lcd memory on position 6
		lcd.createChar(6,charSpecX);
		lcd.setCursor(5,1);
		// tell the lcd to print the charator from memory position 6
		lcd.write(6);
		lcd.setCursor(7,1);
		// tell the lcd to print the charator from memory position 6
		lcd.write(6);
		lcd.setCursor(9,1);
		// tell the lcd to print the charator from memory position 6
		lcd.write(6);
		if(i>10){
			ventCharFlag=false;
			i=0;
		}
	}else{
		lcd.setCursor(5,1);
		lcd.Print(plusSymb);
		lcd.setCursor(7,1);
		lcd.Print(plusSymb);
		lcd.setCursor(9,1);
		lcd.Print(plusSymb);
		if(i>10){
			ventCharFlag=true;
			i=0;
		}
	}
}
