
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/7/15
 Last Edited: 11/9/15
 
 A general purpose iterative BST library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include "../Queue/Queue.c"

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define PREORDER -1
#define INORDER 0
#define POSTORDER 1
#define bst_size(T) T->size
#define bst_empty(T) !T->size
#define bst_peek(T) T->root->data

/**********************************************************************

            	B I N A R Y   S E A R C H   T R E E S	

***********************************************************************/
typedef struct bst_node {
  void *key;
  struct bst_node *left;
  struct bst_node *right;
  struct bst_node *parent;
} bst_node;

typedef struct BST {
  int (*comparator)(void *key1, void *key2);
  bst_node *root;
  int size; //nodes in the tree
} BST;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
BST *bst_make(int (*comparator)(void *key1, void* key2));//make a new BST with given a comparator
void bst_free(BST *T, int free_keys);                    //free the BST and optionally its keys
void bst_breadth_first(BST *T, void (*f)(bst_node *key));//breadth first search of BST
//void bst_a*(BST *T, void (*f)(bst_node *key));           //
//void bst_best_first(BST *T, void (*f)(bst_node *key));   //
void bst_walk(BST *T, int walk, void (*f)(void *key));   //walk over the BST [depth first search pre,in,post] and apply side-effect f to each elem 
BST *bst_map(BST *T, void *(*f)(void *key));             //return a new BST resulting from applying f to each elem
BST *bst_insert(BST *T, void *key);                      //add an item to the BST, no duplicates allowed
BST *bst_remove(BST *T, void *key, int free_key);        //remove the node with the given key from the BST
bst_node *bst_search(BST *T, void *key);                 //return the node which contains key if found in BST, NULL otherwise
bst_node *bst_minimum(bst_node *root);                   //return the smallest element in the BST
bst_node *bst_maximum(bst_node *root);                   //return the biggest element in the BST
bst_node *bst_succ(bst_node *root);                      //find the succ of the BST
bst_node *bst_pred(bst_node *root);                      //find the pref of the BST
int bst_height(BST *T);                                  //return the height of the BST

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
BST *bst_make(int (*comparator)(void *key1, void *key2)) {
  BST *T = malloc(sizeof(BST));
  T->comparator = comparator;
  T->root = NULL;
  T->size = 0;
  return T;
}

void bst_free(BST *T, int free_keys) {
  bst_node *current = T->root;
  signed char state_flag = -1;
  bst_node *prev;
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

void bst_breadth_first(BST *T, void (*f)(bst_node *key)) {
  Queue *Q = queue_make();
  queue_enqueue(Q, T->root);
  bst_node *current;
  while(!queue_empty(Q)) {
    current = queue_dequeue(Q);
    f(current);
    if(current->left != NULL) { queue_enqueue(Q, current->left); }
    if(current->right != NULL) { queue_enqueue(Q, current->right); }
  }
  queue_free(Q, 0);
}

void bst_walk(BST *T, int walk, void (*f)(void *key)) {
  bst_node *current = T->root;
  signed char state_flag = -1;
  while(current != NULL) {
    switch(state_flag) {
      case -1: //go left
        if(walk == PREORDER) { f(current->key); }
        if(current->left != NULL) { current = current->left; }
        else { state_flag = 1; }
        break;
      case 0: //visit, go up
        if(walk == POSTORDER) { f(current->key); }
        if(current->parent != NULL && current->parent->left == current) { state_flag = 1; }
        current = current->parent;
        break;
      default: //case 1:  //go right
        if(walk == INORDER) { f(current->key); }
        if(current->right != NULL) {
          current = current->right;
          state_flag = -1;
        }
        else { state_flag = 0; }
        break;
    }
  }
}

BST *bst_map(BST *T, void *(*f)(void *key)) {
  BST *T_copy = bst_make(T->comparator);
  bst_node *current = T->root;
  bst_node *tmp, *parent = NULL; //parent will become our new tree, tmp is to make new nodes
  signed char state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        tmp = malloc(sizeof(bst_node));
        tmp->left = NULL;
        tmp->right = NULL;
        tmp->key = f(current->key); //map over the node
        tmp->parent = parent;
        if(parent != NULL) {
          if(T->comparator(tmp->key, parent->key) == -1) { parent->left = tmp; } //fixed problem but idk if i like this...can i make this better if using this method?
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

BST *bst_insert(BST *T, void *key) {
  if(T->root == NULL) { 
    T->root = malloc(sizeof(bst_node));
    T->root->left = NULL;
    T->root->right = NULL;
    T->root->parent = NULL;
    T->root->key = key; 
  }
  else {
    bst_node *parent, *current = T->root;
    int comp;
    while(current != NULL) {
      comp = T->comparator(key, current->key); 
      parent = current;
      if(comp == -1) { current = current->left; }
      else if(comp == 1) { current = current->right; }
      else { return T; } // (comp == 0) no duplicates allowed
    }
    current = malloc(sizeof(bst_node));
    current->left = NULL;
    current->right = NULL;
    current->key = key;
    current->parent = parent;
    (comp == -1) ? (parent->left = current) : (parent->right = current);
  }
  T->size++;
  return T;
}

BST *bst_remove(BST *T, void *key, int free_key) {
  bst_node *trash = bst_search(T, key);
  if(trash == NULL) { return T; } //cannot remove if not contained 
  if(trash->left != NULL && trash->right != NULL) { //2 children
    bst_node *succ = bst_maximum(trash); //will have at most only 1 child
    if(succ->left == NULL) { succ->parent->right = NULL; }
    else {  //has child (LEFT) 
      succ->parent->right = succ->left; 
      succ->left->parent = succ->parent;
    }
    key = trash->key;
    trash->key = succ->key;
    succ->key = key;
    trash = succ;
  }
  else {
    bst_node *val;
    if(trash->left == NULL && trash->right == NULL) { val = NULL; } //if no children just fix up pointers
    else if(trash->right == NULL) { //1 child (LEFT)
      trash->left->parent = trash->parent;
      val = trash->left;
    }
    else { //1 child (RIGHT) (trash->left == NULL)
      trash->right->parent = trash->parent;
      val = trash->right;
    }
    //fix up tree
    if(trash->parent != NULL) {
      if(trash == trash->parent->left) { trash->parent->left = val; }
      else { trash->parent->right = val; }
    }
    else { T->root = val; }
  }
  if(free_key) { free(trash->key); }
  free(trash);
  T->size--;
  return T;
}

bst_node *bst_search(BST *T, void *key) {
  bst_node *current = T->root;
  int comp;
  while(current != NULL) {
    comp = T->comparator(key, current->key);
    if(comp == -1) { current = current->left; }
    else if(comp == 1) { current = current->right; }
    else { break; }
  }
  return current;
}

bst_node *bst_minimum(bst_node *current) {
  while(current->left != NULL) { current = current->left; }
  return current;
}

bst_node *bst_maximum(bst_node *current) {
  while(current->right != NULL) { current = current->right; }
  return current;
}

bst_node *bst_succ(bst_node *current) {
  if(current->right != NULL) { return bst_minimum(current->right); }
  else {
    bst_node *parent = current->parent;
    while(parent != NULL && parent->right == current) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

bst_node *bst_pred(bst_node *current) {
  if(current->left != NULL) { return bst_maximum(current->left); }
  else {
    bst_node *parent = current->parent;
    while(parent != NULL && parent->left == current) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

int bst_height(BST *T) {
  if(T->size) {
    Queue *Q = queue_make(NULL);
    queue_enqueue(Q, T->root);
    bst_node *current;
    int node_count, bst_height = 0;
    while(1) {
      node_count = Q->size;
      if(node_count == 0) { return bst_height; }
      else { bst_height++; }
      for(;node_count > 0;node_count--) {
        current = queue_dequeue(Q);
        if(current->left != NULL) { queue_enqueue(Q, current->left); }
        if(current->right != NULL) { queue_enqueue(Q, current->right); }
      }
    }
  }
  else { return 0; }
}
