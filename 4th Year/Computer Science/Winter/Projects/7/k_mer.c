#include <stdio.h>

// length of a k-mer
#define k 5

main()
{
	char c, k_mer[k];
	int i=0, j;

	while((c = getchar()) != EOF){
		// if the next char is '#' then reset the index i
		// and get the next char
		if(c == '#'){
			i = 0;
			continue;
		}
		// if we've only added less than k chars add c
		if(i < k-1){
			k_mer[i++] = c;
			continue;
		}
		// if we've added k-1 chars, add c and print out
		// the k-mer
		if(i == k-1){
			k_mer[i++] = c;
			for(j=0; j < k; j++)
				printf("%c", k_mer[j]);
			printf("\n");
			continue;
		}
		// if there is already a full array, shift everything down
		// and add c to the last entry, printing as we go
		if(i == k){
			for(j=0; j < k-1; j++){
				k_mer[j] = k_mer[j+1];
				printf("%c", k_mer[j]);
			}
			printf("%c\n", k_mer[i-1] = c);
		}
	}
}
