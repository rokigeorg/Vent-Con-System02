/*
 * FeedBack.h
 *
 *  Created on: 11.02.2016
 *      Author: abdullai
 */

#ifndef FEEDBACK_H_
#define FEEDBACK_H_

#include "PropertyEdit.h"
#include "ModbusMaster.h"
#include "SensorGeneral.h"

class FeedBack {
public:
	FeedBack(SensorGeneral *s, PropertyEdit *p, PropertyEdit *setFrq);
	virtual ~FeedBack();
	void reload(SensorGeneral *s, PropertyEdit *p);
	void check();
private:
	SensorGeneral *sens;
	PropertyEdit *prop;
	PropertyEdit *setFrq;

};

/*

class FeedBack
{
  public:
    // 2. Define a public static accessor func
    static FeedBack *FBinstance();
    static void reset(SensorGeneral *s, PropertyEdit *p, PropertyEdit *setFrq)
    {
      type = t;
      delete inst;
      inst = 0;
    }

  private:
    FeedBack();
    void check();
	SensorGeneral *sens;
	PropertyEdit *prop;
	PropertyEdit *setFrq;
};

*/

#endif /* FEEDBACK_H_ */
