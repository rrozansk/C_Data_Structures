
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
  void(*printer)(void *data);
  Frame *top;
  int size;
} Stack; // Stack STRUCT

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Stack *stack_make(void(*printer)(void *data)); //make me a stack
void stack_free(Stack *S);                     //free the stack
void stack_delete(Stack *S);                   //delete the stack (free's data)
Stack *stack_copy(Stack *S);                   //copy the stack
Stack *stack_push(Stack *S, void *item);       //add an item to the top
void *stack_pop(Stack *S);                     //return the Frame at the top of the stack and side effect the stack
int stack_contains(Stack *S, void *data);      //returns whether the stack contains the data
void stack_print(Stack *S);                    //print out the stack

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Stack *stack_make(void(*printer)(void *data)) {
  Stack *S = malloc(sizeof(Stack));
  S->printer = printer;
  S->top = NULL;
  S->size = 0;
  return S;
}

void stack_free(Stack *S) {
  Frame *current = S->top;
  Frame *prev = S->top;
  while(current != NULL) {
    current = current->next;
    free(prev);
    current = prev;
  }
  S->printer = NULL;
  S->top = NULL;
  S->size = 0;
}

//combine with free like in trees with int for del keys
void stack_delete(Stack *S) {
  Frame *current = S->top;
  Frame *prev = S->top;
  while(current != NULL) {
    current = current->next;
    free(prev->data);
    free(prev);
    current = prev;
  }
  S->printer = NULL;
  S->top = NULL;
  S->size = 0;
}

//chane this to map
Stack *stack_copy(Stack *S) {
  Stack *new_stk = malloc(sizeof(Stack));
  new_stk->printer = S->printer;
  new_stk->size = S->size;
  if(S->size == 0) { new_stk->top = NULL; }
  else {
    Frame *new_top = malloc(sizeof(Frame));
    new_top->data = S->top->data;
    new_stk->top = new_top;
    Frame *current = S->top->next;
    Frame *prev = new_top;
    while(current != NULL) {
      Frame *new_frame = malloc(sizeof(Frame));
      new_frame->data = current->data;
      prev->next = new_frame;
      prev = new_frame;
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

//will need comparator to do this correctly
//1 if == 0 if not
int stack_contains(Stack *S, void *data) {
  int wait_pos = 0;
  Frame *current = S->top;
  while(current != NULL) {
    wait_pos++;
    if(current->data == data) { break; }
    current = current->next;
  }
  return wait_pos;
}

void stack_print(Stack *S) {
  Frame *F = S->top;
  while(F != NULL) {
    S->printer(F->data);
    F = F->next;
  }
}
