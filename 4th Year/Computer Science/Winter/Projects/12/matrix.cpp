#include "matrix.h"

fullMatrix::fullMatrix(uint r, uint c)
  : matrix(r,c)
{ // set up a matrix of zeros
   if (r<=0 || c<=0)error("bad matrix indeces \n");
   v = new double[r*c];
   for(uint i=0; i<r*c; i++) 
     v[i]=0.0;
 };

fullMatrix::fullMatrix( const fullMatrix& A)
  :matrix(A.row,A.col)
{ // build a matrix from another one
   v = new double[row*col];
   for(uint i=0; i< row*col; i++)
     v[i] =A.v[i];
 };

void fullMatrix::swapRows( uint i, uint k )
{ // interchange rows i & k
  if( i==k ) return;
  if(  i>=row || k>=row )
    error( "Matrix::swapRows: index out of range");

  fullMatrix& A = *this;
  double tem;
  uint j;
  for( j=0; j<col; j++ ){
    tem = A(i,j);
    A(i,j) = A(k,j);
    A(k,j) = tem;
  }
}

fullMatrix operator+(const fullMatrix& A, const fullMatrix& B){ // add two Matrices
 if (A.row!=B.row || A.col!=B.col)error("fullMatrix+: incomatable sizes \n");
 fullMatrix C(A.row, A.col);
 uint i,rc=A.row*A.col;
 for(i=0; i<rc; i++)
    C.v[i] = A.v[i] + B.v[i];
 return C;
}

fullMatrix operator-(const fullMatrix& A, const fullMatrix& B){
 // fullMatrix subtraction
 if (A.row!=B.row || A.col!=B.col)error("fullMatrix-: incomatable sizes \n");
 fullMatrix C(A.row, A.col);
 uint i,rc=A.row*A.col;
 for(i=0; i<rc; i++)
    C.v[i]=A.v[i] - B.v[i];
 return C;
}

fullMatrix operator*(const fullMatrix& A, const fullMatrix& B){
 // fullMatrix multiplication
 if (A.col!=B.row)error("fullMatrix*: incomatable sizes \n");
 fullMatrix C(A.row, B.col);
 uint i,j,k;
 for(i=0; i<A.row; i++)
    for(j=0; j<B.col; j++)
       for(k=0; k<A.col; k++)
         C(i,j) += A(i,k) * B(k,j);
 return C;
}

fullMatrix operator*( double a, const fullMatrix& B){
 // a*B
 fullMatrix C( B);
 uint i,s=C.row*C.col;
 for(i=0; i<s; i++)
   C.v[i] *=a;
 return C;
}

fullMatrix operator*(const fullMatrix& B, double a){
  return a*B;
}

double operator,(const fullMatrix& A, const fullMatrix& B)
{ // (A:B)
  double sum = 0.0;
  uint rc = A.row*A.col;
  if( rc != B.row*B.col)
    error("Innerproduct size mismatch");
  for( uint i=0; i<rc; i++ )
    sum += A.v[i]*B.v[i];
  return sum;
}

fullMatrix pow( const fullMatrix & A, uint n )
{// raise A to the power n
  assert( A.col == A.row );
  if( n == 0 )
    return eye(A.col);
  if( n == 1 ) 
    return A;
  fullMatrix B(pow(A,n/2));
  if( n%2 )
    return B*B*A;
  return B*B;
}

ostream& dump(ostream& os, const fullMatrix& A){
 // output for small matrices
 uint i, j;
 os << std::endl;
 for(i=0; i<A.row; i++){
    for(j=0; j<A.col; j++)
      os << std::setw(8) <<A(i,j) << " ";
    os << std::endl;
   }
 return os;
}

ostream& operator<<(ostream& os, const fullMatrix& A){
 // output for small matrices
  return dump(os, A);
}

fullMatrix eye( uint n )
{ // identity
  fullMatrix I(n,n);
  uint i;
  for(i=0; i<n; i++ )
    I(i,i) = 1.0;
  return I;
}

fullMatrix transpose (const fullMatrix& A){
 fullMatrix C(A.col, A.row);
 uint i,j;
 for(i=0; i<A.row; i++)
    for(j=0; j<A.col; j++)
      C(j,i) = A(i,j);
    return C;   
}

void solve( const fullMatrix & A, 
              fullMatrix& X, 
              const fullMatrix& Y )
{ // find X s.t. A X = Y using Gausian Elimination with
  // row pivoting -- no scaling this version

  if( A.row != A.col ){
      std::cerr << "solve only works for square matrices\n";
      return;
  }

  int i, j, k, n = A.col, m=Y.col;
  double s, pa;

  // initialize X to Y and copy A
  X = Y;
  fullMatrix  B(A);
  // could scale here

  // next do forward elimination and forward solution
  for( i=0; i<n; i++ ){ // clean up the i-th column

    // find the pivot
    k = i;  // row with the pivot
    pa = std::abs(B(i,i));
    for( j=i+1; j<n; j++ )
      if( std::abs(B(j,i)) > pa ){
        pa = std::abs(B(j,i));
        k = j;
      }
    if( pa == 0.0 )
      error( "solve: singular matrix");
    B.swapRows(i,k);
    X.swapRows(i,k); // the pivot is now in B(i,i)

    for( j=i+1; j<n; j++ ){ // for each row below row i
      s = B(j,i)/B(i,i);
      for( k=i+1; k<n; k++ ) // modifications that make B(j,i) zero
        B(j,k) -= s*B(i,k);
      for( k=0; k<m; k++ )   // change the rhs too
        X(j,k) -= s*X(i,k);
    }
  }
  // now B is upper triangular

  for( i=n-1; i>=0; i-- ){ // do back solution
    for(j = i+1; j<n; j++ ){
      for( k=0; k<m; k++ )
        X(i,k) -= B(i,j)*X(j,k);
    }
    for( k=0; k<m; k++ )
      X(i,k) /= B(i,i);
  }
}

fullMatrix abs( const fullMatrix& A )
{ // abs each entry
  fullMatrix B(A);
  uint rc = A.row*A.col;
  for( uint i=0; i<rc; i++ )
    B.v[i] = std::abs(B.v[i]);
  return B;
}

fullMatrix& operator*=( fullMatrix& A, const fullMatrix& B)
{// A = A*B -- can change the shape of  A

  fullMatrix C(A*B);
  uint i, n=C.row*C.col;
  if( A.row*A.col != n ){
    delete [] A.v;
    A.v = new double[C.row*C.col];
  }
  for(i=0; i<n; i++ )
    A.v[i] = C.v[i];
  return A;
}
