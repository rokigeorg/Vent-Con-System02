/*
 * DebouncedInput.h
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#ifndef DEBOUNCEDINPUT_H_
#define DEBOUNCEDINPUT_H_

#include "DigitalIoPin.h"

unsigned int GetSysticks();

class DebouncedInput: public DigitalIoPin {
public:
	DebouncedInput(int arduinoPin, unsigned int debounceTimeMs);
	virtual ~DebouncedInput();
	bool read();
private:
	unsigned int stableTime;
	unsigned int last;
	bool state;
	unsigned int debounceTime;
};

#endif /* DEBOUNCEDINPUT_H_ */
