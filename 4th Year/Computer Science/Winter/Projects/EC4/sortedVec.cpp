// implementation for sorted vectors of unsigned ints
#include <algorithm>
#include "sortedVec.h"
#include "utility.h"

sortedVec::sortedVec(int i0, int i1, int i2, int i3, int i4)
{
	uint i;
	if(i0 >= 0)
		vec.push_back(static_cast<uint>(i0));
	if(i1 >= 0)
		vec.push_back(static_cast<uint>(i1));
	if(i2 >= 0)
		vec.push_back(static_cast<uint>(i2));
	if(i3 >= 0)
		vec.push_back(static_cast<uint>(i3));
	if(i4 >= 0)
		vec.push_back(static_cast<uint>(i4));
	sort(vec.begin(), vec.end());
}

bool sortedVec::remove(uint i)
{ // remove i if it is in the vec
	// return true if it was found
	uint j=0, ls = vec.size(), k;
	for(; j<ls; j++){
		if(vec[j] == i){
			for(k=j+1; k<ls; k++)
				vec[k-1] = vec[k];
			vec.resize(ls-1);
			return true;
		}
	}
	return false;
}

void sortedVec::augment(const sortedVec& a)
{ // replace *this with the ordered vector containing *this and a
	using namespace std;
	uint vp=0, mp=0, ap=0, as=a.size(), vs=vec.size();

	if(as == 0) return; // nothing to do

	vector<uint> m(vs+as); // probably too big
	for(;;){
		if(vp==vs){ // finished vec, copy in rest of a
			for(; ap<as;)
				m[mp++] = a[ap++];
			break;
		}
		else if(ap==as){ // finished a, copy in rest of vec
			for(; vp<vs;)
				m[mp++] = vec[vp++];
			break;
		}
		else if(vec[vp]<=a[ap]){
			if(vec[vp]==a[ap]) ap++;
			m[mp++] = vec[vp++];
		}
		else
			m[mp++] = a[ap++];
	}
	m.resize(mp);
	vec = m;
}

bool sortedVec::contains(uint i)
{ // returns true if i is in *this
	// could use bisection, but didn't
	uint j;
	for(j=0; j<vec.size(); j++)
		if(i == vec[j])
			return 1;
	return 0;
}

