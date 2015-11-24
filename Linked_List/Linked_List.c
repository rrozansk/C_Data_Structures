
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 11/9/15
 
 A general purpose linked list library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

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
//filter

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
List *ls_make() {
  List *L = malloc(sizeof(List));
  L->size = 0;
  L->head = NULL;
  L->tail = NULL;
  return L;
}

void ls_free(List *L, int free_data) {
  ls_node *current = L->head;
  while(current != NULL) {
    L->head = current->next;
    if(free_data) { free(current->data); }
    free(current);
    current = L->head;
  }
  L->head = NULL;
  L->tail = NULL;
  L->size = 0;
}

void ls_walk(List *L, void (*f)(void *data)) {
  ls_node *current = L->head;
  for(;current != NULL; current = current->next) { f(current->data); }
}

List *ls_map(List *L, void *(*f)(void *data)) {
  List *C = malloc(sizeof(List));
  C->size = L->size;
  ls_node *current = L->head;
  ls_node *new_node;
  if(!ls_empty(L)) {
    new_node = malloc(sizeof(ls_node));
    new_node->data = f(current->data);
    new_node->next = NULL;
    C->head = C->tail = new_node;
    current = current->next;
  }
  for(;current != NULL; current = current->next) {
    new_node = malloc(sizeof(ls_node));
    new_node->data = f(current->data);
    new_node->next = NULL;
    C->tail = C->tail->next = new_node;
  }
  return C;
}

List *ls_cons(List *L, void *data) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = data;
  if(ls_empty(L)) { 
    head->next = NULL;
    L->head = L->tail = head; 
  }
  else { 
    head->next = L->head;
    L->head = head; 
  }
  L->size++;
  return L;
}

List *ls_snoc(List *L, void *data) {
  ls_node *tail = malloc(sizeof(ls_node));
  tail->data = data;
  tail->next = NULL;
  if(ls_empty(L)) { L->head = L->tail = tail; }
  else { L->tail = L->tail->next = tail; }
  L->size++;
  return L;
}

List *ls_insert_nth(List *L, int n, void *data) {
  if(n == 0) { return ls_cons(L, data); }
  else if(n == ls_size(L)) { return ls_snoc(L, data); }
  else {
    ls_node *current = L->head;
    for(;n>1;n--) { current = current->next; }
    ls_node *new_node = malloc(sizeof(ls_node));
    new_node->data = data;
    new_node->next = current->next;
    current->next = new_node;
    L->size++;
    return L;
  }
}

List *ls_remove_nth(List *L, int n, int free_data) {
  ls_node *current = L->head;
  if(n == 0) { //removing first item
    if(ls_size(L) == 1) { L->head = L->tail = NULL; }
    else { L->head = L->head->next; }
  }
  else if(n == ls_size(L)-1) { //removing last item
    for(;current->next->next != NULL; current = current->next);
    L->tail = current;
    current = current->next;
    L->tail->next = NULL;
  }
  else { //else removing some other item between 2 items
    for(;n>1; n--, current = current->next);
    ls_node *tmp = current;
    current = current->next;
    tmp->next = current->next;
  }
  if(free_data) { free(current->data); }
  free(current);
  L->size--;
  return L;
}

int ls_search(List *L, void *data, int (*comparator)(void *data1, void *data2)) {
  int pos = 0;
  ls_node *current = L->head;
  for(;current != NULL; current = current->next, pos++) {
    if(comparator(current->data, data) == 0) { return pos; }
  }
  return -1;
}

void *ls_ref(List *L, int i) {
  ls_node *current = L->head;
  for(; i>0; i--) { current = current->next; }
  return current->data;
}

List *ls_reverse(List *L) {
  ls_node *old_ls = L->head;
  ls_node *new_ls = NULL;
  ls_node *head;
  while(old_ls != NULL) {
    head = old_ls;
    old_ls = old_ls->next;
    head->next = new_ls;
    new_ls = head;
  }
  L->tail = L->head;
  L->head = new_ls;
  return L;
}

List *ls_sort(List *L, int (*comparator)(void *data1, void *data2)) {
  int listSize = 1, numMerges, leftSize, rightSize;
  ls_node *list = L->head, *tail,*left,*right,*next;
  do {
    numMerges = 0;
    left = list;
    tail = list = NULL;
    while(left != NULL) {
      numMerges++;
      right = left;
      leftSize = 0;
      rightSize = listSize;
      while(right != NULL && leftSize < listSize) { leftSize++,right = right->next; }
      while(leftSize > 0 || (rightSize > 0 && right != NULL)) {
        if(leftSize == 0) {next = right; right = right->next; rightSize--; }
        else if(rightSize == 0 || right == NULL) {next = left; left = left->next; leftSize--; }
        else if(comparator(left->data,right->data) < 0) {next = left; left = left->next; leftSize--; }
        else {next = right; right = right->next; rightSize--; }
        if(tail != NULL) { tail->next = next; }
        else { list = next; }
        tail = next;
      }
      left = right;
    }
    if(tail != NULL) { tail->next = NULL; }
    listSize <<= 1;
  } while(numMerges > 1);
  L->head = list;
  L->tail = tail;
  return L;
}
