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

/* free a binary tree */
void freeBT(BTNP root)
{
  if(root == 0)
    return;
  else {
    freeBT(root->left);
    freeBT(root->right);
    free(root);
  }
}

/* copy a binary tree */
/* does not deal with insufficient memory case */
BTNP copyBT(BTNP root)
{
  if(root == (BTNP) 0)
    return (BTNP) 0;
  else
    make_BTN(root->key, root->name, copyBT(root->left), copyBT(root->right));
}

/* delete the node in the a BST containing key */
/* return true iff the key was found */
int deleteBST(int key, BTNP *rootp)
{
	if(*rootp == 0)
		return 0;
	else if((*rootp)->key == key){
		*rootp = (BTNP) 0;
		return 1;
	}
	else if((*rootp)->left != (BTNP) 0 && key < ((*rootp)->left)->key)
		return deleteBST(key, &((*rootp)->left));
	else if((*rootp)->right != (BTNP) 0  && key > ((*rootp)->right)->key)
		return deleteBST(key, &((*rootp)->right));
	else if((*rootp)->left != (BTNP) 0 && key == ((*rootp)->left)->key){	
		if(((*rootp)->left)->left == (BTNP) 0 && ((*rootp)->left)->right == (BTNP) 0){
			(*rootp)->left = (BTNP) 0;
			return 1;
		}
		else if(((*rootp)->left)->right == (BTNP) 0){
			(*rootp)->left = ((*rootp)->left)->left;
			return 1;
		}
		else if(((*rootp)->left)->left == (BTNP) 0){
			(*rootp)->left = ((*rootp)->left)->right;
			return 1;
		}
		else {
			BTNP *maxp = ptr2ptr2max(&((*rootp)->left));
			((*rootp)->left)->name = (*maxp)->name;
			deleteBST((*maxp)->key, rootp);
			return 1;
		}
	}
	else if((*rootp)->right != (BTNP) 0 && key == ((*rootp)->right)->key){
		if(((*rootp)->right)->left == (BTNP) 0 && ((*rootp)->right)->right == (BTNP) 0){
			(*rootp)->right = (BTNP) 0;
			return 1;
		}
		else if(((*rootp)->right)->right == (BTNP) 0){
			(*rootp)->right = ((*rootp)->right)->left;
			return 1;
		}
		else if(((*rootp)->right)->left == (BTNP) 0){
			(*rootp)->right = ((*rootp)->right)->right;
			return 1;
		}
		else {
			BTNP *maxp = ptr2ptr2max(&((*rootp)->right));
			((*rootp)->right)->name = (*maxp)->name;
			deleteBST((*maxp)->key, rootp);
			return 1;
		}
	}
}

/* helper for deleteBST */
/* return the address of the pointer to max element in BST */
/* don't call on empty tree */
BTNP *ptr2ptr2max(BTNP *rootp)
{
  if((*rootp)->right == (BTNP) 0)
    return rootp;
  else
    return ptr2ptr2max(&(*rootp)->right);
}

/* check to see if the keys are in proper order */
int isBST(BTNP root)
{
  if(root->left == (BTNP) 0 && root->right == (BTNP) 0)
    return 1;
  else if(root->left == 0 && root->key < (root->right)->key)
    return isBST(root->right);
  else if(root->right == 0 && root->key > (root->left)->key)
    return isBST(root->left);
  else if(root->key > (root->left)->key && root->key < (root->right)->key)
    return isBST(root->left) && isBST(root->right);
  else
    return 0;
}

/* helper function for isBST */
int ckBST(BTNP root, BTNP *prevp);
