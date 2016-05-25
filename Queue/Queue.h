/**********************************************************************

                      F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 5/25/16
 Last Edited: 5/25/16
*/

#ifndef QUEUE_DEFS
#define QUEUE_DEFS

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

#endif
