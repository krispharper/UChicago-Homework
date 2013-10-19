// curveLength.cpp -- adaptive computation of a curvelength
// in n-space

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#include "dvec.h"

using std::cout; using std::endl;
using std::ofstream; using std::ostream;
using std::vector;

typedef dvec (*vfunctp)(double); // pointer to a function

// functions to integrate
dvec spring( double t );
dvec wrap( double t );
dvec sticks( double t );
dvec line( double t );
dvec parabola( double t);
dvec sqrt_sin( double t);

// interface function for curve length
double curveLength(double a, double b, vfunctp f, double tol=1.0e-4);

// internal adaptive arc length
double curveLen(double a, const dvec& fa, double b, dvec& fb,
				vfunctp f, double tol, ostream& os);

// global variables
int functionEvaluations;
const char *fnName=0;

void drawLine( const dvec& p, const dvec& q, ostream& os);

int main()
{
  fnName = "spring.dat";
  cout << "The length of spring[0,1] is " 
       << curveLength(0.0,1.0,spring,0.001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";

  fnName = "wrap.dat";
  cout << "The length of wrap[0,1] is " 
       << curveLength(0.0,1.0,wrap,0.001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";

  fnName = "sticks.dat";
  cout << "The length of sticks[0,5] is " 
       << curveLength(0.0,5.0,sticks,0.00001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";

  fnName = "line.dat";
  cout << "The length of line[0,2] is " 
       << curveLength(0.0,2.0,line,0.00001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";

  fnName = "parabola.dat";
  cout << "The length of parabola[0,1] is " 
       << curveLength(0.0,1.0,parabola,0.00001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";

  fnName = "sqrt_sin.dat";
  cout << "The length of sqrt_sin[0,1] is " 
       << curveLength(0.0,1.0,sqrt_sin,0.00001) << endl;
  cout<< "  This took " <<  functionEvaluations 
       << " function evaluations.\n";


  dvec q(1.0,2.0,3.0);
  cout << "To test the overload of << for 3-d dvec, we print"
       << " a dvec of size "<< q.size()
       << ": "<< q << ".\n";

  dvec w(1.2,3.1);
  dvec z(w);
  cout << "To test the overload of << for 2-d dvec, we print"
       << " a dvec of size "<< w.size()
       << ": "<< w << ".\n";

  return 0;
}

double curveLength(double a, double b, vfunctp f, double tol)
{
	ofstream pict(fnName);
	dvec fa = f(a), fb = f(b);
	functionEvaluations = 2;

	return curveLen(a, fa, b, fb, f, tol, pict);
}

double curveLen(double a, const dvec& fa, double b, dvec& fb,
				vfunctp f, double tol, ostream& os)
{
	static int level = 0;
	++level;
	double course = norm(fa-fb);
	double m = .5*(a+b);
	dvec fm = f(m);
	++functionEvaluations;
	double fine = norm(fa-fm) + norm(fm-fb);

	bool isgood = fine - course < tol;
	// ok levels of refinement are [3,30]
	isgood = isgood && level > 2;
	isgood = isgood || level >= 30;

	if(!isgood)
		fine = curveLen(a, fa, m, fm, f, tol*.5, os)
				+ curveLen(m, fm, b, fb, f, tol*.5, os);
	if(isgood){
		drawLine(fa, fm, os);
		drawLine(fm, fb, os);
	}

	--level;
	return fine;
}

void drawLine( const dvec& p, const dvec& q, ostream& os)
{// "draw" a line in 3 space from p to q
  p.print4plot(os);
  q.print4plot(os);
  os << std::endl;
}

// parametric spiral in 3d
dvec spring(double t)
{
	double c = 10*3.141592653589;
	double r = .25;
	double x = r*cos(c*t);
	double y = r*sin(c*t);
	double z = t;
	return dvec(x,y,x);
}

// parametric spiral on the sphere
dvec wrap(double t)
{
	double c = 10*3.141592653589;
	double b = sqrt(t) - sqrt(1-t);
	double a = sqrt(1-b*b);
	double x = a*cos(c*t);
	double y = a*sin(c*t);
	double z = b;
	return dvec(x,y,z);
}

// connect the dots
dvec sticks(double t)
{
	vector<dvec> p;
	p.push_back(dvec(-1,-1,-1));
	p.push_back(dvec(1,0,0));
	p.push_back(dvec(-1,.8,.8));
	p.push_back(dvec(1,.8,.8));
	p.push_back(dvec(-1,-1,1));
	p.push_back(dvec(-1,1,-1));

	if(t<=0.0) return p[0];
	else if( t<=1 ) return (1-t)*p[0]+t*p[1];
	else if( t<=2 ) return (2-t)*p[1]+(t-1)*p[2];
	else if( t<=3 ) return (3-t)*p[2]+(t-2)*p[3];
	else if( t<=4 ) return (4-t)*p[3]+(t-3)*p[4];
	else if( t<=5 ) return (5-t)*p[4]+(t-4)*p[5];
	else return p[5];
}

dvec line(double t)
{return dvec(t,2*t);}

dvec parabola(double t)
{return dvec(t, t*t);}

dvec sqrt_sin(double t)
{return dvec(t, sqrt(sin(t)));}
