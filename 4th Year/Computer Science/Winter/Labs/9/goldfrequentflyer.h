#ifndef GOLDFREQUENTFLYER_H
#define GOLDFREQUENTFLYER_H
#include <iostream>
#include <string>
#include "basefrequentflyer.h"
using namespace std;

class GoldFrequentFlyer : public BaseFrequentFlyer {
	int numFlights;

	public :
	GoldFrequentFlyer(string name, int number, int miles, int numFlights)
		: BaseFrequentFlyer(name, number, miles), numFlights(numFlights) {}
	
	int realMilesToAirmiles(int realMiles)
		{return realMiles * (numFlights/10 + 1);}
};
#endif
