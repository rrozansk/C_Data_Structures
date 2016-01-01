
/***********************************************************************

            	    		H E A P S   T E S T S

***********************************************************************/
#include "Heap.c"
#include "../Linked_List/Linked_List.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int max_heap(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

int min_heap(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return 1;
  if (d1 > d2) return -1;
}

int rand_pick(void *data1, void *data2) { return (rand() % 2 == 0) ? 1 : -1; }
void printer(void *data) { printf("%d\t", *(int *)data); }

int main() {
  
  srand(time(NULL));

  Heap *H  = heap_make(max_heap);

  int generations = 1;
  int print_gen = 1;
  int h_size = 15; //doesnt work on pow of 2

  int i;
  int j = 0;
  int *tmp;

  void *nums[h_size];

  for(;j<generations;j++) {
    if(j % print_gen == 0) { printf("making heap #%d\n", j); }
    for(i = 0; i < h_size; i++) { 
      tmp = malloc(sizeof(int));
      *tmp = rand() % 10;
      nums[i] = tmp;
      H = heap_push(H, tmp); 
    }
    Heap *Hp = heapify(nums, 15, H->comparator);
    printf("heapified\n");
    heap_walk(Hp, printer);
    printf("\n");
    void **ordered = heapsort(Hp);
    printf("sorted\n");
    for(i = 0; i < heap_size(Hp); i++) { printf("%d\t", *(int *)ordered[i]); }
    printf("\n");
    for(i = 0; !heap_empty(H); i++) { 
//      printf("heap iter: %d\tsize: %d\n", i, heap_size(H));
      heap_walk(H, printer);
      printf("\n");
      heap_pop(H);
//      printf("\npoped: %d\n", *(int *)heap_pop(H));
//      printf("found? %d\n", heap_search(H, tmp));
    }
    heap_free(H, 0);
  }
  return 0;
}
