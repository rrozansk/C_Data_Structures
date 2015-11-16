
/***********************************************************************

            	    		A V L   T R E E S   T E S T S

***********************************************************************/

#include "AVL_Tree.c"
#include "../Linked_List/Linked_List.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

void printer(void *data) { printf("%d\n", *(int *)data); }
void visitor(avl_node *node) { printf("%d\n", *(int *)node->key); }

int main() {
  
  srand(time(NULL));

  AVL *T = avl_make(comparator);
  List *L = ls_make();

  printf("randomly generating AVL tree\n");
  int i = 0;
  for(;i<10000;i++) { 
    int *tmp = malloc(sizeof(int));
    *tmp = rand();
    ls_cons(L, tmp);
    avl_insert(T, tmp);
   }

  ls_sort(L, comparator);
 
  printf("finished building tree\n");
  printf("tr nodes # -> %d\n", avl_size(T));
  printf("tr height -> %d\n", avl_height(T));
  printf("deleting tree\n");
  
  ls_free(L, 0);
  avl_free(T, 1);

  return 0;
}
