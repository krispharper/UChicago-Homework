#ifndef SOERTEDVEC_H
#define SOERTEDVEC_H
#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef unsigned int uint;

class sortedVec{
  public:
  std::vector<uint> vec;
  sortedVec():vec(0){}
  sortedVec( int i0, int i1=-1, int i2=-1, int i3=-1, int i4=-1);

  uint& operator[]( uint i ){ return vec[i];}
  uint operator[]( uint i ) const { return vec[i];}

  bool remove( uint i);
  void augment( const sortedVec& a );
  uint size() const {return vec.size();};
  bool contains( uint i );
};

#endif
