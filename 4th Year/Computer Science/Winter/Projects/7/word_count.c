#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define BUFFSIZE 100

// a character buffer and free position for ungetch
char buf[BUFFSIZE];
int bufp = 0;

// binary tree node structure
struct tnode {
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};

struct tnode *addtree(struct tnode *p, char *w);
void treeprint(struct tnode *p);
int getword(char *, int);
struct tnode *talloc(void);
int k_getch(void);
void k_ungetch(int c);

// word frequency counter
main()
{
	struct tnode *root;
	char word[MAXWORD];

	root = NULL;
	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			root = addtree(root, word);
	treeprint(root);
	return 0;
}

// add a node with word w at or below a node p
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL){
		p = talloc();
		p->word = strdup(w);
		p->count = 1;
		p->left = p->right = NULL;
	}
	else if((cond = strcmp(w, p->word)) == 0)
		p->count++;
	else if(cond < 0)
		p->left = addtree(p->left, w);
	else
		p->right = addtree(p->right, w);
	return p;
}

// print a tree in order
void treeprint(struct tnode *p)
{
	if(p != NULL){
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}
}

// make space for a tnode
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

// get the next word from the input
int getword(char *word, int lim)
{
	int c, k_getch(void);
	void k_ungetch(int);
	char *w = word;

	while(isspace(c = k_getch()))
		;
	if(c != EOF)
		*w++ = c;
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for(; --lim > 0; w++)
		if(!isalnum(*w = k_getch())){
			k_ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}

// get a character
int k_getch(void)
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

// push a character back onto the input
void k_ungetch(int c)
{
	if (bufp >= BUFFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
}
