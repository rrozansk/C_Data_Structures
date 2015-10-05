
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 10/5/15
 
 Just a simple linked ls library where every node in the ls points to the
 data instead of storing it itself. This allows the user to store there own
 data structures in a linked ls and just point to them. 
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
  void(*printer)(void *data);
  ls_node *head;
  ls_node *tail;
  int size;
} List;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
List *make_ls(void (*printer)(void *data));
void delete_ls(List *L);
void ls_insert_beggining(List *L, void *item);
void ls_insert_after(List *L, void *ls_item, void *new_item);
void ls_insert_end(List *L, void *item);
void ls_remove_item(List *L, void *item);
int ls_contains(List *L, void *item);
int ls_length(List *L);
int ls_empty(List *L);
void ls_print(List *L); 
void ls_append(List *L1, List *L2);
void ls_reverse(List *L);
void ls_sort(List *L, int (*comparator)(void *data1, void *data2));
void *ls_ref(List *L, int i);
void ls_set(List *L, int i, void *data);

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
List *make_ls(void (*printer)(void *data)) {
  List *L = malloc(sizeof(List));
  L->printer = printer;
  L->size = 0;
  L->head = NULL;
  L->tail = NULL;
  return L;
}

void delete_ls(List *L) {
  ls_node *current = L->head;
  while(current != NULL) {
    ls_node *next = current->next;
    free(current);
    current = next;
  }
  free(L);
}

void insert_beggining(List *L, void *item) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = item;
  head->next = L->head;
  L->head = head;
  if(!L->size) L->tail = head;
  L->size = L->size++;
}  

void insert_after(List *L, void *ls_item, void *new_item) {
  ls_node *new_node = malloc(sizeof(ls_node));
  new_node->data = new_item;
  new_node->next = NULL;
  ls_node *current = L->head;
  while(current != NULL) {
    if(current->data == ls_item) {
      ls_node *rest = current->next;
      current->next = new_node;
      new_node->next = rest;
      if(rest == NULL) L->tail = new_node;
      L->size = L->size++;
      break;
    }
    current = current->next;
  }
}

void insert_end(List *L, void *item) {
  ls_node *tail = malloc(sizeof(ls_node));
  tail->data = item;
  tail->next = NULL;
  if(L->size) L->tail->next = tail;
  L->tail = tail;
  if(!L->size) L->head = tail;
  L->size = L->size++;
}

void remove_item(List *L, void *item) {
  ls_node *current = L->head;
  ls_node *prev = L->head;
  while(current != NULL) {
    if(current->data == item) {
      prev->next = current->next;
      free(current);
      L->size = L->size--;
      break;
    }
    prev = current;
    current = current->next;
  }
}

int ls_contains(List *L, void *item) {
  ls_node *current = L->head;
  while(current != NULL) {
    if (current->data == item) return 1;
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
void ls_append(List *L1, List *L2) {
  L1->tail->next = L2->head;
  L1->tail = L2->tail;
  L1->size = L1->size + L2->size;
}

void ls_reverse(List *L) {
  ls_node *old_ls = L->head;
  ls_node *new_ls = NULL;
  while(old_ls != NULL) {
    ls_node *head = old_ls;
    old_ls = old_ls->next;
    head->next = new_ls;
    new_ls = head;
  }
  L->tail = L->head;
  L->head = new_ls;
}

// ...ikd if this is a good idea...
ls_node *merge_sort(List *L, int start, int end) {
}

ls_node *merge(ls_node *L1, ls_node *L2) {
}

//merge sort --> maybe use quick sort. it is inplace although merge sort seems
//better bc we will be jumping around memory instead of just being in an array
void ls_sort(List *L, int (*comparator)(void *data1, void *data2)) {
  int subls_size = L->size/2;
  ls_node *L1 = merge_sort(L, 0, subls_size);
  ls_node *L2 = merge_sort(L, subls_size+1, L->size);
  L->head = merge(L1, L2);
}

void *ls_ref(List *L, int i) {
  int j = 0;
  ls_node *current = L->head;
  while(j < i) current = current->next;
  return current->data;
}

void ls_set(List *L, int i, void *data) {
  int j = 0;
  ls_node *current = L->head;
  while(j < i) current = current->next;
  current->data = data;
}
