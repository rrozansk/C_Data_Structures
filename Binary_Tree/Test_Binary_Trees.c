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
  //printf("parent->%d\t\tme: %d\n", *(int *)node->parent, *(int *)node->key);
}

int main() {

  Tree *T = tr_make(comparator, printer);

  int a = 4;
  int b = 2;
  int c = 6;
  int d = 1;
  int e = 3;
  int f = 5;
  int g = 7;
  int h = 8;

  tr_insert(
      tr_insert(
        tr_insert(
          tr_insert(
            tr_insert(
              tr_insert(
                tr_insert(
                  tr_insert(T, &e), 
                  &c), 
                &d), 
              &f), 
            &a), 
          &h), 
        &g),
      &b);

  printf("pre-order walk of tree with visitor being the printer\n");
  tr_walk_pre(T, T->printer);
  printf("breath first search of tree with visitor being the printer\n");
  T->size = 7;
  tr_breadth_first(T, visitor);

  return 0;
}

