// Kris Harper
// CMSC 15200
// Project 9
// 2.25.11

// implementation for points in 3-space

#include <cmath>
#include <assert.h>
#include <vector>
#include <iostream>

#include "dvec.h"

using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;
using std::abs;
using std::vector;

dvec::dvec(unsigned int n)
: v(n) {}

dvec::dvec(const vector<double>& w)
{v = w;}

dvec::dvec(double x, double y, double z)
{
	v.push_back(x);
	v.push_back(y);
	v.push_back(z);
}

dvec::dvec(double x, double y)
{
	v.push_back(x);
	v.push_back(y);
}

ostream& dvec::print(ostream& os) const
{
	int i;

	os << '(';
	for(i=0; i<v.size() - 1; i++)
		os << v[i] << ',';
	os << v[v.size() - 1] << ')';
	return os;
}

void dvec::print4plot(ostream& os) const
{
	int i;

	for(i=0; i<v.size() - 1; i++)
		os << v[i] << ' ';
	os << v[v.size() - 1] << endl;
}

unsigned int dvec::size() const
{
	return v.size();
}

double norm(const dvec& p)
{
	int i;
	double sum = 0;

	for(i=0; i<p.size(); i++)
		sum += p.v[i] * p.v[i];
	return sqrt(sum);
}

dvec operator -(const dvec& p, const dvec& q)
{
	assert(p.size() == q.size());

	int i;
	
	dvec temp = dvec(p.size());
	for(i=0; i<p.size(); i++)
		temp.v[i] = p.v[i] - q.v[i];
	return temp;
}

dvec operator +(const dvec& p, const dvec& q)
{
	assert(p.size() == q.size());

	int i;

	dvec temp = dvec(p.size());
	for(i=0; i<p.size(); i++)
		temp.v[i] = p.v[i] + q.v[i];
	return temp;
}

dvec operator *(double a, const dvec& q)
{
	int i;

	dvec temp = dvec(q.size());
	for(i=0; i<q.size(); i++)
		temp.v[i] = a*q.v[i];
	return temp;
}

ostream& operator << (ostream& os, const dvec& p)
{
	return p.print(os);
}
