
/**********************************************************************

         	    		F I L E   I N F O R M A Q I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 1/1/16
 Last Edited: 1/10/16
 
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
#define pqueue_peek(Q) ((void **)Q->arr[0])[1]
#define PQUEUE_INIT_SIZE 16

/**********************************************************************

            	P R I O R I T Y   Q U E U E

***********************************************************************/
typedef struct Priority_Queue {
  int h_size;
  int size; //nodes in the pqueue
  void **arr; 
} Priority_Queue;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Priority_Queue *pqueue_make();                                         //make a new Priority_Queue with given a comparator
void pqueue_free(Priority_Queue *Q, int free_keys);                    //free the Priority_Queue and optionally its keys
void pqueue_walk(Priority_Queue *Q, void (*f)(void *key));             //walk over the Priority_Queue and apply side-effect f to each elem 
Priority_Queue *pqueue_map(Priority_Queue *Q, void *(*f)(void *key));
Priority_Queue *pqueue_shift_up(Priority_Queue *Q, int i);
Priority_Queue *pqueue_shift_down(Priority_Queue *Q, int i);
Priority_Queue *pqueue_push(Priority_Queue *Q, void *key, int priority);         //add an item to the Priority_Queue
void *pqueue_pop(Priority_Queue *Q);
Priority_Queue *pqueue_remove(Priority_Queue *Q, void *key, int free_key);         //remove the node with the given key from the Priority_Queue
int pqueue_search(Priority_Queue *Q, void *key, int comparator(void *data1, void *data2)); //search returns priority of key or -1


//pqueue_chage_priority(Priority_Queue *Q, void *key, int new_priority);

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

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
  if(free_keys) { for(;i < pqueue_size(Q); i++) { free(((void **)Q->arr[i])[1]); } }
  for(i = 0; i < pqueue_size(Q); i++) { 
    free(((void **)Q->arr[i])[0]);  //all the priorities (int *)
    free((void **)Q->arr[i]);
  }
  free(Q->arr);
  Q->arr = NULL;
  Q->size = 0;
  Q->h_size = 0;
}

void pqueue_walk(Priority_Queue *Q, void (*f)(void *key)) {
  int i = 0;
  for(; i < pqueue_size(Q); i++) { f(((void **)Q->arr[i])[1]); }
}
/*
Priority_Queue *pqueue_map(Priority_Queue *Q, void *(*f)(void *key)) {
  Priority_Queue *Qp = malloc(sizeof(Priority_Queue));
  Qp->h_size = Q->h_size;
  Qp->size = Q->size;
  Qp->arr = malloc(sizeof(void *) * Qp->size * 2);
  int i = 0;
  for(; i < pqueue_size(Q); i++) { 
    int *n = malloc(sizeof(int));
    *n = *(int *)((Q->arr + i) + 0);
     ((Q->arr + i) + 0) = n;
     (Q->arr + i) + 1 = f(Q->arr[i][1]);
  }
  return Qp;
}
*/
Priority_Queue *pqueue_shift_up(Priority_Queue *Q, int i) {
  for(; (i > 0) && *(int *)((void **)(Q->arr[i]))[0] > *(int *)((void **)(Q->arr[parent(i)]))[0]; i = parent(i)) {
    void *tmp_key = Q->arr[parent(i)];
    Q->arr[parent(i)] = Q->arr[i];
    Q->arr[i] = tmp_key;
  }
  return Q;
}

Priority_Queue *pqueue_shift_down(Priority_Queue *Q, int i) {
  int child;
  while((child = left_child(i)) < Q->size) {
    if((child+1 < Q->size) && (*(int *)((void **)Q->arr[child])[0] < *(int *)((void **)Q->arr[child+1])[0])) { ++child; }
    if(*(int *)((void **)Q->arr[i])[0] < *(int *)((void **)Q->arr[child])[0]) {
      void *tmp = Q->arr[child];
      Q->arr[child] = Q->arr[i];
      Q->arr[i] = tmp;
      i = child;
    }
    else { return Q; }
  }
  return Q;
}

Priority_Queue *pqueue_push(Priority_Queue *Q, void *key, int priority) {
  if(Q->size == Q->h_size) { Q->arr = realloc(Q->arr, (sizeof(void *) * (Q->h_size *= 2))); }
  int *prio = malloc(sizeof(int));
  *prio = priority;
  void **pair = malloc(sizeof(void *) * 2);
  pair[0] = prio;
  pair[1] = key;
  Q->arr[Q->size] = pair;
  pqueue_shift_up(Q, Q->size);
  Q->size++;
  return Q;
}

void *pqueue_pop(Priority_Queue *Q) {
  void *key = ((void **)Q->arr[0])[1];  //key
  free(((void **)Q->arr[0])[0]);        //priority
  free(Q->arr[0]);                      //pair
  Q->arr[0] = Q->arr[--Q->size];
  pqueue_shift_down(Q, 0);
  return key;
}

Priority_Queue *pqueue_remove(Priority_Queue *Q, void *key, int free_key) {
  if(free_key) {  
    //nop();
  }
  //Q->arr[i] = Q->arr[--Q->size];
  int i = 0; //idk...
  return pqueue_shift_down(Q, i);
}

//this returns the priority --> useful, but how to do remove well then? should i
//do get priority? and then revert search to give pos in array?
int pqueue_search(Priority_Queue *Q, void *key, int comparator(void *data1, void *data2)) {
  int i = 0;
  for(;i < pqueue_size(Q); i++) { 
    if(comparator(((void **)Q->arr[i])[1], key) == 0) { 
      return *(int *)((void **)Q->arr[i])[0]; 
    }
  }
  return -1;
}
