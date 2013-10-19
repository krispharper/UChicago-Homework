// interface for real matrix 
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
typedef unsigned int uint;

class matrix{
	public:
		uint row, col;
		matrix( uint r, uint c )
			: row(r), col(c){}
};

class fullMatrix : public matrix{
	public:
		double *v;

		fullMatrix(uint r, uint c);
		fullMatrix(const fullMatrix& A);

		// constructs a matrix with random integer entries
		// betwen 0 and n
		fullMatrix(uint r, uint c, uint n);
		~fullMatrix(){delete [] v;}

		double& operator() (uint i, uint j);
		double operator() (uint i, uint j) const;
		std::ostream& dump(std::ostream& os) const;
		fullMatrix& operator= (const fullMatrix& A);
		fullMatrix& operator+= (const fullMatrix& A);
		fullMatrix& operator-= (const fullMatrix& A);
		fullMatrix& operator*= (const fullMatrix& A);
		fullMatrix operator+ (const fullMatrix& A);
		fullMatrix operator- (const fullMatrix& A);
		fullMatrix operator* (const fullMatrix& A);
		double operator, (const fullMatrix& A);
		// a couple comparison operators that might be useful
		bool operator== (const fullMatrix& A) const;
		bool operator!= (const fullMatrix& A) const;
};

std::ostream& operator<< (std::ostream& os, const fullMatrix& A);
fullMatrix operator* (double a, const fullMatrix& A);
fullMatrix operator* (const fullMatrix& A, double a);
fullMatrix pow(const fullMatrix& A, uint n);
fullMatrix eye(uint n);
#endif
