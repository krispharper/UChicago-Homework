/* tests of binary search tree tools */

#include <stdio.h>
#include "binarytree.h"
#include "EC3.c"


int main()
{
  /*  tree0 */
  /*          M         */
  /*         /  \       */
  /*        /    \      */
  /*       A      K     */
  /*      / \    / \    */
  /*     C   E F    G   */
  /*        /   \       */
  /*       N     P      */
  BTN N = {1,'N',(BTNP)0,(BTNP)0};
  BTN P = {2,'P',(BTNP)0,(BTNP)0};
  BTN C = {3,'C',(BTNP)0,(BTNP)0};
  BTN G = {4,'G',(BTNP)0,(BTNP)0};
  BTN E = {5,'E',&N,(BTNP)0};
  BTN F = {6,'F',(BTNP)0,&P};
  BTN A = {7,'A',&C,&E};
  BTN K = {8,'K',&F,&G};
  BTN M = {9,'M',&A,&K};
  BTNP tree0 = &M;
  /* tree0 is not a BST */

  printf("The weight and height of tree0  are %d and %d.\n",
       weightBT(tree0), heightBT(tree0));

  printf("tree0 preorder is ");
  print_preorder(tree0);
  printf("\ntree0 inorder is ");
  print_inorder(tree0);
  printf("\ntree0 postorder is ");
  print_postorder(tree0);
  printf("\n");
  if( isBST(tree0) )
    printf("tree0 is a BST\n");
  else
    printf("tree0 is not a BST\n");

  BTNP tree0copy = copyBT(tree0);
  printf("The weight and height of tree0copy  are %d and %d.\n",
       weightBT(tree0copy), heightBT(tree0copy));

  printf("tree0copy preorder is ");
  print_preorder(tree0copy);
  printf("\ntree0copy inorder is ");
  print_inorder(tree0copy);
  printf("\ntree0copy postorder is ");
  print_postorder(tree0copy);
  printf("\n");
  if( isBST(tree0copy) )
    printf("tree0copy is a BST\n");
  else
    printf("tree0copy is not a BST\n");

  freeBT(tree0copy);

  BTNP tree1=0;
  insertBST(4,'G',&tree1);
  insertBST(5,'E',&tree1);
  insertBST(3,'C',&tree1);
  insertBST(2,'P',&tree1);
  insertBST(7,'A',&tree1);
  insertBST(8,'K',&tree1);
  insertBST(1,'N',&tree1);
  insertBST(9,'M',&tree1);
  insertBST(6,'F',&tree1);

  printf("The weight and height of tree1  are %d and %d.\n",
         weightBT(tree1), heightBT(tree1));

  printf("tree1 preorder is ");
  print_preorder(tree1);
  printf("\ntree1 inorder is ");
  print_inorder(tree1);
  printf("\ntree1 postorder is ");
  print_postorder(tree1);
  printf("\n");
  if( isBST(tree1) )
    printf("tree1 is a BST\n");
  else
    printf("tree1 is not a BST\n");

  BTNP np;
  int i;
  for( i=0; i<11; i++){
    np = findBST(i,tree1);
    printf( "  key %2d",i) ;
    if( np ){
      printf( " has name "); 
      printBTN(np);
      printf("\n");;
    }
    else 
      printf(" is not in tree1.\n");
  }

  /*  findBST(4,tree1)->name = 'b';*/
  insertBST(4,'b', &tree1);
  printf("After changing the name of key 4\n");
  for( i=0; i<11; i++){
    np = findBST(i,tree1);
    printf( "  key %2d",i) ;
    if( np ){
      printf( " has name "); 
      printBTN(np);
      printf("\n");;
    }
    else 
      printf(" is not in tree1.\n");
  }
  if( isBST(tree1) )
    printf("tree1 is a BST\n");
  else
    printf("tree1 is not a BST\n");

  deleteBST(7,&tree1);
  printf("After deleteing {7,A}\n");
  for( i=0; i<11; i++){
    np = findBST(i,tree1);
    printf( "  key %2d",i) ;
    if( np ){
      printf( " has name "); 
      printBTN(np);
      printf("\n");;
    }
    else 
      printf(" is not in tree1.\n");
  }
  if( isBST(tree1) )
    printf("tree1 is a BST\n");
  else
    printf("tree1 is not a BST\n");

  return 0;
}
