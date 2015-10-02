/**********************************************************************

            	    		L I N K E D   L I S T   T E S T S

***********************************************************************/

#include "Linked_List.c"

typedef struct S {
  int num;
} S_t;

// in this API usr will always have to write this function some something like
// it. name doesnt matter
void printer(void *data) {
  /*S_t*/int data = *data;
  printf("%d\n", /*data->num*/ *data);
}

int main() {

  List *ls = malloc(sizeof(List));
  list_print(ls, *print_node );
/*
  S_t *trash = malloc(sizeof(S_t));
  trash->num = 5;
  insert_beggining(ls, trash);
*/
  int something = 5;
  int *ptr = &something;
  insert_beggining(ls, ptr);
  list_print(ls, *print_node );
  remove_beggining(ls);
  list_print(ls, *print_node );
  insert_beggining(ls, ptr);
  int more = 6;
  int *ptrm = &more;
  insert_beggining(ls, ptrm);
  int morek = 7;
  int *ptrmn = &morek;
  insert_beggining(ls, ptrmn);
  printf("list lenght: %d\n", list_length(ls));
  list_print(ls, *print_node );
  remove_beggining(ls);
  list_print(ls, *print_node );
  remove_beggining(ls);
  list_print(ls, *print_node );
  printf("list lenght: %d\n", list_length(ls));
  remove_beggining(ls);
  list_print(ls, *print_node );
  remove_beggining(ls);
  list_print(ls, *print_node );
  list_print(ls, *print_node );
  printf("list lenght: %d\n", list_length(ls));
  insert_end(ls, ptrmn);
  list_print(ls, *print_node );
  insert_end(ls, ptrm);
  list_print(ls, *print_node );
  insert_end(ls, ptr);
  list_print(ls, *print_node );
  printf("list lenght: %d\n", list_length(ls));
  List *ls2 = malloc(sizeof(List));
  insert_beggining(ls2, ptr);
  insert_beggining(ls2, ptrm);
  insert_beggining(ls2, ptrmn);
  list_print(ls2, *print_node );
  list_append(ls, ls2);
  list_print(ls, *print_node );
  printf("list2\n");
  list_print(ls2, *print_node );
  remove_after(ls, ptr);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  remove_after(ls, ptr);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  remove_after(ls, ptrmn);
  list_print(ls, *print_node );
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  insert_after(ls, ptrmn, ptr);
  list_print(ls, *print_node );
  int asd = 10;
  int *here = &asd;
  insert_after(ls, ptr, here);
  insert_after(ls, here, ptrmn);
  list_print(ls, *print_node );
  printf("testing got here\n");
  int asdf = 15;
  int *hjk = &asdf;
  insert_after(ls, hjk, here);
  list_print(ls, *print_node );
  insert_end(ls, hjk);
  list_print(ls, *print_node );
  insert_after(ls, hjk, ptr);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );
  remove_end(ls);
  list_print(ls, *print_node );

  return 0;
}
