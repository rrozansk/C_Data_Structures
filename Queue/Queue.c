
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 11/9/15
 
 A general purpose iterative queue library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define queue_size(Q) Q->size
#define queue_empty(Q) !Q->size
#define queue_peek(Q) Q->head->data

/**********************************************************************

                 	    		Q U E U E S

***********************************************************************/
typedef struct q_node {
  void *data;
  struct q_node *next;
} q_node;

typedef struct Queue {
  q_node *head;
  q_node *tail;
  int size;
} Queue;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Queue *queue_make();                                                                //make a new queue
void queue_free(Queue *Q, int free_data);                                           //free the queue and optionally its data
void queue_walk(Queue *Q, void (*f)(void *data));                                   //walk over the queue and apply side-effect f to each elem
Queue *queue_map(Queue *Q, void *(*f)(void *data));                                 //return a new queue resulting from applying f to each elem
Queue *queue_enqueue(Queue *Q, void *data);                                         //add an item at the tail (end) of the queue
void *queue_dequeue(Queue *Q);                                                      //return the data from the head (top) of the queue and side effect the queue
int queue_search(Queue *Q, void *data, int (*comparator)(void *data1, void *data2));//return index of item if in the queue, otherwise -1

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Queue *queue_make() {
  Queue *Q = malloc(sizeof(Queue));
  Q->head = NULL;
  Q->tail = NULL;
  Q->size = 0;
  return Q;
}

void queue_free(Queue *Q, int free_data) {
  q_node *current = Q->head;
  while(current != NULL) {
    Q->head = current->next;
    if(free_data) { free(current->data); }
    free(current);
    current = Q->head;
  }
  Q->head = NULL;
  Q->tail = NULL;
  Q->size = 0;
}

void queue_walk(Queue *Q, void (*f)(void *data)) {
  q_node *current = Q->head;
  for(;current != NULL; current = current->next) { f(current->data); }
}

Queue *queue_map(Queue *Q, void *(*f)(void *data)) {
  Queue *C = malloc(sizeof(Queue));
  C->size = Q->size;
  q_node *current = Q->head;
  q_node *new_node;
  if(!queue_empty(Q)) {
    new_node = malloc(sizeof(q_node));
    new_node->data = f(current->data);
    new_node->next = NULL;
    C->head = C->tail = new_node;
    current = current->next;
  }
  for(;current != NULL; current = current->next) {
    new_node = malloc(sizeof(q_node));
    new_node->data = f(current->data);
    new_node->next = NULL;
    C->tail = C->tail->next = new_node;
  }
  return C;
}

Queue *queue_enqueue(Queue *Q, void *data) {
  q_node *N = malloc(sizeof(q_node));
  N->next = NULL;
  N->data = data;
  if(queue_empty(Q)) { Q->head = Q->tail = N; }
  else { Q->tail = Q->tail->next = N; }
  Q->size++;
  return Q;
}

void *queue_dequeue(Queue *Q) {
  q_node *N = Q->head;
  Q->head = N->next;
  if(Q->head == NULL) { Q->tail = NULL; }
  Q->size--;
  return N->data;
}

int queue_search(Queue *Q, void *data, int (*comparator)(void *data1, void *data2)) {
  int wait_pos = 0;
  q_node *current = Q->head;
  for(;current != NULL; current = current->next, wait_pos++) {
    if(comparator(data, current->data) == 0) { return wait_pos; }
  }
  return -1;
}

