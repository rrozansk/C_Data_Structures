/***********************************************************************

            	    		L I N K E D   L I S T   T E S T S

***********************************************************************/

#include "Linked_List.c"
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

void *copy_num(void *data) {
  int *c = malloc(sizeof(int));
  *c = *(int *)data;
  return c;
}

int main() {

  List *ls = ls_make();
  S_t *val1 = malloc(sizeof(S_t));
  val1->num = 5;
  
  S_t *val2 = malloc(sizeof(S_t));
  val2->num = 7;
  
  S_t *val3 = malloc(sizeof(S_t));
  val3->num = 9;
  
  S_t *val4 = malloc(sizeof(S_t));
  val4->num = 14;
  
  S_t *val5 = malloc(sizeof(S_t));
  val5->num = 2;
  
  S_t *val6 = malloc(sizeof(S_t));
  val6->num = 12;
  
  S_t *val7 = malloc(sizeof(S_t));
  val7->num = 6;
  
  S_t *val8 = malloc(sizeof(S_t));
  val8->num = 1;

  ls = ls_cons(ls_cons(ls_cons(ls_cons(ls_cons(ls_cons(ls_cons(ls_cons(ls, val6), val7), val8), val5), val4), val3), val2), val1);
  printf("list\n");
  ls_walk(ls, printer);
  printf("sorted C\n");
  ls_sort(ls, comparator);
  ls_walk(ls, printer);
  printf("list reverse\n");
  ls_reverse(ls);
  ls_walk(ls, printer);
  printf("list mapped\n");
  List *cp = ls_map(ls, copy_num);
  ls_walk(cp, printer);

  //clean up
  ls_free(ls, 1);
  ls = NULL;

  return 0;
}
