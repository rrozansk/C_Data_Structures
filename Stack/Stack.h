/**********************************************************************

                      F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 3/23/16
 Last Edited: 3/23/16
*/

#ifndef STACK_DEFS
#define STACK_DEFS

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

#endif
