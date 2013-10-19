#include <stdio.h>

int queenDFS(int col, int rows[]);

main()
{
	int i, rows[8];
	
	// for each row excluding 2 and 7, put a queen in column 0,
	// row i and then run queenDFS to find where the queen in
	// column 1 can go
	for(i=0; i < 7; i++){
		if(i != 2){
			rows[0] = i;
			queenDFS(1,rows);
		}
	}
}

int queenDFS(int col, int rows[])
{
	int i, j, k;

	// if we've filled the array, print out the results
	if(col == 8){
		for(k=0; k < 8; k++)
			printf("%d ", rows[k]);
		printf("\n");
		return;
	}

	// i represents the possible rows to put a queen in column col
	// j represents the columns already placed
	// we cycle through all possible rows (i) and see if we can
	// put a queen in column col, row i
	for(i=0; i < 8; i++){
		for(j=0; j < col; j++){
			// these are the conditions which will make a bad board
			// two queens are on the same diagonal if the slope of the
			// line they share is plus or minus 1
			// multiply and square each side to get an integer comparison
			if(rows[j] == i
				|| (rows[j] - i)*(rows[j] - i) == (j - col)*(j - col)
				|| col + i == 2 || col + i == 7 || col + i == 10)
				break;
			// if we've checked all the columns then add the queen
			// and find the next one
			else if(j == col - 1){
				rows[col] = i;
				queenDFS(col+1,rows);
			}
		}
	}
}
