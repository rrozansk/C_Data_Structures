/***********************************************************************

            	    		L I N K E D   L I S T   T E S T S

***********************************************************************/

#include "Linked_List.c"
#include <stdio.h>
#include <stdlib.h>

#define cons(x, ls) ls_insert_beginning(ls, x)
#define snoc(x, ls) ls_insert_end(ls, x)
#define car(ls) ls_ref(ls, 0)

List *cdr(List *L) {
  return L = ls_remove(L, ls_ref(L, 0));
}

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

  List *ls = make_ls(printer, comparator);
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

  ls = cons(val1, cons(val2, cons(val3, cons(val4, cons(val5, cons(val6, snoc(val8, snoc(val7, snoc(val6, ls)))))))));
  ls_print(ls);
  List *A = ls_append(ls, ls);
  List *C = ls_copy(A);
  free_ls(A);
  ls_sort(C);
  printf("sorted C\n");
  ls_print(C);
  printf("car -> %d\n", *(int *)car(ls));
  printf("list reverse\n");
  ls_reverse(ls);
  ls_print(ls);
  printf("sort\n");
  ls_sort(ls);
  ls_print(ls);
  printf("size of ls bf %d cdr's -> %d\n", 2, ls->size);
  cdr(cdr(ls)); 
  ls_print(ls);
  printf("size of ls -> %d\n", ls->size);
  ls_set(ls, 0, val3);
  ls_print(ls);
  printf("car: %d\n", *(int *)car(ls));

  //clean up
  delete_ls(ls);
  free(ls);
  ls = NULL;

  return 0;
}
