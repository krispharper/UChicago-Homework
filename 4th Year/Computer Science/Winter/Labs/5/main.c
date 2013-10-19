/* tests of binary search tree tools */

#include <stdio.h>
#include "binarytree.h"
#include "binarytree.c"


int main()
{

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

  BTNP tree2=0;
  insertBST(2,'A',&tree2);
  insertBST(1,'B',&tree2);
  insertBST(3,'C',&tree2);

  printf("\n\nThe weight and height of tree1  are %d and %d.\n",
         weightBT(tree1), heightBT(tree1));

  printf("tree1 preorder is ");
  print_preorder(tree1);
  printf("\ntree1 inorder is ");
  print_inorder(tree1);
  printf("\ntree1 postorder is ");
  print_postorder(tree1);

  rebalanceBST ( &tree1 );

  printf("\n\nAfter rebalancing tree1");
  printf("\n\nThe weight and height of tree1  are %d and %d.\n",
         weightBT(tree1), heightBT(tree1));

  printf("tree1 preorder is ");
  print_preorder(tree1);
  printf("\ntree1 inorder is ");
  print_inorder(tree1);
  printf("\ntree1 postorder is ");
  print_postorder(tree1);
  printf("\n\n");

  return 0;
}