#ifndef FREQUENTFLYER_H
#define FREQUENTFLYER_H
#include <iostream>
#include <string>
#include "basefrequentflyer.h"
using namespace std;

class FrequentFlyer : public BaseFrequentFlyer {
	public :
	FrequentFlyer(string name, int number, int miles)
		:BaseFrequentFlyer(name, number, miles) {}
	
	int realMilesToAirmiles(int realMiles)
		{return realMiles;}
};
#endif
