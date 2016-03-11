/*
 * Feedback.cpp
 *
 *  Created on: 11.02.2016
 *      Author: georgrokita
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
void FeedBack::reload(SensorGeneral *s, PropertyEdit *p)
{
	sens = s;
	prop = p;
}

void FeedBack::check()
{

	//p->setSensValue(sens->readValues());

	if(prop->getValue() < (prop->getDesValue() - prop->getTol()))
		setFrq->decrement();
	else if(prop->getValue() < (prop->getDesValue() - prop->getTol()))
		setFrq->increment();
	else
		setFrq->setValue(setFrq->getValue()/5);

	//setFrq->save();

}
FeedBack::FeedBack(SensorGeneral *s, PropertyEdit *p, PropertyEdit *setFrq){

	sens = s;
	prop = p;
	this->setFrq = setFrq;
}

FeedBack::~FeedBack() {
	// TODO Auto-generated destructor stub
}


