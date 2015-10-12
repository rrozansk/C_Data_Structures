/**********************************************************************

            	    		L I N K E D   L I S T   T E S T S

***********************************************************************/

#include "Linked_List.c"
#include <stdio.h>

#define cons(x, ls) ls_insert_beginning(ls, x)
#define snoc(x, ls) ls_insert_end(ls, x)

typedef struct S {
  int num;
} S_t;

void printer(void *data) {
  S_t *S = data;
  printf("%d\n", S->num);
}

int main() {

  List *ls = make_ls(printer);
  S_t *val = malloc(sizeof(S_t));
  val->num = 5;

  ls_print(cons(val, snoc(val, snoc(val, ls))));

  return 0;
}
