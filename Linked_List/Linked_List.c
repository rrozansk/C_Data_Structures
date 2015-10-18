
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/13/15
 
 A linked ls library for use with arbitrary data structures
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

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
List *make_ls(void (*printer)(void *data), int (*comparator)(void *data1, void *data2));
void delete_ls(List *L);
List *ls_insert_beginning(List *L, void *item);
List *ls_insert_after(List *L, void *ls_item, void *new_item);
List *ls_insert_end(List *L, void *item);
List *ls_remove(List *L, void *item);
int ls_contains(List *L, void *item);
int ls_length(List *L);
int ls_empty(List *L);
void ls_print(List *L); 
List *ls_append(List *L1, List *L2);
List *ls_reverse(List *L);
List *ls_sort(List *L);
ls_node *merge_sort(ls_node *head, int size, List *L);
ls_node *merge(ls_node *l, ls_node *s, List *L);
void *ls_ref(List *L, int i);
List *ls_set(List *L, int i, void *data);

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
List *make_ls(void (*printer)(void *data), int (*comparator)(void *data1, void *data2)) {
  List *L = malloc(sizeof(List));
  L->comparator = comparator;
  L->printer = printer;
  L->size = 0;
  L->head = NULL;
  L->tail = NULL;
  return L;
}

//need to fix so pointer to the next node are freed w/o mem map error
void delete_ls(List *L) {
  ls_node *current = L->head;
  ls_node *next;
  while(current != NULL) {
    next = current->next;
    free(current->data);
    //need to free current->next 
    free(current);
    current = next;
  }
  free(L->head);
  L->head = NULL;
  free(L->tail);
  L->tail = NULL;
  L->size = 0;
}

List *ls_insert_beginning(List *L, void *item) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = item;
  head->next = L->head;
  L->head = head;
  if(!L->size) L->tail = head;
  L->size = L->size++;
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

//should remove all?
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

int ls_length(List *L) {
  return L->size;
}

int ls_empty(List *L) {
  return L->size ? 0 : 1; 
}

void ls_print(List *L) {
  ls_node *current = L->head;
  while(current != NULL) {
    L->printer(current->data);
    current = current->next;
  }
}

//should i perform a deep copy and actually make a new list?
List *ls_append(List *L1, List *L2) {
  L1->tail->next = L2->head;
  L1->tail = L2->tail;
  L1->size = L1->size + L2->size;
  return L1;
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

//memory leak if i dont free old data. but what is someone else has it from ref
List *ls_set(List *L, int i, void *data) {
  int j = 0;
  ls_node *current = L->head;
  for(; j<i; j++) current = current->next;
  current->data = data;
  return L;
}
