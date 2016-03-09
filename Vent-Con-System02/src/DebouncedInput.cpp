/*
 * DebouncedInput.cpp
 *
 *  Created on: 31.1.2016
 *      Author: krl
 */

#include "DebouncedInput.h"

DebouncedInput::DebouncedInput(int arduinoPin, unsigned int debounceTimeMs)
: DigitalIoPin(arduinoPin, DigitalIoPin::pullup, true),
  stableTime(0), last(0), state(false), debounceTime(debounceTimeMs) {
	// TODO Auto-generated constructor stub

}

DebouncedInput::~DebouncedInput() {
	// TODO Auto-generated destructor stub
}

bool DebouncedInput::read() {
	bool value = DigitalIoPin::read();
	unsigned int now = GetSysticks();

	// see if current value is different from last reported value
	if(value != state) {
		// it is different from last reported increment stable time counter
		stableTime += now - last;
	}
	else {
		// went back to old value - reset stable time counter
		stableTime = 0;
	}
	last = now;

	// we change the reported value only when the new value has been stable for at least debounceTime
	if(stableTime > debounceTime) {
		state = value;
		stableTime = 0;
	}

	return state;
}
