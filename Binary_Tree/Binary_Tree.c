
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/7/15
 Last Edited: 10/29/15
 
 A binary search tree library, with no recursion. everything is
 implemented iteratively, using queues where necessary. 
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
Tree *tr_node_delete(Tree *T, void *key, int free_key);       //free the node, and optionally its key, and fix up the tree
Tree *tr_make(int (*comparator)(void *key1, void* key2));     //make a BST with given a comparator
void tr_delete(Tree *T, int free_keys);                       //free the tree, and optionally the keys
Tree *tr_map(Tree *T, void *(*f)(void *key));                 //traverse the tree while making a new one, user is responsible for malloc of new key!!
tr_node *tr_maximum(tr_node *current);                        //return the biggest element in the tree
tr_node *tr_minimum(tr_node *current);                        //return the smallest element in the tree
tr_node *tr_search(Tree *T, void *key);                       //return the key if found or NULL
Tree *tr_insert(Tree *T, void *key);                          //insert a key into the tree, no duplicates allowed
tr_node *tr_succ(tr_node *current);                           //
tr_node *tr_pred(tr_node *current);                           //
int tr_height(Tree *T);                                       //return the height of the tree
Tree *tr_walk(Tree *T, int walk, void (*visitor)(void *key)); //depth first search pre,in,post -> here visitor can also mutate val@key
void tr_breadth_first(Tree *T, void (*visitor)(tr_node *key));//breadth first search of tree

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
Tree *tr_node_delete(Tree *T, void *key, int free_key) {
  tr_node *found = tr_search(T, key); //should search return node or key
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

void tr_delete(Tree *T, int free_keys) {
  tr_node *current = T->root;
  signed char state_flag = -1;
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
  tr_node *tmp, *parent = NULL; //parent will become our new tree, tmp is to make new nodes
  signed char state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        tmp = tr_node_make(f(current->key)); //map over the node
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

tr_node *tr_maximum(tr_node *current) {
  while(current->right != NULL) { current = current->right; }
  return current;
}

tr_node *tr_minimum(tr_node *current) {
  while(current->left != NULL) { current = current->left; }
  return current;
}

tr_node *tr_search(Tree *T, void *key) {
  tr_node *current = T->root;
  int comp;
  while(current != NULL) {
    comp = T->comparator(key, current->key);
    if(comp == -1) { current = current->left; }
    else if(comp == 1) { current = current->right; }
    else { break; }
  }
  return current;
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

tr_node *tr_succ(tr_node *current) {
  if(current->right != NULL) { return tr_minimum(current->right); }
  else {
    tr_node *parent = current->parent;
    while(parent != NULL && current == parent->right) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

tr_node *tr_pred(tr_node *current) {
  if(current->left != NULL) { return tr_maximum(current->left); }
  else {
    tr_node *parent = current->parent;
    while(parent != NULL && current == parent->left) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

int tr_height(Tree *T) {
  if(T->size) {
    Queue *Q = queue_make(NULL);
    queue_enqueue(Q, T->root);
    tr_node *current;
    int node_count, tr_height = 0;
    while(1) {
      node_count = Q->size;
      if(node_count == 0) { return tr_height; }
      else { tr_height++; }
      while(node_count > 0) {
        current = queue_dequeue(Q);
        if(current->left != NULL) { queue_enqueue(Q, current->left); }
        if(current->right != NULL) { queue_enqueue(Q, current->right); }
        node_count--;
      }
    }
  }
  else { return 0; }
}

Tree *tr_walk(Tree *T, int walk, void (*visitor)(void *key)) {
  tr_node *current = T->root;
  signed char state_flag = -1;
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
  return T;
}

void tr_breadth_first(Tree *T, void (*visitor)(tr_node *key)) {
  if(T->size) {
    Queue *Q = queue_make(NULL); //this should be fine since i wont print it
    queue_enqueue(Q, T->root);
    tr_node *current;
    while(!queue_empty(Q)) {
      current = queue_dequeue(Q);
      visitor(current);
      if(current->left != NULL) { queue_enqueue(Q, current->left); }
      if(current->right != NULL) { queue_enqueue(Q, current->right); }
    }
    queue_free(Q);
  }
}
