#ifndef MATRIXH
#define MATRIXH
#include <iomanip>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>

typedef unsigned int uint;
using std::ostream;

#include "utility.h"

class matrix{
 public:
  uint row, col;
  matrix( uint r, uint c )
    : row(r), col(c){}
};

class fullMatrix : public matrix{
  // This is a row x col matrix
public:
 double *v;
 fullMatrix(uint r , uint c=1);       // constructors & destructor
 fullMatrix( const fullMatrix& );
 ~fullMatrix(){delete [] v;};

 double& operator()(uint i, uint j=0)
 { return (v[i*col+j]);}
 double operator()(uint i, uint j=0)const
 { return (v[i*col+j]);}

 fullMatrix& operator=( const fullMatrix& B){   // *this=B
   // note that this does not follow the standard form for
   // writing assignment operators. Instead it says it is
   // an error to write A = B unless A & B have the same size.

   if( row!=B.row || col!=B.col)
     error("fullMatrix=: incompatible sizes");
   uint i, rc = row*col;
   for( i=0; i<rc; i++ ) v[i] = B.v[i];
   return *this;
 }

 fullMatrix& operator+=( const fullMatrix& B){   // *this+=B
   if( row!=B.row || col!=B.col)
     error("fullMatrix+=: incompatible sizes");
   uint i, rc = row*col;
   for( i=0; i<rc; i++ ) v[i] += B.v[i];
   return *this;
 }

 fullMatrix& operator-=( const fullMatrix& B){   // *this-=B
   if( row!=B.row || col!=B.col)
     error("fullMatrix-=: incompatible sizes");
   uint i, rc = row*col;
   for( i=0; i<rc; i++ ) v[i] -= B.v[i];
   return *this;
 }

  void swapRows( uint i, uint k ); // interchange rows i & k
};

fullMatrix operator+(const fullMatrix& A, const fullMatrix& B); // A+B
fullMatrix operator-(const fullMatrix& A, const fullMatrix& B); // A-B
fullMatrix operator*(const fullMatrix& A, const fullMatrix& B); // A*B
fullMatrix operator*( double a, const fullMatrix& B); // a*B
fullMatrix operator*( const fullMatrix& B, double a); // B*a
double operator,(const fullMatrix& A, const fullMatrix& B); // (A:B)
fullMatrix eye( uint n ); // identity
fullMatrix transpose(const fullMatrix&); // return A^T
// solve A X = Y
void solve(const fullMatrix& A, fullMatrix& X, const fullMatrix& y);
fullMatrix abs( const fullMatrix& A ); // abs each entry
fullMatrix& operator*=( fullMatrix& A, const fullMatrix& B);

fullMatrix pow( const fullMatrix & A, uint n );
ostream& dump(ostream& os, const fullMatrix& A);
ostream& operator<<(ostream& os, const fullMatrix& A);

#endif
