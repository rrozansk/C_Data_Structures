
/***********************************************************************

            	    		A V L   T R E E S   T E S T S

***********************************************************************/

#include "AVL_Tree.c"
#include "../Linked_List/Linked_List.c"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

int rand_pick(void *data1, void *data2) {
  return (rand() % 2 == 0) ? 1 : -1;
}

void printer(void *data) { printf("%d\t", *(int *)data); }
void visitor(avl_node *node) { printf("%d\n", *(int *)node->key); }
void add1(void *data) { *(int *)data = *(int *)data + 1; }
  
int last;
int avl;

void avl_check(void *key) {
  if(last > *(int *)key) { avl = 0; printf("last: %d\tthis: %d\n", last, *(int *)key); }
  last = *(int *)key;
}

int is_avl(AVL *T) {
  avl = 1;
  last = 0;
  if(avl_empty(T)) { return 1; }
  avl_walk(T, INORDER, avl_check);
  return avl;
}

int main() {
  
  srand(time(NULL));

  AVL *T = avl_make(comparator);
  List *Set = ls_make();
  
  int tr_generations = 10000000;
  int tr_size = 100;
  int PRINT_GEN = 1000000;
  double min_h;
  double max_h;
  
  printf("max height with %d nodes during %d generations is best case %lf - worst case %lf\n" \
      ,tr_size, tr_generations, min_h = log2(tr_size), max_h = 1.44*log2(tr_size));

  int j = 1;
  int h;
  for(;j<=tr_generations;j++) {
    if(j % PRINT_GEN == 0) { printf("generating tree %d\n", j); }
    int i = 1;
    for(;i<=tr_size;i++) { 
      int *tmp = malloc(sizeof(int));
      *tmp = rand() % 1000;
      if(ls_search(Set, tmp, comparator) == -1) { 
        ls_cons(Set, tmp); 
        avl_insert(T, tmp);
        h = avl_height(T);
        max_h = ceil(1.44*log2(avl_size(T)));
        min_h = floor(log2(avl_size(T))); 
        if(h > max_h || h < min_h ) { 
          printf("invalid tr height insert\ttr h: %d (%lf - %lf)\n", avl_height(T), min_h, max_h); 
        } //rotation/balance work when inserting
      }
      else { free(tmp); }
    }
    ls_sort(Set, rand_pick); //trying to get random deletion behavior to test my code against everything
    do {
      void *key = ls_ref(Set, 0);
      Set = ls_remove_nth(Set, 0, 0);
      avl_remove(T, avl_search(T, key), 1);
      if(avl_size(T) > 0 && (avl_height(T) > ceil(1.44*log2(avl_size(T))) || avl_height(T) < floor(log2(avl_size(T))))) { 
        printf("invalid tr height deletet\ttr h: %d (%lf - %lf)\n", avl_height(T), min_h, max_h); 
      } //rotation/balance work when inserting
      if(!is_avl(T)) { printf("invalid tr order\n"); } //violation of bst property
    }  while(!avl_empty(T));
    if(!avl_empty(T)) { printf("invalid tr size\n"); } //deletion didnt always work if above 0
  }
  return 0;
}
