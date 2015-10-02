
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 9/30/15
 
 A library for general use of stacks for arbitrary data structures (payloads)
*/

/**********************************************************************

                  	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

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
Stack *make_stack(void(*printer)(void *data)); //make me a stack
void delete_stack(Stack *S);                   //delete the stack
void push(Stack *S, void *item);               //add an item to the top
void *pop(Stack *S);                           //return the Frame at the top of the stack and side effect the stack
void *peek(Stack *S);                          //return the Frame at the top (without removing it)
int stack_contains(Stack *S, void *data);      //returns whether the stack contains the data
int stack_size(Stack *S);                      //return the number of Frames in the stack
int stack_empty(Stack *S);                     //is the stack empty?
void stack_print(Stack *S);                    //print out the stack

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Stack *make_stack(void(*printer)(void *data)) {
  Stack *S = malloc(sizeof(Stack));
  S->printer = printer;
  S->top = NULL;
  S->size = 0;
  return S;
}

void delete_stack(Stack *S) {
  Frame *current = S->top;
  while(current != NULL) {
    Frame *temp = current->next;
    free(current);
    current = temp;
  }
  free(S);
}

void push(Stack *S, void *data) {
  Frame *F = malloc(sizeof(Frame));
  F->data = data;
  F->next = S->top;
  S->top = F;
  S->size = S->size++;
}

void *pop(Stack *S) {
  void *data = S->top->data;
  S->top = S->top->next;
  S->size = S->size--;
  return data;
}

void *peek(Stack *S) {
  return S->top->data;
}

int stack_contains(Stack *S, void *data) {
  int wait_pos = 0;
  Frame *current = S->top;
  while(current != NULL) {
    wait_pos += 1;
    if(current->data == data) break;
    current = current->next;
  }
  return wait_pos;
}

int stack_size(Stack *S) {
  return S->size;
}

int stack_empty(Stack *S) {
  return S->size ? 0 : 1;
}

void stack_print(Stack *S) {
  Frame *F = S->top;
  while(F != NULL) {
    S->printer(F->data);
    F = F->next;
  }
}
