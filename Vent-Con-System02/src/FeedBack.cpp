/*
 * Feedback.cpp
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#include "FeedBack.h"
#include "ModbusMaster.h"
#include "SensorGeneral.h"

/*
FeedBack *FeedBack::instance()
{
  if (!inst)
    // 3. Do "lazy initialization" in the accessor function
    if (type == "octal")
      inst = new Octal();
    else
      inst = new Number();
  return inst;
}
*/

// Implementation
FeedBack* FeedBack::_instance = 0;

FeedBack* FeedBack::Instance(SensorGeneral *s, PropertyEdit *p, PropertyEdit *setF) {
    if (_instance == 0) {
        _instance = new FeedBack;
        _instance->sens = s;
        //sens = s;
        _instance->prop = p;
        _instance->setFrq = setF;
    }
    return _instance;
}
void FeedBack::reload(SensorGeneral *s, PropertyEdit *p)
{
	sens = s;
	prop = p;
}

void FeedBack::check()
{

	//p->setSensValue(sens->readValues());
	if ((_instance->prop->getValue() > (_instance->prop->getDesValue() - _instance->prop->getTol()))&& (_instance->prop->getValue() < (_instance->prop->getDesValue() + _instance->prop->getTol())))
		_instance->setFrq->setValue(_instance->setFrq->getValue()/5);
	else if(_instance->prop->getValue() < (_instance->prop->getDesValue() + _instance->prop->getTol()))
		_instance->setFrq->decrement();
	else if(_instance->prop->getValue() > (_instance->prop->getDesValue() - _instance->prop->getTol()))
		_instance->setFrq->increment();


	setFrq->save();

}
FeedBack::FeedBack(){

}



FeedBack::~FeedBack() {
	// TODO Auto-generated destructor stub
}


