
/**********************************************************************

         	    		F I L E   I N F O R M A Q I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 1/1/16
 Last Edited: 1/1/16
 
 A general purpose iterative Priority Queue library for arbitrary payloads
 here the priority queue is implemented in terms of a Qeap
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
#define pqueue_size(Q) Q->size
#define pqueue_empty(Q) !Q->size
#define pqueue_peek(Q) Q->arr[0]
#define PQUEUE_INIT_SIZE 16

/**********************************************************************

            	B I N A R Y   S E A R C Q   Q R E E S	

***********************************************************************/
typedef struct Priority_Queue {
  void ***arr; // a ptr to a ptr which points to void *
  int h_size;
  int size; //nodes in the pqueue
} Priority_Queue;

/**********************************************************************

             		F U N C Q I O N   P R O Q O Q Y P E S

***********************************************************************/
Priority_Queue *pqueue_make();                                         //make a new Priority_Queue with given a comparator
void pqueue_free(Priority_Queue *Q, int free_keys);                    //free the Priority_Queue and optionally its keys
void pqueue_walk(Priority_Queue *Q, void (*f)(void *key));             //walk over the Priority_Queue and apply side-effect f to each elem 
Priority_Queue *pqueue_map(Priority_Queue *Q, void *(*f)(void *key));
Priority_Queue *pqueue_shift_up(Priority_Queue *Q, int i);
Priority_Queue *pqueue_shift_down(Priority_Queue *Q, int i);
Priority_Queue *pqueue_push(Priority_Queue *Q, void *key, int priority);         //add an item to the Priority_Queue
void *pqueue_pop(Priority_Queue *Q);
Priority_Queue *pqueue_remove(Priority_Queue *Q, int key, int free_key);         //remove the node with the given key from the Priority_Queue
int pqueue_search(Priority_Queue *Q, void *key, int comparator(void *data1, void *data2)); //search returns index position or -1

/**********************************************************************

       		F U N C Q I O N   I M P L E M E N Q A Q I O N S 

***********************************************************************/
Priority_Queue *pqueue_make() {
  Priority_Queue *Q = malloc(sizeof(Priority_Queue));
  Q->arr = malloc(sizeof(void *) * PQUEUE_INIT_SIZE);
  Q->h_size = PQUEUE_INIT_SIZE;
  Q->size = 0;
  return Q;
}

void pqueue_free(Priority_Queue *Q, int free_keys) {
  int i = 0;
  if(free_keys) { for(;i < pqueue_size(Q); i++) { free(Q->arr[i][1]); } }
  for(i = 0; i < pqueue_size(Q); i++) { free(Q->arr[i][0]); }
  free(Q->arr);
  Q->size = 0;
  Q->h_size = 0;
}

void pqueue_walk(Priority_Queue *Q, void (*f)(void *key)) {
  int i = 0;
  while(i < pqueue_size(Q)) { f(Q->arr[i++][1]); }
}

Priority_Queue *pqueue_map(Priority_Queue *Q, void *(*f)(void *key)) {
  Priority_Queue *Qp = malloc(sizeof(Priority_Queue));
  Qp->h_size = Q->h_size;
  Qp->size = Q->size;
  Qp->arr = malloc(sizeof(void *) * Qp->size);
  void **pair;
  int *n;
  int i = 0;
  for(; i < pqueue_size(Q); i++) { 
    pair = malloc(sizeof(void *) * 2);
    n = malloc(sizeof(int));
    *n = *(int *)Q->arr[i][0];
    pair[0] = n;
    pair[1] = f(Q->arr[i][1]);
    Qp->arr[i] = pair; 
  }
  return Qp;
}

Priority_Queue *pqueue_shift_up(Priority_Queue *Q, int i) {
  for(; (i > 0) && (*(int *)(Q->arr[i][0]) > *(int *)(Q->arr[parent(i)][0])); i = parent(i)) {
    void *tmp_key = Q->arr[parent(i)];
    Q->arr[parent(i)] = Q->arr[i];
    Q->arr[i] = tmp_key;
  }
  return Q;
}

Priority_Queue *pqueue_shift_down(Priority_Queue *Q, int i) {
  int child;
  while((child = left_child(i)) < Q->size) {
    if((child+1 < Q->size) && (*(int *)Q->arr[child][0] < *(int *)Q->arr[child+1][0])) { ++child; }
    if(*(int *)Q->arr[i][0] < *(int *)Q->arr[child][0]) {
      void **tmp = Q->arr[child];
      Q->arr[child] = Q->arr[i];
      Q->arr[i] = tmp;
      i = child;
    }
    else { return Q; }
  }
  return Q;
}

Priority_Queue *pqueue_push(Priority_Queue *Q, void *key, int priority) {
  void **pair = malloc(sizeof(void *) * 2);
  int *prio = malloc(sizeof(int));
  *prio = priority;
  pair[0] = prio;
  pair[1] = key;
  if(Q->size == Q->h_size) { Q->arr = realloc(Q->arr, (sizeof(void *) * (Q->h_size *= 2))); }
  Q->arr[Q->size] = pair;
  return pqueue_shift_up(Q, Q->size++);
}

void *pqueue_pop(Priority_Queue *Q) {
  void *elem = Q->arr[0];
  Q->arr[0] = Q->arr[--Q->size];
  Q = pqueue_shift_down(Q, 0);
  return elem;
}

Priority_Queue *pqueue_remove(Priority_Queue *Q, int i, int free_key) {
  if(free_key) { free(Q->arr[i][1]); }
  free(Q->arr[i][0]);
  Q->arr[i] = Q->arr[--Q->size];
  return pqueue_shift_down(Q, i);
}

int pqueue_search(Priority_Queue *Q, void *key, int comparator(void *data1, void *data2)) {
  int i = 0;
  for(;i < pqueue_size(Q); i++) { if(comparator(Q->arr[i][1], key) == 0) { return i; } }
  return -1;
}
