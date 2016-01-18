
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 11/9/15
 
 A general purpose iterative stack library for arbitrary payloads
*/

/**********************************************************************

                  	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                  	    	 M A C R O S	

***********************************************************************/
#define stack_size(S) S->size
#define stack_empty(S) !S->size
#define stack_peek(S) S->top->data

/**********************************************************************

                  	    		S T A C K S

***********************************************************************/
typedef struct Frame {
  void *data;
  struct Frame *next;
} Frame;

typedef struct Stack {
  Frame *top;
  int size;
} Stack; // Stack STRUCT

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Stack *stack_make();                                                                //make a new stack
void stack_free(Stack *S, int free_data);                                           //free the stack and optionally its data
void stack_walk(Stack *S, void (*f)(void *data));                                   //walk over the stack and apply side-effect f to each elem
Stack *stack_map(Stack *S, void *(*f)(void *data));                                 //return a new stack resulting from applying f to each elem
Stack *stack_push(Stack *S, void *data);                                            //add an item to the top of the stack
void *stack_pop(Stack *S);                                                          //return the data from the top of the stack and side effect the stack
int stack_search(Stack *S, void *data, int (*comparator)(void *data1, void *data2));//return index of item if in the stack, otherwise -1

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Stack *stack_make() {
  Stack *S = malloc(sizeof(Stack));
  S->top = NULL;
  S->size = 0;
  return S;
}

void stack_free(Stack *S, int free_data) {
  Frame *current = S->top;
  while(current != NULL) {
    S->top = current->next;
    if(free_data) { free(current->data); }
    free(current);
    current = S->top;
  }
  free(S);
}

void stack_walk(Stack *S, void (*f)(void *data)) {
  Frame *current = S->top;
  for(;current != NULL; current = current->next) { f(current->data); }
}

Stack *stack_map(Stack *S, void *(*f)(void *data)) {
  Stack *new_stk = malloc(sizeof(Stack));
  new_stk->size = S->size;
  Frame *current = S->top;
  Frame *new_frame, *prev;
  if(!stack_empty(S)) {
    new_frame = malloc(sizeof(Frame));
    new_frame->data = f(current->data);
    new_frame->next = NULL;
    new_stk->top = prev = new_frame;
    current = current->next;
  }
  for(;current != NULL; current = current->next) {
    new_frame = malloc(sizeof(Frame));
    new_frame->data = f(current->data);
    new_frame->next = NULL;
    prev = prev->next = new_frame;
  }
  return new_stk;
}

Stack *stack_push(Stack *S, void *data) {
  Frame *F = malloc(sizeof(Frame));
  F->data = data;
  F->next = S->top;
  S->top = F;
  S->size++;
  return S;
}

void *stack_pop(Stack *S) {
  Frame *current = S->top;
  void *data = current->data;
  S->top = current->next;
  free(current);
  S->size--;
  return data;
}

int stack_search(Stack *S, void *data, int (*comparator)(void *data1, void *data2)) {
  int wait_pos = 0;
  Frame *current = S->top;
  for(;current != NULL; current = current->next, wait_pos++) {
    if(comparator(data, current->data) == 0) { return wait_pos; }
  }
  return -1;
}
