
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 11/8/15
 
 A linked ls library for use with arbitrary data structures
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
#define ls_peek(L) L->head->data

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
List *ls_make();
void ls_free(List *L, int free_data);
void ls_walk(List *L, void (*visitor)(void *data));
List *ls_map(List *L, void *(*f)(void *data));
List *ls_cons(List *L, void *item);
List *ls_snoc(List *L, void *item);
//cdr
int ls_contains(List *L, int (*comparator)(void *data1, void *data2) , void *item); //return index of item or -1 if not comtained in list

//List *ls_remove_nth_item(List *L, int n);
List *ls_remove(List *L, void *item, int (*comparator)(void *data1, void *data2), int free_data);

List *ls_reverse(List *L);

List *ls_sort(List *L, int (*comparator)(void *data1, void *data2));
ls_node *merge_sort(ls_node *head, int size, List *L, int (*comparator)(void *data1, void *data2)); //should not be called by user
ls_node *merge(ls_node *l, ls_node *s, List *L, int (*comparator)(void *data1, void *data2));       //should not be called by user

void *ls_ref(List *L, int i);
void ls_set(List *L, int i, void *data);              //user must free old data if they are not using it elsewhere

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

void ls_walk(List *L, void (*visitor)(void *data)) {
  ls_node *current = L->head;
  for(;current != NULL; current = current->next) { visitor(current->data); }
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

List *ls_cons(List *L, void *item) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = item;
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

List *ls_snoc(List *L, void *item) {
  ls_node *tail = malloc(sizeof(ls_node));
  tail->data = item;
  tail->next = NULL;
  if(ls_empty(L)) { L->head = L->tail = tail; }
  else { L->tail = L->tail->next = tail; }
  L->size++;
  return L;
}

List *ls_remove(List *L, void *item, int (*comparator)(void *data1, void *data2), int free_data) {
  ls_node *current = L->head;
  ls_node *prev = NULL;
  for(;current != NULL; current = current->next) {
    if(comparator(current->data, item) == 0) {
      //removed the first thing in the list
      if(prev == NULL) { L->head = current->next; }
      //removed last thing in list
      else if(current->next == NULL) { 
        prev->next = NULL;
        L->tail = prev;
      }
      else { prev->next = current->next; }
      if(free_data) {free(current->data); }
      free(current);
      L->size--;
    }
    prev = current;
  }
  return L;
}

int ls_contains(List *L, int (*comparator)(void *data1, void *data2), void *item) {
  int pos = 0;
  ls_node *current = L->head;
  for(;current != NULL; current = current->next, pos++) {
    if(comparator(current->data, item) == 0) { return pos; }
  }
  return -1;
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

ls_node *merge(ls_node *l, ls_node *s, List *L, int (*comparator)(void *data1, void *data2)) {
  if(l == NULL) { return s; } 
  if(s == NULL) { return l; }
  int comp = comparator(l->data,s->data);
  if(comp <= 0) { //comp == 0 || comp == -1
    l->next = merge(l->next, s, L, comparator);
    return l;
  }
  else { //comp == 1
    s->next = merge(l, s->next, L, comparator);
    return s;
  }
}

ls_node *merge_sort(ls_node *head, int size, List *L, int (*comparator)(void *data1, void *data2)) {
  if(size <= 1) { return head; }
  else {
    int i = 0;
    int new_size = size/2;
    ls_node *new_head = head;
    for(;i<new_size-1;i++) { new_head = new_head->next; }
    ls_node *tmp = new_head;
    new_head = new_head->next;
    tmp->next = NULL;
    return merge(merge_sort(head, new_size, L, comparator), 
                 merge_sort(new_head, size-new_size, L, comparator), 
                 L, comparator);
  }
}

List *ls_sort(List *L, int (*comparator)(void *data1, void *data2)) {
  L->head = merge_sort(L->head, L->size, L, comparator);
  L->tail = L->head;
  while(L->tail->next != NULL) {
    L->tail = L->tail->next;
  }
  return L;
}

void *ls_ref(List *L, int i) {
  int j = 0;
  ls_node *current = L->head;
  for(; j<i; j++) { current = current->next; }
  return current->data;
}

void ls_set(List *L, int i, void *data) {
  int j = 0;
  ls_node *current = L->head;
  for(; j<i; j++) { current = current->next; }
  current->data = data;
}
