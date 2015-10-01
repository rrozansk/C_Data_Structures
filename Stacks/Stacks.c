
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 9/30/15
 
 A library for general use (pointers to void*) of stacks 
*/

#include <stdlib.h>

/**********************************************************************

                  	    		S T A C K S

***********************************************************************/
typedef struct Frames {
  void *data;
  struct Frames *next;
} Frame;

typedef struct Stacks {
  void(*printer)(void *data);
  Frame *head;
  int size;
} Stack; // Stack STRUCT

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Stack *make_stack(void(*printer)(void *data)); //make me a stack
void push(Stack *S, void *item);               //add an item to the top
void *pop(Stack *S);                          //return the Frame at the top of the stack and side effect the stack
void *peek(Stack *S);                         //return the Frame at the top (without removing it)
int stack_size(Stack *S);                            //return the number of Frames in the stack
int is_empty(Stack *S);                         //is the stack empty?
void stack_print(Stack *S); //print out the stack

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Stack *make_stack(void(*printer)(void *data)) {
  Stack *S = malloc(sizeof(Stack));
  S->printer = printer;
  S->head = NULL;
  S->size = 0;
  return S;
}

void push(Stack *S, void *data) {
  Frame *F = malloc(sizeof(Frame));
  F->data = data;
  F->next = S->head;
  S->head = F;
  S->size = S->size++;
}

void *pop(Stack *S) {
  void *data = S->head->data;
  S->head = S->head->next;
  S->size = S->size--;
  return data;
}

void *peek(Stack *S) {
  return S->head->data;
}

int stack_size(Stack *S) {
  return S->size;
}

int is_empty(Stack *S) {
  return S->size ? 0 : 1;
}

void stack_print(Stack *S) {
  if(S->size) {
    Frame *F = S->head;
    while(F != NULL) {
      S->printer(F->data);
      F = F->next;
    }
  }
}
