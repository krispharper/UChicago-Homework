// Kris Harper
// kharper
// CMSC 15200
// Lab 8
// 2.22.11

#include <vector>
#include <iostream>

using namespace std;
int DFS(vector<int> a[], bool v[], int start);

// a depth first search program to count the connected componenets of a graph
main()
{
	int size, i, j, count=0;
	cin>>size;
	// make arrays for the adjacent nodes and the visited nodes
	vector<int> a[size];
	bool v[size];
	
	// initialize the visited nodes array to 0
	for(i=0; i<size; i++)
		v[i] = 0;

	// continually add each neighbor node to the neighbor array
	while(1){
		cin>>i;
		if(i == -1)
			break;
		cin>>j;
		a[j].push_back(i);
		a[i].push_back(j);
	}
	// call DFS on each unvisited node, increasing count each time
	for(i=0; i<size; i++){
		if(v[i] == 0){
			DFS(a, v, i);
			count++;
		}
	}
	cout<<count<<"\n";
}

// a depth first search which visits every node in a connected component starting at start
int DFS(vector<int> a[], bool v[], int start)
{
	int i;
	// visit each unvisited neighbor node of start and follow any paths
	// by recursively calling DFS on those neighbors
	for(i=0; i < a[start].size(); i++){
		if(v[a[start][i]] == 0){
			v[a[start][i]] = 1;
			DFS(a, v, a[start][i]);
		}
	}
}
