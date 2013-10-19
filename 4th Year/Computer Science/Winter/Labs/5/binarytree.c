// Kris Harper
// kharper
// CMSC 15200
// Lab 2
// 2.1.11

#include "binarytree.h"


BTNP make_BTN( int key_value, char n,
               BTNP l_child,
               BTNP r_child)
{ /* set up a new BTN */

  BTNP p;
  if((p=(BTNP)malloc( sizeof(BTN))) ){
    p->key   = key_value;
    p->left  = l_child;
    p->right = r_child;
    p->name  = n;
  }
  return p;
}

void printBTN( BTNP p)
{ /* print node name */
  printf( "%c", p->name);
}


/* height of binary tree rooted at *btnptr */
/* height of empty tree (btnptr==0) is 0 */
int heightBT( BTNP btnptr )
{
  if( ! btnptr ) 
    return 0;

  int height_l = heightBT( btnptr->left );
  int height_r = heightBT( btnptr->right );
  return 1 + ((height_l>height_r) ? height_l : height_r);
}

/* weight of binary tree rooted at *btnptr */
/* weight of empty tree (btnptr==0) is 0 */
int weightBT( BTNP btnptr )
{
  if( ! btnptr ) 
    return 0;

  return 1 +  weightBT( btnptr->left ) + weightBT( btnptr->right );
}

/* visit the nodes of a binary tree and print them */
void print_preorder( BTNP btnptr )
{
  if(!btnptr) return;

  printBTN(btnptr);
  print_preorder(btnptr->left);
  print_preorder(btnptr->right);
}

void print_inorder( BTNP btnptr )
{
  if(!btnptr) return;

  print_inorder(btnptr->left);
  printBTN(btnptr);
  print_inorder(btnptr->right);
}

void print_postorder( BTNP btnptr )
{
  if(  !btnptr ) return;

  print_postorder(btnptr->left);
  print_postorder(btnptr->right);
  printBTN(btnptr);
}

/* binary search tree tools, keys should be unique */

BTNP findBST( int key, BTNP root )
{ /* find returns pointer to node containing key, if it */
  /* exists, 0 otherwise */
  if( !root || root->key == key )
    return root;
  if( key < root->key )
    return findBST( key, root->left );
  else /* key > root->key case */
    return findBST( key, root->right );
}


BTNP insertBST( int key, char c ,BTNP *rootp)
{ /* insertBST changes the data if the node is there */
  /* it inserts it if it is not there */
  /* in either case it returns a pointer to the node */
  /* that was changed or inserted. */

  /* note that the pointer at location rootp, */
  /* i.e., *rootp, will change if a new node is added.*/

  assert( rootp ); /* cannot be called with rootp == 0 */

  BTNP root = *rootp;
  if( !root ) /* empty tree case */
    return  *rootp = make_BTN(key,c,0,0);

  if( key == root->key ){
    root->name = c;
    return root;
  }
  if(key > root->key)
    return insertBST( key, c, & root->right );
  else /* key  < root->key */
    return insertBST( key, c, & root->left );
}

/* rebalance the binary tree */
/* create an array for pointers to the nodes */
/* fill the array using place_inorder */
/* rebuild the tree using rebuildBST */
void rebalanceBST(BTNP *rootp)
{
  BTNP *nodes = malloc(weightBT(*rootp)*sizeof(BTNP));
  int index = 0, i;
  place_inorder(&index, *rootp, nodes);
  rebuildBST(rootp, nodes, 0, weightBT(*rootp)-1);
}

/* place pointers to the tree nodes in an array, inorder */
/* index should point to an int that is 0 when the function is first called */
/* *index should be updated to the next position in the array */
/* as the array is being filled */
void place_inorder(int *index, BTNP btnptr, BTNP tree_array[])
{
  if(!btnptr)
    return;
  place_inorder(index, btnptr->left, tree_array);
  tree_array[(*index)++] = btnptr;
  place_inorder(index, btnptr->right, tree_array);
}

/* build a BST by binary search over the array of pointers tree_array */
/* consider the array entries between begin and end (inclusive) */
/* *rootp should be changed to be a pointer to the root of the tree */
void rebuildBST(BTNP *rootp, BTNP tree_array[], int begin, int end)
{
  int middle = begin+(end-begin)/2;

  if(end - begin == 0){
    (tree_array[begin])->left = (BTNP) 0;
    (tree_array[begin])->right = (BTNP) 0;
    *rootp = tree_array[begin];
    return;
  }
  else if(end - begin == 1){
    (tree_array[begin])->left = (BTNP) 0;
    (tree_array[begin])->right = tree_array[end];
    *rootp = tree_array[begin];
    rebuildBST(&(tree_array[begin]->right), tree_array, end, end);
    return;
  }
  else {
    rebuildBST(&(tree_array[middle]->left), tree_array, begin, middle-1);
    rebuildBST(&(tree_array[middle]->right), tree_array, middle+1, end);
    *rootp = tree_array[middle];
    return;
  }
}