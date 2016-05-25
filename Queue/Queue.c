/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 5/25/15
 
 A general purpose iterative queue library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include "Queue.h"

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
