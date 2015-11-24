
/***********************************************************************

            	    		B I N A R Y   T R E E S   T E S T S

***********************************************************************/

#include "Binary_Search_Tree.c"
#include "../Linked_List/Linked_List.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

int rand_pick(void *data1, void *data2) {
  if(rand() % 2 == 0) { return -1; }
  else { return 1; }
}

void printer(void *data) { printf("%d\t", *(int *)data); }
void visitor(bst_node *node) { printf("%d\n", *(int *)node->key); }
void add1(void *key) { *(int*)key = (*(int *)key)++; }

void *make_key(void *old_key) {
  int *new_key = malloc(sizeof(int));
  *new_key = *(int *)old_key + 10;
  return new_key;
}

int main() {
  
  srand(time(NULL));

  BST *T = bst_make(comparator);
  List *L = ls_make();

  int generations = 10;
  int tr_size = 6;

  int j = 0;
  for(;j<generations;j++) {
    //randomly generate a tree
    printf("making tree #%d\n", j);
    int i = 0;
    for(;i<tr_size;i++) { 
      int *tmp = malloc(sizeof(int));
      *tmp = rand() % 100;
      bst_insert(T, tmp);
      ls_cons(L, tmp);
    }
    printf("list:\t");
    ls_walk(L, printer);
    ls_sort(L, rand_pick);
    //printf("tr height: %d\n", bst_height(T));
    //printf("tr size: %d\n", bst_size(T));
    //printf("tr null?: %d\n", bst_empty(T));
    
    i = 0;
    for(;bst_size(T) > 0; i++) { 
      void *key = ls_ref(L, 0);
      ls_remove_nth(L, 0, 0);
      printf("\nremoving: %d", *(int *)key);
      bst_remove(T, bst_search(T, key), 1); 
      printf("\npre\t");
      bst_walk(T, PREORDER, printer);
      printf("\nin\t");
      bst_walk(T, INORDER, printer);
      printf("\npost\t");
      bst_walk(T, POSTORDER, printer);
     }
    //bst_free(T, 1);
    ls_free(L, 0);
  }
  /*
  tr_breadth_first(T, visitor); //then print tree
  printf("tr max -> %d\n", *(int *)tr_maximum(T));
  printf("tr min -> %d\n", *(int *)tr_minimum(T));
  printf("tr null? -> %d\n", tr_null(T));
  Tree *T2 = tr_map(T, make_key);
  tr_walk(T2, PREORDER, printer);
  tr_walk(T, PREORDER, printer);
  
  tr_free(T, 1); 
  tr_free(T2, 1);

  T = NULL;
  T2 = NULL;
  */ 
  return 0;
}
