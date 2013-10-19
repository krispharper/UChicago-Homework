#include <assert.h>
#include <iomanip>
#include <cstdlib>
#include "matrix.h"
using std::ostream;

// basic row and column constructor (empty matrix)
fullMatrix::fullMatrix( uint r, uint c )
: matrix(r,c)
{
	uint i;
	v = new double[r*c];   
	for(i=0; i < r*c; i++)
		v[i] = 0;
}

// build a matrix from another
fullMatrix::fullMatrix(const fullMatrix& A)
: matrix(A.row, A.col)
{
	uint i, n = A.row*A.col;
	v = new double[n];   
	for( i=0; i<n; i++ )
		v[i] = A.v[i];
}

// build an r by c matrix with random integer entries less than n
fullMatrix::fullMatrix(uint r, uint c, uint n)
: matrix(r, c)
{
	uint i;
	v = new double[r*c];
	for(i=0; i < r*c; i++)
		v[i] = rand() % n;
}

// get the (i,j)th entry
double& fullMatrix::operator() (uint i, uint j)
{
	return v[j*row+i];
}

// same as above for const variables
double fullMatrix::operator() (uint i, uint j) const
{
	return v[j*row+i];

}

// print out the matrix
ostream& fullMatrix::dump(ostream& os) const
{
	uint i,j;

	for( i=0; i<row; i++ ){
		for(j=0; j<col; j++ )
			std::cout << std::setw(4) << v[j*row+i] << " ";
		std::cout << std::endl;
	}
	return os;
}

// overloaded = operator
fullMatrix& fullMatrix::operator= (const fullMatrix& A)
{
	assert(A.row == row && A.col == col);
	// check for self assignment
	if(this == &A)
		return *this;
	uint i;
	for(i=0; i < row * col; i++)
		v[i] = A.v[i];
	return *this;
}

// overloaded += operator
fullMatrix& fullMatrix::operator+= (const fullMatrix& A)
{
	assert(A.row == row && A.col == col);
	uint i;
	for(i=0; i < row*col; i++)
		v[i] += A.v[i];
	return *this;
}

// overloaded -= operator
fullMatrix& fullMatrix::operator-= (const fullMatrix& A)
{
	assert(A.row == row && A.col == col);
	uint i;
	for(i=0; i < row*col; i++)
		v[i] -= A.v[i];
	return *this;
}

// overloaded *= operator
fullMatrix& fullMatrix::operator*= (const fullMatrix& A)
{
	assert(A.row == col);
	uint i,j,k;
	double temp = 0;
	fullMatrix result(row, A.col);

	for(i=0; i < row; i++){
		for(j=0; j < A.col; j++){
			for(k=0; k<col; k++)
				temp += (*this)(i,k) * A(k,j);
			result(i,j) = temp;
			temp = 0;
		}
	}
	// change dimensions to match output
	col = A.col;
	delete v;
	v = new double [row*col];
	*this = result;
	return *this;
}

// equality comparison operators
bool fullMatrix::operator== (const fullMatrix& A) const
{
	uint i;
	bool temp = 1;

	for(i=0; i < row*col; i++)
		temp = temp && (v[i] == A.v[i]);
	return temp;
}

bool fullMatrix::operator!= (const fullMatrix& A) const
{
	return !(*this == A);
}

// component-wise addition
fullMatrix fullMatrix::operator+ (const fullMatrix& A)
{
	assert(A.row == row && A.col == col);
	return fullMatrix(*this) += A;
}

// component-wise subtraction
fullMatrix fullMatrix::operator- (const fullMatrix& A)
{
	assert(A.row == row && A.col == col);
	return fullMatrix(*this) -= A;
}

// component-wise subtraction
fullMatrix fullMatrix::operator* (const fullMatrix& A)
{
	assert(A.row == col);
	return fullMatrix(*this) *= A;
}

// frobenius inner product
double fullMatrix::operator, (const fullMatrix& A)
{
	assert(row == A.row && col == A.col);
	uint i;
	double result = 0;

	for(i=0; i < row*col; i++)
		result += v[i]*A.v[i];
	return result;
}

fullMatrix pow(const fullMatrix& A, uint n)
{
	assert(A.row == A.col);
	assert(n >= 0);
	fullMatrix result = eye(A.row);
	fullMatrix powers = A;

	if(n == 0)
		return eye(A.row);
	if(n == 1)
		return A;
	while(n){
		result = (n % 2 == 0) ? result : powers * result;
		n /= 2;
		powers = powers * powers;
	}
	return result;
}

// overloaded << operator for use with cout
ostream& operator<< (ostream& os, const fullMatrix& A)
{
	return A.dump(os);
}

// scalar multiplication
fullMatrix operator* (double a, const fullMatrix& A)
{
	uint i;
	fullMatrix temp(A.row, A.col);

	for(i=0; i < A.row * A.col; i++)
		temp.v[i] = a*A.v[i];
	return temp;
}

fullMatrix operator* (const fullMatrix& A, double a)
{
	return a*A;
}

// returns the n by n identity matrix
fullMatrix eye(uint n)
{
	assert(n > 0);
	uint i;
	fullMatrix temp(n, n);
	for(i=0; i < n; i++)
		temp(i,i) = 1;
	return temp;
}
