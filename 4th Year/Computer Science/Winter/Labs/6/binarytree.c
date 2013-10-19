/* implementation of binary tree tools */
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
void rebalanceBST( BTNP *rootp ){
  BTNP * tree_array;
  int index = 0;
  int *i = &index;
  int weight = weightBT(*rootp);

  /* allocate the memory and check for errors */
  tree_array = (BTNP *)calloc(weight, sizeof(BTNP));
  
  if(!tree_array){
    printf("Error allocating memory\n");
    exit(1);
  }

  /* place the tree in tree_array, in order */
  place_inorder( i, *rootp, tree_array );

  /* rebuild a balanced tree */
  rebuildBST( rootp, tree_array, 0, weight-1);
  
  /* free the array */
  free(tree_array);

}

/* place pointers to the nodes of the tree in an array, in order */
void place_inorder( int *index, BTNP btnptr, BTNP tree_array[] ){
  if (! btnptr)
    return;
  place_inorder( index, btnptr->left, tree_array);
  tree_array[(*index)++] = btnptr;
  place_inorder( index, btnptr->right, tree_array);
}

/* rebuild a balanced BST from the (sorted by key) array tree_array */
void rebuildBST( BTNP *rootp, BTNP tree_array[], int begin, int end){

  /* base case */
  if(begin>end){
    *rootp = NULL;
    return;
  }

  int mid = (end + begin)/2;

  /* set the left and right pointers of the root of this section */
  rebuildBST( &(tree_array[mid]->left), tree_array, begin, mid-1 );
  rebuildBST( &(tree_array[mid]->right), tree_array, mid+1, end );

  /* set *rootp (the root) to the middle of this section of the array */
  *rootp = tree_array[mid];
  
}
