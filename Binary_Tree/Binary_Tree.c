
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/7/15
 Last Edited: 10/27/15
 
 Just a simple binary tree library on which to build other tree data 
 structures with similiar properties including AVL, red black, and BST's
 it is also general purpose use for any data structure you would like to store
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
  void (*printer)(void *key);
  tr_node *root;
  int size;
} Tree;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Tree *tr_make(int (*comparator)(void *key1, void* key2), void (*printer)(void *key));
void tr_free(Tree *T, int free_keys);                         //free the tree, and optionally the keys
Tree *tr_copy(Tree *T, int key_size);                         //copy the tree
Tree *tr_insert(Tree *T, void *key);                          //insert a key into a new node into the tree
tr_node *tr_node_make(void *key);
//Tree *tr_node_free(Tree *T, void *key);                     //free the node of the tree which contains key
void *tr_maximum(tr_node *N);                                 //return the biggest element in the tree
void *tr_minimum(tr_node *N);                                 //return the smallest element in the tree
tr_node *tr_search(Tree *T, void *key);                       //return the node with given key from the tree
//tr_node *tr_succ(tr_node *N);                               //the next decendant of a node
//tr_node *tr_pred(tr_node *N);                               //the next ancestor of a node
int tr_height(tr_node *node);                                 //find the height
int tr_is_empty(Tree *T);                                     //return whether the tree is empty
Tree *tr_walk(Tree *T, int walk, void (visitor)(void *key));  //3 different version of depth first search -> here visitor can also mutate val@key
//Tree *tr_map(Tree *T, int walk, void (*make_key)(void *key)); //return a new tree 
void tr_breadth_first(Tree *T, void (visitor)(tr_node *key)); //breadth first search of tree

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
Tree *tr_make(int (*comparator)(void *key1, void *key2), void (*printer)(void *key)) {
  Tree *T = malloc(sizeof(Tree));
  T->comparator = comparator;
  T->printer = printer;
  T->root = NULL;
  T->size = 0;
  return T;
}

void tr_free(Tree *T, int free_keys) {
  tr_node *current = T->root;
  int state_flag = -1;
  T->size = 0;
  while(current) {
    switch(state_flag) {
      case -1: //go left 
        if(current->left) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit
        if(current->parent == NULL) {
          if(free_keys) { free(current->key); }
          free(current);
          current = NULL;
          T->root = NULL;
        }
        else if(current->parent->right == current) { 
          current = current->parent; 
          if(free_keys) { free(current->right->key); }
          free(current->right);
        }
        else  { //(current->parent->left == current) 
          current = current->parent; 
          if(free_keys) { free(current->left->key); }
          free(current->left);
          state_flag = 1;
        } 
        break;
      default: //case 1:  //go right
        if(current->right) { 
          current = current->right;
          state_flag = -1; 
        }
        else { state_flag = 0; }
        break;
    }
  }
}

Tree *tr_copy(Tree *T, int key_size) { //key size is in bytes
  Tree *T_copy = tr_make(T->comparator, T->printer);
  T_copy->size = T->size;
  tr_node *current = T->root;
  tr_node *new_node_left, *new_node_right;
  void *new_key;
  int state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        if(current->left) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit
        if(current->parent == NULL || current->parent->right == current) { current = current->parent; }
        else { //(current->parent->left == current)
          new_node_left = malloc(key_size);
          memcpy(new_key, current->key, key_size);
          new_node_left = tr_node_make(new_key);
          current = current->parent; 
          state_flag = 1;
        }
        break;
      default: //case 1:  //go right
        if(current->right) { 
          current = current->right;
          state_flag = -1; 
        }
        else { state_flag = 0; }
        break;
    }
  }
  return T_copy;
}

Tree *tr_insert(Tree *T, void *key) {
  T->size++;
  tr_node *new_node = tr_node_make(key);
  if (T->root == NULL)
    T->root = new_node;
  else {
    tr_node *parent = NULL;
    tr_node *current = T->root;
    int comp;
    while(current != NULL) {
      comp = T->comparator(key, current->key); 
      if (comp == -1) {
        parent = current;
        current = current->left;
      }
      else if (comp == 0) {
        return T; //no duplicates allowed
      }
      else { // (comp == 1) 
        parent = current;
        current = current->right;
      }
    }
    if(comp == -1) parent->left = new_node;
    else parent->right = new_node;
    new_node->parent = parent;
  }
  return T;
}

tr_node *tr_node_make(void *key) {
  tr_node *new_node = malloc(sizeof(tr_node));
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;
  new_node->key = key;
  return new_node;
}

Tree *tr_node_free(Tree *T, void *key) {
}

void *tr_maximum(tr_node *N) {
  if(N != NULL) {
    tr_node *current = N;
    while(current != NULL) {
      N = current;
      current = current->right;
    }
    return N->key;
  }
  return NULL;
}

void *tr_minimum(tr_node *N) {
  if(N != NULL) {
    tr_node *current = N;
    while(current != NULL) {
      N = current; 
      current = current->left;
    }
    return N->key;
  }
  return NULL;
}

tr_node *tr_search(Tree *T, void *key) {
  if(T->root != NULL) {
    tr_node *current = T->root;
    while(current != NULL) {
      int comp = T->comparator(key, current->key);
      if(comp == -1) {
        current = current->left;
      }
      else if(comp == 0) {
        return current->key;
      }
      else { //comp == 1
        current = current->right;
      }
    }
  }
  else {
    return NULL;
  }
}

tr_node *tr_succ(tr_node *N) {
}

tr_node *tr_pred(tr_node *N) {
}

//rec def
int tr_height(tr_node *node) {
  if(node != NULL) {
    int lh = tr_height(node->left);
    int rh = tr_height(node->right);
    return (rh >= lh) ? 1+rh : 1+lh;
  }
  else { return 0; }
}

int tr_is_empty(Tree *T) { return !T->size; } //idk if useful or not

Tree *tr_walk(Tree *T, int walk, void(visitor)(void *key)) {
  tr_node *current = T->root;
  int state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        if(walk == PREORDER) visitor(current->key);
        if(current->left) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit
        if(walk == POSTORDER) visitor(current->key);
        if(current->parent == NULL || current->parent->right == current) { current = current->parent; }
        else { //(current->parent->left == current)
          current = current->parent; 
          state_flag = 1;
        }
        break;
      default: //case 1:  //go right
        if(walk == INORDER) visitor(current->key);
        if(current->right) { 
          current = current->right;
          state_flag = -1; 
        }
        else { state_flag = 0; }
        break;
    }
  }
}

Tree *tr_map(Tree *T, int walk, void (*make_key)(void *key)) {
}

void tr_breadth_first(Tree *T, void (visitor)(tr_node *key)) {
  if(T->size) {
    Queue *Q = queue_make(T->printer);
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
