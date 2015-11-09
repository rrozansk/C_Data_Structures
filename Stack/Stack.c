
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/18/15
 
 A library for general use of stacks for arbitrary data structures (payloads)
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
#define stack_null(S) !S->size
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
Stack *stack_make();                                                                   //make me a stack
void stack_free(Stack *S, int free_data);                                              //free the stack
void stack_walk(Stack *S, void (*visitor)(void *data));                                //aply visitor to everything in the stack
Stack *stack_map(Stack *S, void *(*f)(void *data));                                    //map over the stack
Stack *stack_push(Stack *S, void *item);                                               //add an item to the top
void *stack_pop(Stack *S);                                                             //return the Frame at the top of the stack and side effect the stack
int stack_contains(Stack *S, int (*comparator)(void *data1, void *data2), void *data); //returns whether the stack contains the data

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
  Frame *trash;
  while(current != NULL) {
    trash = current;
    current = current->next;
    if(free_data) { free(trash->data); }
    free(trash);
  }
  S->top = NULL;
  S->size = 0;
}

void stack_walk(Stack *S, void (*visitor)(void *data)) {
    Frame *current = S->top;
    while(current != NULL) {
      visitor(current->data);
      current = current->next;
    }
}

Stack *stack_map(Stack *S, void *(*visitor)(void *data)) {
  Stack *new_stk = malloc(sizeof(Stack));
  new_stk->size = S->size;
  if(S->size == 0) { new_stk->top = NULL; }
  else {
    Frame *current = S->top;
    while(current != NULL) {
      Frame *new_frame = malloc(sizeof(Frame));
      new_frame->data = visitor(current->data);
      new_frame->next = new_stk->top;
      new_stk->top = new_frame;
      current = current->next;
    }
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

int stack_contains(Stack *S, int (*comparator)(void *data1, void *data2), void *data) {
  int wait_pos = 0;
  Frame *current = S->top;
  while(current != NULL) {
    if(comparator(data, current->data) == 0) { return wait_pos; }
    wait_pos++;
    current = current->next;
  }
  return -1;
}
