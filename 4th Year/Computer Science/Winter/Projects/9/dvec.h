// interface for points in n-space

#ifndef DVECH
#define DVECH
#include <iostream>
#include <vector>

class dvec{
  public:
  std::vector<double> v;

  //constructors

  // builds zero vector
  dvec( unsigned int n );

  // build from data
  dvec( const std::vector<double>& w );

  // build from 3 doubles
  dvec( double x, double y, double z );

  // build from 2 doubles
  dvec( double x, double y);


  std::ostream& print( std::ostream& os) const;

  void print4plot(std::ostream& os) const;

  unsigned int size() const;

};

double  norm( const dvec& p);
dvec operator-( const dvec& p, const dvec& q);
dvec operator+( const dvec& p, const dvec& q);
dvec operator*( double a, const dvec& q);

std::ostream& operator << (std::ostream& os, const dvec& p);
#endif
