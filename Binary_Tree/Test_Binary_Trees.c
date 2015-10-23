/***********************************************************************

            	    		B I N A R Y   T R E E S   T E S T S

***********************************************************************/

#include "Binary_Tree.c"
#include <stdio.h>
#include <stdlib.h>

typedef struct S {
  int num;
} S_t;

void printer(void *data) {
  printf("%d\n", ((S_t *)data)->num);
}

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

int main() {

  Tree *T = tr_make(comparator, printer);

  S_t *v1 = malloc(sizeof(S_t));
  v1->num = 1;
  S_t *v2 = malloc(sizeof(S_t));
  v2->num = 2;
  S_t *v3 = malloc(sizeof(S_t));
  v3->num = 3;
  S_t *v4 = malloc(sizeof(S_t));
  v4->num = 4;
  S_t *v5 = malloc(sizeof(S_t));
  v5->num = 5;
  S_t *v6 = malloc(sizeof(S_t));
  v6->num = 6;
  S_t *v7 = malloc(sizeof(S_t));
  v7->num = 7;
  S_t *v8 = malloc(sizeof(S_t));
  v8->num = 8;

  T->root = (void *)v4;
  T->root->left = (void *)v2;
  T->root->right = (void *)v6;
  T->root->left->left = (void *)v1;
  T->root->left->right = (void *)v3;
  T->root->right->left = (void *)v5;
  T->root->right->right = (void *)v7;

  T->root->parent = NULL;
  T->root->left->parent = T->root;
  T->root->right->parent = T->root;
  T->root->left->left->parent = T->root->left;
  T->root->left->right->parent = T->root->left;
  T->root->right->left->parent = T->root->right;
  T->root->right->right->parent = T->root->right;

  printf("trying to print tree manually\n");
  T->printer(T->root);
  T->printer(T->root->left);
  T->printer(T->root->left->left);
  T->printer(T->root->left->right);
  T->printer(T->root->right);
  T->printer(T->root->right->left);
  T->printer(T->root->right->right);

  T->size = 7;
//  tr_walk_pre(T, T->printer);
  tr_breadth_first(T, T->printer);

  return 0;
}

