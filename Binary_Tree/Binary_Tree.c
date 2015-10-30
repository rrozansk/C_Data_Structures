
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/7/15
 Last Edited: 10/29/15
 
 Just a simple binary tree library
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include <string.h>
#include "../Queue/Queue.c"

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define PREORDER -1
#define INORDER 0
#define POSTORDER 1
#define tr_null(T) !T->size
#define tr_size(T) T->size

/**********************************************************************

            	    	B I N A R Y   T R E E S	

***********************************************************************/
typedef struct tr_node {
  void *key;
  struct tr_node *left;
  struct tr_node *right;
  struct tr_node *parent;
} tr_node;

typedef struct Tree {
  int (*comparator)(void *key1, void *key2);
  tr_node *root;
  int size;
} Tree;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
tr_node *tr_node_make(void *key);                             //make a tree node given a key
Tree *tr_node_free(Tree *T, void *key, int free_key);         //free the node, and optionally its key
Tree *tr_make(int (*comparator)(void *key1, void* key2));
void tr_free(Tree *T, int free_keys);                         //free the tree, and optionally the keys
Tree *tr_map(Tree *T, void *(*f)(void *key));                 //traverse the tree while making a new one

void *tr_maximum(tr_node *N);                                 //return the biggest element in the tree
void *tr_minimum(tr_node *N);                                 //return the smallest element in the tree
tr_node *tr_search(Tree *T, void *key);                       //return the node with given key from the tree
Tree *tr_insert(Tree *T, void *key);                          //insert a key into a new node into the tree
//tr_node *tr_succ(tr_node *N);                               //
//tr_node *tr_pred(tr_node *N);                               //
int tr_height(tr_node *node);                                 //return the height of the tree

Tree *tr_walk(Tree *T, int walk, void (*visitor)(void *key));  //depth first search -1(pre),0(in),1(post) -> here visitor can also mutate val@key
void tr_breadth_first(Tree *T, void (*visitor)(tr_node *key)); //breadth first search of tree

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
tr_node *tr_node_make(void *key) {
  tr_node *new_node = malloc(sizeof(tr_node));
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;
  new_node->key = key;
  return new_node;
}

//not done yet
Tree *tr_node_free(Tree *T, void *key, int free_key) {
  tr_node *found = tr_search(T, key);
  if(found != NULL) {
    tr_node *prev = found->parent;
    tr_node *l_sub = found->left;
    tr_node *r_sub = found->right;
    //... need succ and pred
  }
  return T;
}

Tree *tr_make(int (*comparator)(void *key1, void *key2)) {
  Tree *T = malloc(sizeof(Tree));
  T->comparator = comparator;
  T->root = NULL;
  T->size = 0;
  return T;
}

void tr_free(Tree *T, int free_keys) {
  tr_node *current = T->root;
  char state_flag = -1;
  tr_node *prev;
  while(current) {
    switch(state_flag) {
      case -1: //go left 
        if(current->left != NULL) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit
        if(current->parent != NULL && current->parent->left == current) { state_flag = 1; }
        prev = current; //save node we are one
        current = current->parent; 
        if(free_keys) free(prev->key); 
        free(prev); //move the node we were on
        break;
      default: //case 1:  //go right
        if(current->right != NULL) { 
          current = current->right;
          state_flag = -1; 
        }
        else { state_flag = 0; }
        break;
    }
  }
  T->size = 0;
  T->root = NULL;
}

Tree *tr_map(Tree *T, void *(*f)(void *key)) {
  Tree *T_copy = tr_make(T->comparator);
  tr_node *current = T->root;
  tr_node *tmp, *parent = NULL;
  char state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        tmp = tr_node_make(f(current->key)); //map over the node
        tmp->parent = parent;
        if(parent != NULL) {
          if(parent->left == NULL) { parent->left = tmp; }
          else { parent->right = tmp; }
        }
        parent = tmp;
        if(current->left != NULL) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit
        if(current->parent != NULL) { 
          tmp = parent = tmp->parent;
          if(current->parent->left == current) { state_flag = 1; }
        }
        current = current->parent;
        break;
      default: //case 1:  //go right
        if(current->right != NULL) {
          current = current->right;
          state_flag = -1; 
        }
        else { state_flag = 0; }
        break;
    }
  }
  T_copy->size = T->size;
  T_copy->root = parent;
  return T_copy;
}

void *tr_maximum(tr_node *current) {
  while(current->right != NULL) { current = current->right; }
  return current->key;
}

void *tr_minimum(tr_node *current) {
  while(current->left != NULL) { current = current->left; }
  return current->key;
}

tr_node *tr_search(Tree *T, void *key) {
  if(T->root != NULL) {
    tr_node *current = T->root;
    int comp;
    while(current != NULL) {
      comp = T->comparator(key, current->key);
      if(comp == -1) { current = current->left; }
      else if(comp == 1) { current = current->right; }
      else { return current->key; }
    }
  }
  else { return NULL; }
}

Tree *tr_insert(Tree *T, void *key) {
  if(T->root == NULL) { T->root = tr_node_make(key); }
  else {
    tr_node *parent, *current = T->root;
    int comp;
    while(current != NULL) {
      comp = T->comparator(key, current->key); 
      parent = current;
      if(comp == -1) { current = current->left; }
      else if(comp == 1) { current = current->right; }
      else { return T; } // (comp == 0) no duplicates allowed
    }
    current = tr_node_make(key);
    current->parent = parent;
    if(comp == -1) { parent->left = current; }
    else { parent->right = current; }
  }
  T->size++;
  return T;
}

tr_node *tr_succ(tr_node *N) {
}

tr_node *tr_pred(tr_node *N) {
}

//rec def --> can i make this iterative without using another data structure?
int tr_height(tr_node *node) {
  if(node != NULL) {
    int lh = tr_height(node->left);
    int rh = tr_height(node->right);
    return (rh >= lh) ? 1+rh : 1+lh;
  }
  else { return 0; }
}

Tree *tr_walk(Tree *T, int walk, void(visitor)(void *key)) {
  tr_node *current = T->root;
  char state_flag = -1;
  while(current != NULL) {
    switch(state_flag) {
      case -1: //go left
        if(walk == PREORDER) { visitor(current->key); }
        if(current->left != NULL) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit, go up
        if(walk == POSTORDER) { visitor(current->key); }
        if(current->parent != NULL && current->parent->left == current) { state_flag = 1; }
        current = current->parent;
        break;
      default: //case 1:  //go right
        if(walk == INORDER) { visitor(current->key); }
        if(current->right != NULL) {
          current = current->right;
          state_flag = -1;
        }
        else { state_flag = 0; }
        break;
    }
  }
}

void tr_breadth_first(Tree *T, void (visitor)(tr_node *key)) {
  if(T->size) {
    Queue *Q = queue_make((void *)NULL); //this should be fine since i wont print it
    queue_enqueue(Q, T->root);
    tr_node *current;
    while(!queue_empty(Q)) {
      current = queue_dequeue(Q);
      visitor(current);
      if(current->left != NULL) queue_enqueue(Q, current->left);
      if(current->right != NULL) queue_enqueue(Q, current->right);
    }
    queue_free(Q);
  }
}
