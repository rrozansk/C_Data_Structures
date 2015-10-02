
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 9/24/15
 
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
  ls_node *tail; //makes append O(1) w/ single extra pointer overhead 
  int size;
} List;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
List *make_ls(void (*printer)(void *data));
void delete_ls(List *L);
void ls_insert_after(List *L, void *ls_item, void *new_item);
void ls_insert_beggining(List *L, void *item);
void ls_insert_end(List *L, void *item);
void ls_remove_item(List *L, void *item);
void ls_print(List *L); 
int ls_length(List *L);
void ls_append(List *L1, List *L2);
//void ls_reverse(List *L);
//int ls_contains(List *L, void *item);
//int ls_empty(List *L);
//void ls_sort(List *L, int (*comparator)(void *data1, void *data2));
//void *ls_ref(List *L, int i);
//void ls_set(List *L, int i, void *data);

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
List *make_ls(void (*printer)(void *key)) {
  List *L = malloc(sizeof(List));
  L->printer = printer;
  L->size = 0;
  L->head = NULL;
  L->tail = NULL;
  return L;
}

void insert_after(List *L, void *ls_item, void *new_item) {
  if(L->head != NULL) { // make sure ls is not empty
    ls_node *head = L->head;
    while(head->data != ls_item) {// stop once on node N or last element of ls
      if(head->next == NULL) {
        head = NULL;
        break; //stop if next is NULL -- didnt find ls_item in L
      }
      head = head->next;
    }
    if(head != NULL) { // foud node N is ls L
      ls_node *rest = head->next;
      ls_node *new_node = malloc(sizeof(ls_node));
      new_node->data = new_item;
      new_node->next = rest;
      head->next = new_node;
    }
  }
}

void insert_beggining(List *L, void *item) {
  ls_node *head = malloc(sizeof(ls_node));
  head->data = item;
  if(L->head == NULL) { // ls is NULL
    head->next = NULL;
    L->head = head;
    L->tail = head;
  }
  else {
    head->next = L->head;
    L->head = head;
  }
  L->size = L->size++;
}  

void insert_end(List *L, void *item) {
  ls_node *new_node = malloc(sizeof(ls_node));
  new_node->data = item;
  new_node->next = NULL;
  if(L->size) {
    L->head = new_node;
    L->tail = new_node;
  }
  else {
    L->tail->next = new_node;
    L->tail = new_node;
  }
  L->size = L->size++;
}

//finish implementing this function
void remove_item(List *L, void *item) {
  if(L->size) {
    ls_node *current = L->head;
    int removed = 0;
    while(current != NULL) {
     //check for pointer eq and size issues like removing singleton item 
    }
    if(removed) L->size = L->size--;
  }
}

void ls_print(List *L) {
  if(L->size) {
    ls_node *current = L->head;
    while(current != NULL) {
      L->printer(current->data);
      current = current->next;
    }
  }
}

int ls_length(List *L) {
  return L->size;
}

void ls_append(List *L1, List *L2) {
  //hopefully the lss have the same type bc of print function 
  L1->tail->next = L2->head;
  L1->tail = L2->tail;
  L1->size = L1->size + L2->size;
}
