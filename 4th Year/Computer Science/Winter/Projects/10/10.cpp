#include "sortedVec.h"
#define NODES 20

int main()
{
	uint i, j, min, minNode, minIndex, nv = NODES;
	uint p[NODES];

	// set the permutation to the identity
	for(i=0; i < NODES; i++)
		p[i] = i;

	// create the graph as a vector of sortedVecs and
	// input the graph data
	vector<sortedVec> edgeList(NODES);

	edgeList[0] = sortedVec(1, 5);
	edgeList[4] = sortedVec(3, 9);
	edgeList[15] = sortedVec(10, 16);
	edgeList[19] = sortedVec(14, 18);
	for(i=1; i<=3; i++)
		edgeList[i] = sortedVec(i-1, i+1, i+5);
	for(i=16; i<=18; i++)
		edgeList[i] = sortedVec(i-5, i-1, i+1);
	for(i=5; i<=10; i+=5)
		edgeList[i] = sortedVec(i-5, i+1, i+5);
	for(i=9; i<=14; i+=5)
		edgeList[i] = sortedVec(i-5, i-1, i+5);
	for(i=6; i<=13; i++){
		edgeList[i] = sortedVec(i-5, i-1, i+1, i+5);
		if(i == 8)
			i+=2;
	}

	// print the original graph
	for(i=0; i < edgeList.size(); i++){
		cout << p[i] << ": ";
		for(j=0; j < edgeList[p[i]].size(); j++)
			cout << edgeList[p[i]].v[j] << " ";
		cout << endl;
	}
	// remove nodes while there's one left
	while(nv != 1){
		// find the minimal degree node
		min = edgeList[p[0]].size();
		minNode = p[0];
		minIndex = 0;
		for(i=0; i < nv; i++)
			if(edgeList[p[i]].size() < min){
				min = edgeList[p[i]].size();
				minNode = p[i];
				minIndex = i;
			}
		cout << "Afer removing node " << minNode << ':' << endl;
		// for each node in the edge list of minNode, add the edge list
		// of minNode to the edge list for the current node
		// then remove minNode and the current node from the lists
		for(i=0; i < edgeList[minNode].size(); i++){
			edgeList[edgeList[minNode].v[i]].augment(edgeList[minNode]);
			edgeList[edgeList[minNode].v[i]].remove(minNode);
			edgeList[edgeList[minNode].v[i]].remove(edgeList[minNode].v[i]);
		}
		// swap the indexes to add to the permutation
		swap(p[minIndex], p[nv-1]);
		nv--;
		// print out the graph using the indexes 0 to nv-1
		for(i=0; i < nv; i++){
			cout << p[i] << ": ";
			for(j=0; j < edgeList[p[i]].size(); j++)
				cout << edgeList[p[i]].v[j] << " ";
			cout << endl;
		}
	}
	// print the permutation in reverse (proper) order
	for(i=0; i < NODES; i++)
		cout << p[NODES - 1 - i] << ' ';
	cout << endl;
	return 0;
}
