#include <iostream>
#include <cstdlib>
#include <time.h>
#include "matrix.h"
using std::cout;
using std::endl;

int main()
{
	// pick a seed for generating random matrices
	srand(time(NULL));
	fullMatrix A(3,3,20);
	fullMatrix B(3,3,20);
	fullMatrix C(3,3,20);
	fullMatrix D(3,2,20);
	fullMatrix E(2,3,20);
	double a = rand() % 20 + 1;
	uint n = rand() % 3 + 2;
	cout << "Matrix A:\n" << A << endl;
	cout << "Matrix B:\n" << B << endl;
	cout << "Matrix C:\n" << C << endl;
	cout << "Matrix D:\n" << D << endl;
	cout << "Matrix E:\n" << E << endl;
	C = A;
	cout << "Testing C=A:\n" << C  << endl;
	cout << "Testing A+B:\n" << A + B << endl;
	cout << "Testing A-B:\n" << A - B << endl;
	cout << "Testing A*B:\n" << A * B << endl;
	cout << "Testing D*E:\n" << D * E << endl;
	cout << "Testing A^" << n << ":\n" << pow(A,n) << endl;
	cout << "Testing " << a << "*A:\n" << a*A << endl;
	cout << "Testing A*" << a << ":\n" << A*a << endl;
	cout << "Testing (A,B):\n" << (A,B) << endl;
	C += B;
	cout << "Testing A+=B:\n" << C << endl;
	C = A;
	C -= B;
	cout << "Testing A-=B:\n" << C << endl;
	C = A;
	C *= B;
	cout << "Testing A*=B:\n" << C << endl;
	cout << "Testing eye(3):\n" << eye(3) << endl;
	bool equal = A == B;
	bool notequal = A != B;
	cout << "Testing A == B:\n" << equal << endl;
	cout << "Testing A != B:\n" << notequal << endl;
	return 0;
}
