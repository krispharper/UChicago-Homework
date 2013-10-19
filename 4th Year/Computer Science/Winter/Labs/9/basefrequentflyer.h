#ifndef BASEFREQUENTFLYER_H
#define BASEFREQUENTFLYER_H
#include <iostream>
#include <string>
using namespace std;

class BaseFrequentFlyer {
	string name;
	int number;
	int miles;

	public:
	BaseFrequentFlyer(string name, int number, int miles)
		: name(name), number(number), miles(miles){}
	
	void addMiles(int realMiles)
		{miles += realMilesToAirmiles(realMiles);}

	virtual int realMilesToAirmiles(int realMiles) = 0;

	void printData()
		{cout << name << '\t' << number << '\t' << miles << endl;}

	~BaseFrequentFlyer(){}
};
#endif
