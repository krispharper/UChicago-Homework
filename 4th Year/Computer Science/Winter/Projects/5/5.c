#include "binarytree.h"

#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )

BTNP make_BTN(int key_value, char n, BTNP l_child, BTNP r_child)
{
  BTN temp = {key_value, n, l_child, r_child};
  BTNP tempp = (BTNP) malloc(sizeof(BTN));
  *tempp = temp;
  return tempp;
}

// print node name
void printBTN(BTNP p)
{
  if(p == (BTNP) 0)
    printf("no node");
  else
    printf("%c\n", p->name);
}

// height of binary tree rooted at *btnptr
// height of empty tree (btnptr==0) is 0
int heightBT(BTNP btnptr)
{ 
  if(btnptr == (BTNP) 0)
    return 0;
  else 
    return max(1+heightBT(btnptr->left), 1+heightBT(btnptr->right));
}

// weight of binary tree rooted at *btnptr
// weight of empty tree (btnptr==0) is 0
int weightBT(BTNP btnptr)
{
//  static int weight = 0;
  
  if(btnptr == (BTNP) 0)
    return 0;
  else
    return 1 + weightBT(btnptr->left) + weightBT(btnptr->right);
}

// visit the nodes of a binary tree and print the node.
void print_preorder(BTNP btnptr)
{
  if(btnptr == (BTNP) 0)
    printf("");
  else {
    printf("%c ", btnptr->name);
    print_preorder(btnptr->left);
    print_preorder(btnptr->right);
  }
}

void print_inorder(BTNP btnptr)
{
  if(btnptr == (BTNP) 0)
    printf("");
  else {
    print_inorder(btnptr->left);
    printf("%c ", btnptr->name);
    print_inorder(btnptr->right);
  }
}

void print_postorder( BTNP btnptr )
{
  if(btnptr == (BTNP) 0)
    printf("");
  else {
    print_postorder(btnptr->left);
    print_postorder(btnptr->right);
    printf("%c ", btnptr->name);
  }
}

// binary search tree (BST) tools, keys should be unique

// find returns pointer to node containing key, if it
// exists, 0 otherwise
BTNP findBST(int key, BTNP root)
{
  if(root == (BTNP) 0)
    return (BTNP) 0;
  else if(root->key == key)
    return root;
  else if(root->key > key)
    return findBST(key, root->left);
  else
    return findBST(key, root->right);
}

// insert changes the data if the node is there
// it inserts it if it is not there
// in either case it returns a pointer to the node
BTNP insertBST(int key, char c ,BTNP *rootp)
{
  BTNP *newrootp;

  if(*rootp == (BTNP) 0)
    return *rootp = make_BTN(key, c, (BTNP) 0, (BTNP) 0);
  else if(key == (*rootp)->key){
    (*rootp)->name = c;
    return *rootp;
  }
  else if((*rootp)->key > key){
    newrootp = &(*rootp)->left;
    return insertBST(key, c, newrootp);
  }
  else {
    newrootp = &(*rootp)->right;
    return insertBST(key, c, newrootp);
  }
}