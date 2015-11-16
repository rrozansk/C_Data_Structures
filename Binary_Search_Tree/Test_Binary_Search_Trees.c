
/***********************************************************************

            	    		B I N A R Y   T R E E S   T E S T S

***********************************************************************/

#include "Binary_Search_Tree.c"
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
  Queue *Q = queue_make();

  int j = 0;
//  for(;j<1000;j++) {
    //randomly generate a tree
    printf("making tree #%d\n", j);
    int i = 0;
    for(;i<10000;i++) { 
      int *tmp = malloc(sizeof(int));
      *tmp = rand();
      bst_insert(T, tmp);
      queue_enqueue(Q, tmp);
      ls_cons(L, tmp);
    }
    ls_sort(L, comparator); //should make removing more "random"-ish
    
    printf("finished building tree, finding height\n");
    printf("tr height: %d\n", bst_height(T));
    printf("tr size: %d\n", bst_size(T));
    printf("tr null?: %d\n", bst_empty(T));
    printf("deleting every node\n");
    
    i = 0;
    for(;bst_size(T) > 0; i++) { 
      //bst_remove(T, bst_search(T, queue_dequeue(Q)), 0);
      void *key = ls_ref(L, 0);
      ls_remove_nth(L, 0, 0);
      bst_remove(T, bst_search(T, key), 0); 
    }
    printf("deletion done\n");
    bst_free(T, 1);
    ls_free(L, 0);
    queue_free(Q, 0);
//  }
  /*
  printf("breath first search of tree with visitor being the printer\n");
  tr_breadth_first(T, visitor);
  printf("pre-order walk of tree with visitor being the printer\n");
  tr_walk(T, PREORDER, printer);
  printf("in-order walk of tree with visitor being the printer\n");
  tr_walk(T, INORDER, printer);
  printf("post-order walk of tree with visitor being the printer\n");
  tr_walk(T, POSTORDER, printer);
  printf("doing a tree walk to mutate keys\n");
  tr_walk(T, INORDER, add1);
  tr_walk(T, INORDER, printer);
  printf("tr max -> %d\n", *(int *)tr_maximum(T));
  printf("tr min -> %d\n", *(int *)tr_minimum(T));
  printf("tr null? -> %d\n", tr_null(T));
  printf("attempting tr map\n");
  Tree *T2 = tr_map(T, make_key);
  printf("pre-order walk of map-ed tree with visitor being the printer\n");
  tr_walk(T2, PREORDER, printer);
  printf("pre-order walk of original tree with visitor being the printer\n");
  tr_walk(T, PREORDER, printer);
  printf("attempting to free tr's\n");
  tr_free(T, 1); //since we malloc all our data we can tell the tree to delete the keys aswell (1)
  tr_free(T2, 1);
  T = NULL;
  T2 = NULL;
  printf("attempt's succesful\n");
  printf("tr null?: %d\n", bst_empty(T));
  */ 
  return 0;
}
