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

void make_key(void *key) {
  int key_val = *(int *)key;
  key_val++;
  *(int *)key = key_val;
}

int main() {

  Tree *T = tr_make(comparator, printer);

  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  int f = 6;
  int g = 7;
  int h = 8;
  int i = 9;
  int j = 10;
  int k = 11;
  int l = 12;
  int m = 13;
  int n = 14;
  int o = 15;

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
                                tr_insert(T, &h),
                                &d),
                              &l),
                            &b),
                          &a),
                        &c),
                      &e),
                    &f),
                  &g),
                &k),
              &j),
            &i),
          &n),
        &m),
      &o);

  printf("breath first search of tree with visitor being the printer\n");
  tr_breadth_first(T, visitor);
  printf("pre-order walk of tree with visitor being the printer\n");
  tr_walk(T, -1, T->printer);
  printf("in-order walk of tree with visitor being the printer\n");
  tr_walk(T, 0, T->printer);
  printf("post-order walk of tree with visitor being the printer\n");
  tr_walk(T, 1, T->printer);
  printf("doing a tree walk to mutate keys\n");
  tr_walk(T, 0, make_key);
  tr_walk(T, 0, T->printer);
  printf("tr max -> %d\n", *(int *)tr_maximum(T->root));
  printf("tr min -> %d\n", *(int *)tr_minimum(T->root));
  printf("tr height -> %d\n", tr_height(T->root));
  printf("tr empty? -> %d\n", tr_is_empty(T));
  printf("attempting to free tr\n");
  tr_free(T);
  printf("attempt succesful\n");
  return 0;
}

