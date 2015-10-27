/***********************************************************************

            	    		B I N A R Y   T R E E S   T E S T S

***********************************************************************/

#include "Binary_Tree.c"
#include <stdio.h>
#include <stdlib.h>

void printer(void *data) {
  printf("%d\n", *(int *)data);
}

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

void visitor(tr_node *node) {
  printf("%d\n", *(int *)node->key);
}

void ptr_visitor(void *key) {
  printf("%p\n", key);
}

void make_key(void *key) {
  int key_val = *(int *)key;
  key_val++;
  *(int *)key = key_val;
}

int main() {

  Tree *T = tr_make(comparator, printer);

  int *a = malloc(sizeof(int));
  *a = 1;
  int *b =  malloc(sizeof(int));
  *b = 2;
  int *c =  malloc(sizeof(int));
  *c = 3;
  int *d = malloc(sizeof(int));
  *d = 4;
  int *e =  malloc(sizeof(int));
  *e = 5;
  int *f =  malloc(sizeof(int));
  *f = 6;
  int *g = malloc(sizeof(int));
  *g = 7;
  int *h = malloc(sizeof(int));
  *h = 8;
  int *i = malloc(sizeof(int));
  *i = 9;
  int *j = malloc(sizeof(int));
  *j = 10;
  int *k = malloc(sizeof(int));
  *k = 11;
  int *l = malloc(sizeof(int));
  *l = 12;
  int *m = malloc(sizeof(int));
  *m = 13;
  int *n = malloc(sizeof(int));
  *n = 14;
  int *o = malloc(sizeof(int));
  *o = 15;

  tr_insert(
      tr_insert(
        tr_insert(
          tr_insert(
            tr_insert(
              tr_insert(
                tr_insert(
                  tr_insert(
                    tr_insert(
                      tr_insert(
                        tr_insert(
                          tr_insert(
                            tr_insert(
                              tr_insert(
                                tr_insert(T, h),
                                d),
                              l),
                            b),
                          a),
                        c),
                      e),
                    f),
                  g),
                k),
              j),
            i),
          n),
        m),
      o);

  printf("breath first search of tree with visitor being the printer\n");
  tr_breadth_first(T, visitor);
  printf("pre-order walk of tree with visitor being the printer\n");
  tr_walk(T, PREORDER, T->printer);
  printf("in-order walk of tree with visitor being the printer\n");
  tr_walk(T, INORDER, T->printer);
  printf("post-order walk of tree with visitor being the printer\n");
  tr_walk(T, POSTORDER, T->printer);
  printf("doing a tree walk to mutate keys\n");
  tr_walk(T, INORDER, make_key);
  tr_walk(T, INORDER, T->printer);
  printf("tr max -> %d\n", *(int *)tr_maximum(T->root));
  printf("tr min -> %d\n", *(int *)tr_minimum(T->root));
  printf("tr height -> %d\n", tr_height(T->root));
  printf("tr empty? -> %d\n", tr_is_empty(T));
  printf("post-order walk of tree with visitor being the ptr_visitor\n");
  tr_walk(T, POSTORDER, ptr_visitor);
  printf("attempting to free tr\n");
  tr_free(T, 1); //since we malloc all our data we can tell the tree to delete the keys aswell (1)
  printf("attempt succesful\n");
  return 0;
}

