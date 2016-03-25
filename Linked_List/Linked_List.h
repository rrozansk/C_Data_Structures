/**********************************************************************

                      F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 3/23/16
 Last Edited: 3/23/16
*/

#ifndef LINKED_LIST_DEFS
#define LINKED_LIST_DEFS

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define ls_size(L) L->size
#define ls_empty(L) !L->size

/**********************************************************************

            	    		L I N K E D   L I S T

***********************************************************************/
typedef struct ls_node {
  void *data; 
  struct ls_node *next;
} ls_node;

typedef struct List {
  ls_node *head;
  ls_node *tail;
  int size;
} List;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
List *ls_make();                                                                //make a new list
void ls_free(List *L, int free_data);                                           //free a list and optionally its data
void ls_walk(List *L, void (*f)(void *data));                                   //walk over the list and apply side-effect f to each elem
List *ls_map(List *L, void *(*f)(void *data));                                  //return a new list resulting from applying f to each elem
List *ls_cons(List *L, void *data);                                             //add an item to the front of the list 
List *ls_snoc(List *L, void *data);                                             //add an item to the end of the list
List *ls_insert_nth(List *L, int n, void *data);                                //add an item to any position in the list
List *ls_remove_nth(List *L, int n, int free_data);                             //remove any item in the list
int ls_search(List *L, void *data, int (*comparator)(void *data1, void *data2));//return index of item if in the list, otherwise -1
void *ls_ref(List *L, int i);                                                   //return data of i-th item in the list
List *ls_reverse(List *L);                                                      //reverse the list
List *ls_sort(List *L, int (*comparator)(void *data1, void *data2));            //sort the list given a function to compare two values (return -1[<],0[=],1[>])
//filter, append

#endif
