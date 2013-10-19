#ifndef SILVERFREQUENTFLYER_H
#define SILVERFREQUENTFLYER_H
#include <iostream>
#include <string>
#include "basefrequentflyer.h"
using namespace std;

class SilverFrequentFlyer : public BaseFrequentFlyer {
	int years;

	public :
	SilverFrequentFlyer(string name, int number, int miles, int years)
		: BaseFrequentFlyer(name, number, miles), years(years) {}
	
	int realMilesToAirmiles(int realMiles)
		{return realMiles*(years + 1);}
};
#endif
