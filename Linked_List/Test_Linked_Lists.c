/***********************************************************************

            	    		L I N K E D   L I S T   T E S T S

***********************************************************************/

#include "Linked_List.c"
#include <stdio.h>
#include <stdlib.h>

#define cons(x, ls) ls_insert_beginning(ls, x)
#define snoc(x, ls) ls_insert_end(ls, x)

typedef struct S {
  int num;
} S_t;

void printer(void *data) {
  S_t *S = data;
  printf("%d\n", S->num);
}

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

/*
 * the following macro for cdr fails due to make 2 references to the variable
 * ls, causing an extra_remove_item to occur
 *
 * #define cdr(ls) ls_remove_item(ls, ls_ref(ls, 0))
 * ls_remove_item(ls_remove_item(ls, ls_ref(ls, 0)), ls_ref(ls_remove_item(ls, ls_ref(ls, 0)), 0));
 */
List *cdr(List *L) {
  L = ls_remove(L, ls_ref(L, 0));
  return L;
}

int main() {

  List *ls = make_ls(printer, comparator);
  S_t *val1 = malloc(sizeof(S_t));
  val1->num = 5;
  
  S_t *val2 = malloc(sizeof(S_t));
  val2->num = 7;
  
  S_t *val3 = malloc(sizeof(S_t));
  val3->num = 9;
  
  ls = cons(val2, snoc(val3, snoc(val1, ls)));
  ls_print(ls);
  printf("size of ls bf %d cdr's -> %d\n", 2, ls->size);
  //printf("car -> %d\n", *(int *)ls_ref(ls, 0));
  
  cdr(cdr(ls)); 
  
  printf("size of ls -> %d\n", ls->size);
  printf("tested cdr\n");
  ls_print(ls);

  return 0;
}
