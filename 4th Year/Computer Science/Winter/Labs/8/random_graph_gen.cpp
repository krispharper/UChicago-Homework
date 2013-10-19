#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/* Generates a random graph on n vertices
 * The first parameter is the number of vertices
 * The second paramter is the probability with which to choose an edge
 * If no paramters are specified, the number of vertices is 10, and the probability 0.5
 */
int main(int argv, char *argc[]){
  int n, i, j;
  double p;

  srandom(time(0));
    
  switch(argv){
  case 1:
    n = 10;
    p = 0.5;
    break;
  case 2:
    n = atoi(argc[1]);
    p = 0.5;
    break;
  case 3:
    n = atoi(argc[1]);
    p = atof(argc[2]);
    break;
  default:
    cout<<"Incorrect usage of parameters"<<endl;
  };

  cout<<n<<endl;
  for(i = 0; i<n; i++)
    for(j = i+1; j<n; j++){
      if( random() % 10000< p * 10000)
	cout<<i<<" "<<j<<endl;
      }
  cout<<"-1 -1"<<endl;
}

