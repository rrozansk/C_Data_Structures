
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/18/15
 
 A general purpose queue library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define queue_size(Q) Q->size
#define queue_peek(Q) Q->head->data
#define queue_empty(Q) !Q->size

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
Queue *queue_make();                                 //make a new queue
void queue_free(Queue *Q, int free_data);            //free a queue and optionally its data
Queue *queue_walk(Queue *Q, void (*f)(void *data));  //copy a queue
Queue *queue_map(Queue *Q, void *(*f)(void *data));  //copy a queue
Queue *queue_enqueue(Queue *Q, void *data);          //add an item at the tail
void *queue_dequeue(Queue *Q);                       //remove the item at the head and return it
int queue_contains(Queue *Q, int (*comparator)(void *data1, void *data2), void *data); //return the waiting position of data in the queue 

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
  q_node *prev;
  while(current != NULL) {
    prev = current;
    current = current->next;
    if(free_data) { free(prev->data); }
    free(prev);
  }
  Q->head = NULL;
  Q->tail = NULL;
  Q->size = 0;
}

Queue *queue_walk(Queue *Q, void (*f)(void *data)) {
  q_node *current = Q->head;
  while(current != NULL) { 
    f(current->data);
    current = current->next; }
  return Q;
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
  while(current != NULL) {
    new_node = malloc(sizeof(q_node));
    new_node->data = f(current->data);
    new_node->next = NULL;
    C->tail->next = new_node;
    C->tail = new_node;
    current = current->next;
  }
  return C;
}

Queue *queue_enqueue(Queue *Q, void *data) {
  q_node *N = malloc(sizeof(q_node));
  N->next = NULL;
  N->data = data;
  if(queue_empty(Q)) {
    Q->head = N;
    Q->tail = N;
  }
  else {
    Q->tail->next = N;
    Q->tail = N;
  }
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

int queue_contains(Queue *Q, int (*comparator)(void *data1, void *data2), void *data) {
  int wait_pos = 0;
  q_node *current = Q->head;
  while(current != NULL) {
    if(comparator(data, current->data) == 0) { break; }
    wait_pos++;
    current = current->next;
  }
  return -1;
}
