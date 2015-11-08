
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/27/15
 
 A linked ls library for use with arbitrary data structures
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define ls_null(L) !L->size
#define ls_length(L) L->size

/**********************************************************************

            	    		L I N K E D   L I S T

***********************************************************************/
typedef struct ls_node {
  void *data; 
  struct ls_node *next;
} ls_node;

typedef struct List {
  //must return -1 (d1 < d2), 0 (d1 == d2), 1 (d1 > d2)
  int (*comparator)(void *data1, void *data2); 
  void(*printer)(void *data);
  ls_node *head;
  ls_node *tail;
  int size;
} List;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
List *ls_make(void (*printer)(void *data), int (*comparator)(void *data1, void *data2));
void ls_free(List *L, int free_data);
List *ls_copy(List *L);                                           //REPLACE WITH MAP and get rid of printer in struct
List *ls_insert_beginning(List *L, void *item);
List *ls_insert_after(List *L, void *ls_item, void *new_item);
List *ls_insert_end(List *L, void *item);
List *ls_remove(List *L, void *item);                   //remove all/just 1st?
int ls_contains(List *L, void *item);
void ls_print(List *L); 
List *ls_append(List *L1, List *L2);
List *ls_reverse(List *L);
List *ls_sort(List *L);
ls_node *merge_sort(ls_node *head, int size, List *L); //should not be called by user
ls_node *merge(ls_node *l, ls_node *s, List *L);       //should not be called by user
void *ls_ref(List *L, int i);
int ls_index(List *L, void *data);
List *ls_set(List *L, int i, void *data);              //user must free old data if they are not using it elsewhere

//where is map!?

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
List *ls_make(void (*printer)(void *data), int (*comparator)(void *data1, void *data2)) {
  List *L = malloc(sizeof(List));
  L->comparator = comparator;
  L->printer = printer;
  L->size = 0;
  L->head = NULL;
  L->tail = NULL;
  return L;
}

void ls_free(List *L, int free_data) {
  ls_node *current = L->head;
  ls_node *prev = L->head;
  while(current != NULL) {
    current = current->next;
    if(free_data) { free(prev->data); }
    free(prev);
    prev = current;
  }
  L->printer = NULL;
  L->comparator = NULL;
  L->head = NULL;
  L->tail = NULL;
  L->size = 0;
}

List *ls_copy(List *L) {
  List *C = malloc(sizeof(List));
  C->printer = L->printer;
  C->comparator = L->comparator;
  C->size = L->size;
  if(L->size == 0) { C->head = C->tail = NULL; }
  else {
    ls_node *new_head = malloc(sizeof(ls_node));
    new_head->data = L->head->data;
    C->head = new_head;
    ls_node *prev = new_head;
    ls_node *current = L->head->next;
    while(current != NULL) {
      ls_node *new_node = malloc(sizeof(ls_node));
      new_node->data = current->data;
      prev->next = new_node;
      prev = new_node;
      current = current->next;
    }
    prev->next = NULL;
    C->tail = prev;
  }
  return C;
}

List *ls_insert_beginning(List *L, void *item) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = item;
  head->next = L->head;
  L->head = head;
  if(ls_null(L)) { L->tail = head; }
  L->size++;
  return L;
}  

List *ls_insert_after(List *L, void *ls_item, void *new_item) {
  ls_node *new_node = malloc(sizeof(ls_node));
  new_node->data = new_item;
  new_node->next = NULL;
  ls_node *current = L->head;
  while(current != NULL) {
    if(L->comparator(current->data, ls_item) == 0) {
      ls_node *rest = current->next;
      current->next = new_node;
      new_node->next = rest;
      if(rest == NULL) L->tail = new_node;
      L->size = L->size++;
      break;
    }
    current = current->next;
  }
  return L;
}

List *ls_insert_end(List *L, void *item) {
  ls_node *tail = malloc(sizeof(ls_node));
  tail->data = item;
  tail->next = NULL;
  if(L->size) L->tail->next = tail;
  L->tail = tail;
  if(!L->size) L->head = tail;
  L->size = L->size++;
  return L;
}

List *ls_remove(List *L, void *item) {
  ls_node *current = L->head;
  ls_node *prev = NULL;
  while(current != NULL) {
    if(L->comparator(current->data, item) == 0) {
      if(prev == NULL) { //removed the first thing in the list
        L->head = current->next;
      }
      else if(current->next == NULL) { //removed last thing in list
        prev->next = NULL;
        L->tail = prev;
      }
      else {
        prev->next = current->next;
      }
      free(current);
      L->size = L->size--;
      break;
    }
    prev = current;
    current = current->next;
  }
  return L;
}

int ls_contains(List *L, void *item) {
  ls_node *current = L->head;
  while(current != NULL) {
    if(L->comparator(current->data, item) == 0) return 1;
    current = current->next;
  }
  return 0;
}

void ls_print(List *L) {
  ls_node *current = L->head;
  while(current != NULL) {
    L->printer(current->data);
    current = current->next;
  }
}

List *ls_append(List *L1, List *L2) {
  if(L1->size == 0)
    return ls_copy(L2);
  if(L2->size == 0)
    return ls_copy(L1);
  else {
    List *l1 = ls_copy(L1);
    List *l2 = ls_copy(L2);
    List *L = malloc(sizeof(List));
    L->printer = L1->printer;
    L->comparator = L1->comparator;
    L->size = L1->size + L2->size;
    L->head = l1->head;
    L->tail = l2->tail;
    l1->tail->next = l2->head;
    free(l1);
    free(l2);
    return L;
  }
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

ls_node *merge(ls_node *l, ls_node *s, List *L) {
  if(l == NULL)
    return s;
  if(s == NULL)
    return l;
  int comp = L->comparator(l->data,s->data);
  if(comp <= 0) { //comp == 0 || comp == -1
    l->next = merge(l->next, s, L);
    return l;
  }
  else { //comp == 1
    s->next = merge(l, s->next, L);
    return s;
  }
}

ls_node *merge_sort(ls_node *head, int size, List *L) {
  if(size <= 1) {
    return head;
  }
  else {
    int i = 0;
    int new_size = size/2;
    ls_node *new_head = head;
    for(;i<new_size-1;i++) 
      new_head = new_head->next;
    ls_node *tmp = new_head;
    new_head = new_head->next;
    tmp->next = NULL;
    return merge(merge_sort(head, new_size, L), merge_sort(new_head, size-new_size, L), L);
  }
}

List *ls_sort(List *L) {
  L->head = merge_sort(L->head, L->size, L);
  L->tail = L->head;
  while(L->tail->next != NULL) {
    L->tail = L->tail->next;
  }
  return L;
}

void *ls_ref(List *L, int i) {
  int j = 0;
  ls_node *current = L->head;
  for(; j<i; j++) current = current->next;
  return current->data;
}

int ls_index(List *L, void *data) {
  int i = 0;
  int found = 0;
  ls_node *current = L->head;
  while(current != NULL) {
    if(L->comparator(current->data, data) == 0) {
      found = 1;
      break;
    }
    else {
      i += 1;
      current = current->next;
    }
  }
  if(found) 
    return i;
  else
    return -1;
}

List *ls_set(List *L, int i, void *data) {
  int j = 0;
  ls_node *current = L->head;
  for(; j<i; j++) current = current->next;
  current->data = data;
  return L;
}
