
/**********************************************************************

         	    		F I L E   I N F O R M A H I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 11/25/15
 Last Edited: 12/31/15
 
 A general purpose iterative Heap library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include <math.h>

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define parent(i) ((i-1)/2)
#define left_child(i) ((2*i)+1)
#define right_child(i) ((2*i)+2)
#define heap_size(H) H->size
#define heap_empty(H) !H->size
#define heap_peek(H) H->arr[0]
#define HEAP_INIT_SIZE 16

/**********************************************************************

            	B I N A R Y   S E A R C H   H R E E S	

***********************************************************************/
typedef struct Heap {
  int (*comparator)(void *key1, void *key2);
  void **arr;
  int h_size;
  int size; //nodes in the heap
} Heap;

/**********************************************************************

             		F U N C H I O N   P R O H O H Y P E S

***********************************************************************/
Heap *heap_make(int (*comparator)(void *key1, void* key2));//make a new Heap with given a comparator
void heap_free(Heap *H, int free_keys);                    //free the Heap and optionally its keys
void heap_walk(Heap *H, void (*f)(void *key));             //walk over the Heap and apply side-effect f to each elem 
Heap *heap_map(Heap *H, void *(*f)(void *key));
Heap *heap_shift_up(Heap *H, int i);
Heap *heap_shift_down(Heap *H, int i);
Heap *heap_remove(Heap *H, int key, int free_key);         //remove the node with the given key from the Heap
Heap *heapify(void *arr[], int size, int (*comparator)(void *key1, void* key2));
Heap *heap_push(Heap *H, void *key);                       //add an item to the Heap
void *heap_pop(Heap *H);
int heap_search(Heap *H, void *key);                       //search returns index position or -1
Heap *heap_union(Heap *H1, Heap *H2, int free_keys);       //join 2 heaps, preserving(merge; 0)/destroying(meld; 1) the original heaps.
void *heapsort(Heap *H);

/**********************************************************************

       		F U N C H I O N   I M P L E M E N H A H I O N S 

***********************************************************************/
Heap *heap_make(int (*comparator)(void *key1, void* key2)) {
  Heap *H = malloc(sizeof(Heap));
  H->comparator = comparator;
  H->arr = malloc(sizeof(void *) * HEAP_INIT_SIZE);
  H->h_size = HEAP_INIT_SIZE;
  H->size = 0;
  return H;
}

void heap_free(Heap *H, int free_keys) {
  int i = 0;
  if(free_keys) { for(;i < heap_size(H); i++) { free(H->arr[i]); } }
  free(H->arr);
  H->size = 0;
  H->h_size = 0;
}

void heap_walk(Heap *H, void (*f)(void *key)) {
  int i = 0;
  while(i < heap_size(H)) { f(H->arr[i++]); }
}

Heap *heap_map(Heap *H, void *(*f)(void *key)) {
  Heap *Hp = malloc(sizeof(Heap));
  Hp->comparator = H->comparator;
  Hp->h_size = H->h_size;
  Hp->size = H->size;
  Hp->arr = malloc(sizeof(void *) * Hp->size);
  int i = 0;
  for(; i < heap_size(H); i++) { Hp->arr[i] = f(H->arr[i]); }
  return Hp;
}

Heap *heap_shift_up(Heap *H, int i) { 
  for(; i > 0 && H->comparator(H->arr[i], H->arr[parent(i)]) == 1; i = parent(i)) {
    void *tmp_key = H->arr[parent(i)];
    H->arr[parent(i)] = H->arr[i];
    H->arr[i] = tmp_key;
  }
  return H;
}

Heap *heap_shift_down(Heap *H, int i) {
  int child;
  while((child = left_child(i)) < H->size) {
    if((child+1 < H->size) && H->comparator(H->arr[child],H->arr[child+1]) == -1) { ++child; }
    if(H->comparator(H->arr[i], H->arr[child]) == -1) {
      void *tmp = H->arr[child];
      H->arr[child] = H->arr[i];
      H->arr[i] = tmp;
      i = child;
    }
    else { break; }
  }
  return H;
}

Heap *heap_remove(Heap *H, int i, int free_key) {
  if(free_key) { free(H->arr[i]); }
  H->arr[i] = H->arr[--H->size];
  return heap_shift_down(H, i);
}

Heap *heapify(void *arr[], int size, int (*comparator)(void * key1, void *key2)) {
  Heap *H = malloc(sizeof(Heap));
  H->comparator = comparator;
  H->size = size;
  H->h_size = (1 << (int)floor(log2(size))+1); //next closest 2^n
  H->arr = malloc(sizeof(void *) * H->h_size);
  for(size = 0; size < H->size; size++) { 
    H->arr[size] = arr[size]; 
    heap_shift_up(H, size); 
  }
  return H;
}

Heap *heap_push(Heap *H, void *key) {
  if(H->size == H->h_size) { H->arr = realloc(H->arr, (sizeof(void *) * (H->h_size *= 2))); }
  H->arr[H->size] = key;
  return heap_shift_up(H, H->size++);
}

void *heap_pop(Heap *H) {
  void *elem = H->arr[0];
  H->arr[0] = H->arr[--H->size];
  heap_shift_down(H, 0);
  return elem;
}

int heap_search(Heap *H, void *key) {
  int i = 0;
  for(;i < heap_size(H); i++) { if(H->comparator(H->arr[i], key) == 0) { return i; } }
  return -1;
}

Heap *heap_union(Heap *H1, Heap *H2, int free_keys) { //can probably be better
  Heap *H = malloc(sizeof(Heap));
  H->comparator = H1->comparator; //assume same as H2
  H->size = H1->size + H2->size;
  H->h_size = (1 << (int)floor(log2(H->size))+1); //next closest 2^n
  H->arr = malloc(sizeof(void *) * H->h_size);
  int i = 0;
  for(; i < H1->size; i++) {
    H->arr[i] = H1->arr[i]; 
    heap_shift_up(H, i);
  }
  for(i = 0; i < H2->size; i++) {
    H->arr[H1->size+i] = H2->arr[i]; 
    heap_shift_up(H, H1->size+i);
  }
  if(free_keys) {  //join 2 heaps, preserving(merge; 0)/destroying(meld; 1) the original heaps.
    for(i = 0; i < H1->size; i++) { free(H1->arr[i]); }
    for(i = 0; i < H2->size; i++) { free(H2->arr[i]); }
  }
  return H;
}

void *heapsort(Heap *H) {
  void **copy = H->arr;
  H->arr = malloc(sizeof(void *) * heap_size(H));
  int i = 0;
  for(; i < heap_size(H); i++) { H->arr[i] = copy[i]; }
  int len = H->size;
  while(--H->size > 0) {
    void *tmp = H->arr[H->size];
    H->arr[H->size] = H->arr[0];
    H->arr[0] = tmp;
    heap_shift_down(H, 0);
  }
  H->size = len;
  void **tmp = H->arr;
  H->arr = copy;
  return tmp;
}
