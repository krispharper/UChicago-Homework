#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef unsigned int uint;

class sortedVec{
	public:
	std::vector<uint> v;
	
	sortedVec() {}
	//sortedVec(const sortedVec& w)
	//	{v = w.v;}
	sortedVec(uint x0, uint x1)
		: v(2) {v[0] = x0; v[1] = x1;}
	sortedVec(uint x0, uint x1, uint x2)
		: v(3) {v[0] = x0; v[1] = x1; v[2] = x2;}
	sortedVec(uint x0, uint x1, uint x2, uint x3)
		: v(4) {v[0] = x0; v[1] = x1; v[2] = x2; v[3] = x3;}
	
	void augment(const sortedVec& a);
	void remove (uint i);
	uint size()
		{return v.size();}
};

void sortedVec::augment(const sortedVec& a)
{
	uint i; 
	sortedVec b = a;

	for(i=0; i < v.size(); i++)
		b.remove(v[i]);

	for(i=0; i < b.size(); i++)
		v.push_back(b.v[i]);
	sort(v.begin(), v.end());
}

void sortedVec::remove(uint i)
{
	uint j;

	for(j=0; j < v.size(); j++)
		if(v[j] == i)
			v.erase(v.begin() + j);
}
