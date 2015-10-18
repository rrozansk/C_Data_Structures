
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/1/15
 
 A general purpose queue library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                 	    		Q U E U E S

***********************************************************************/
typedef struct q_node {
  void *data;
  struct q_node *next;
} q_node;

typedef struct Queue {
  void(*printer)(void *data);
  q_node *head;
  q_node *tail;
  int size;
} Queue;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Queue *make_queue(void (*printer)(void *data)); //make a new queue
void delete_queue(Queue *Q);                    //delete a queue
Queue *queue_enqueue(Queue *Q, void *data);     //add an item at the tail
void *queue_dequeue(Queue *Q);                  //remove the item at the head and return it
void *queue_peek(Queue *Q);                     //return the item at the head (without removing it)
int queue_contains(Queue *Q, void *data);       //return the waiting position of data in the queue 
int queue_size(Queue *Q);                       //return the number of items in the queue
int queue_empty(Queue *Q);                      //return whether the queue has no items
void queue_print(Queue *Q);                     //print out the queue

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Queue *make_queue(void (*printer)(void *data)) {
  Queue *Q = malloc(sizeof(Queue));
  Q->printer = printer;
  Q->head = NULL;
  Q->tail = NULL;
  Q->size = 0;
  return Q;
}

void delete_queue(Queue *Q) {
  q_node *current = Q->head;
  while(current != NULL) {
    q_node *temp = current->next;
    free(current);
    current = temp;
  }
  free(Q);
}

Queue *queue_enqueue(Queue *Q, void *data) {
  q_node *N = malloc(sizeof(q_node));
  N->next = NULL;
  N->data = data;
  if(!Q->size) {
    Q->head = N;
    Q->tail = N;
  }
  else {
    Q->tail->next = N;
    Q->tail = N;
  }
  Q->size = Q->size++;
  return Q;
}

void *queue_dequeue(Queue *Q) {
  q_node *N = Q->head;
  Q->head = N->next;
  Q->size = Q->size--;
  return N->data;
}

void *queue_peek(Queue *Q) {
  return Q->head->data;
}

//cant do this void* comparison
int queue_contains(Queue *Q, void *data) {
  int wait_pos = 0;
  q_node *current = Q->head;
  while(current != NULL) {
    wait_pos += 1;
    if(current == data) break;
    current = current->next;
  }
  return wait_pos;
}

int queue_size(Queue *Q) {
  return Q->size;
}

int queue_empty(Queue *Q) {
  return Q->size ? 0 : 1;
}

void queue_print(Queue *Q) {
  q_node *current = Q->head;
  while(current != NULL) {
    Q->printer(current->data);
    current = current->next;
  }
} 
