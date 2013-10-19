/* interface for binary tree tools */
#ifndef BINARY_TREE_H
#define  BINARY_TREE_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


struct binaryTreeNode{
  int key;      /* search key for test examples */
  char name;    /* tiny data for testing tools  */
  struct binaryTreeNode *left;
  struct binaryTreeNode *right;
};
typedef struct binaryTreeNode BTN;
typedef struct binaryTreeNode *BTNP;

BTNP make_BTN( int key_value, char n,
               BTNP l_child,
               BTNP r_child);

void printBTN( BTNP p); /* print node name */


/* height of binary tree rooted at *btnptr */
/* height of empty tree (btnptr==0) is 0 */
int heightBT( BTNP btnptr );

/* weight of binary tree rooted at *btnptr */
/* weight of empty tree (btnptr==0) is 0 */
int weightBT( BTNP btnptr );

/* visit the nodes of a binary tree and print the */
/* node. */
void print_preorder( BTNP btnptr );
void print_inorder( BTNP btnptr );
void print_postorder( BTNP btnptr );

/* free a binary tree */
void freeBT( BTNP root );

/* copy a binary tree */
/* does not deal with insufficient memory case */
BTNP copyBT( BTNP root );

/* binary search tree (BST) tools, keys should be unique */

/* find returns pointer to node containing key, if it */
/* exists, 0 otherwise */
BTNP findBST( int key, BTNP root );

/* insertBST changes the data if the node is there */
/* it inserts it if it is not there */
/* in either case it returns a pointer to the node */
/* that was changed or inserted. */
BTNP insertBST( int key, char c ,BTNP *rootp);
/* note that the pointer at location rootp, */
/* i.e., *rootp, will change if a new node is added.*/

/* delete the node in the a BST containing key */
/* return true iff the key was found */
int deleteBST( int key, BTNP *rootp );

/* helper for deleteBST */
/* return the address of the pointer to max element in BST */
/* don't call on empty tree */
BTNP *ptr2ptr2max( BTNP *rootp );

/* check to see if the keys are in proper order */
int isBST( BTNP root );

/* helper function for isBST */
int ckBST( BTNP root, BTNP *prevp );



#endif