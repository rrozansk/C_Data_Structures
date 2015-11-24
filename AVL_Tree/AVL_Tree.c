
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************
 
 Author: Ryan Rozanski
 Created: 9/6/15
 Last Edited: 11/13/15
 
 Just a simple AVL tree library
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
#define avl_size(T) T->size
#define avl_empty(T) !T->size
#define avl_peek(T) T->root->data
#define avl_height(T) T->root->height //problem if tree is null

/**********************************************************************

            	    	B I N A R Y   T R E E S	

***********************************************************************/
typedef struct avl_node {
  void *key;
  int height;
  struct avl_node *left;
  struct avl_node *right;
  struct avl_node *parent;
} avl_node;

typedef struct AVL {
  int (*comparator)(void *key1, void *key2);
  avl_node *root;
  int size; //nodes in the tree
} AVL;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
AVL *avl_make(int (*comparator)(void *key1, void* key2));//make a new AVL with given a comparator
void avl_free(AVL *T, int free_keys);                    //free the AVL and optionally its keys
void avl_breadth_first(AVL *T, void (*f)(avl_node *key));//breadth first search of AVL
//void avl_a*(AVL *T, void (*f)(avl_node *key));           //
//void avl_best_first(AVL *T, void (*f)(avl_node *key));   //
void avl_walk(AVL *T, int walk, void (*f)(void *key));   //walk over the AVL [depth first search pre,in,post] and apply side-effect f to each elem 
AVL *avl_map(AVL *T, void *(*f)(void *key));             //return a new AVL resulting from applying f to each elem

//helpers to maintain balance
int avl_balance_factor(avl_node *current);               //return the difference in size of the two sub trees
void avl_fix_height(avl_node *current);                  //after a rotation some node heights need readjusting
avl_node *avl_rotate_left(AVL *T, avl_node *current);
avl_node *avl_rotate_right(AVL *T, avl_node *current);
avl_node *avl_balance(AVL *T, avl_node *current);        //after insertion or deletion rebalance may be needed

AVL *avl_insert(AVL *T, void *key);                      //add an item to the AVL, no duplicates allowed
AVL *avl_remove(AVL *T, avl_node *root, int free_key);   //remove the node with the given key from the AVL
avl_node *avl_search(AVL *T, void *key);                 //return the node which contains key if found in AVL, NULL otherwise
avl_node *avl_minimum(avl_node *root);                   //return the smallest element in the AVL
avl_node *avl_maximum(avl_node *root);                   //return the biggest element in the AVL
avl_node *avl_succ(avl_node *root);                      //find the succ of the AVL
avl_node *avl_pred(avl_node *root);                      //find the pref of the AVL

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
AVL *avl_make(int (*comparator)(void *key1, void *key2)) {
  AVL *T = malloc(sizeof(AVL));
  T->comparator = comparator;
  T->root = NULL;
  T->size = 0;
  return T;
}

void avl_free(AVL *T, int free_keys) {
  avl_node *current = T->root;
  signed char state_flag = -1;
  avl_node *prev;
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

void avl_breadth_first(AVL *T, void (*f)(avl_node *key)) {
  Queue *Q = queue_make();
  queue_enqueue(Q, T->root);
  avl_node *current;
  while(!queue_empty(Q)) {
    current = queue_dequeue(Q);
    f(current);
    if(current->left != NULL) { queue_enqueue(Q, current->left); }
    if(current->right != NULL) { queue_enqueue(Q, current->right); }
  }
  queue_free(Q, 0);
}

void avl_walk(AVL *T, int walk, void (*f)(void *key)) {
  avl_node *current = T->root;
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

AVL *avl_map(AVL *T, void *(*f)(void *key)) {
  AVL *T_copy = avl_make(T->comparator);
  avl_node *current = T->root;
  avl_node *tmp, *parent = NULL; //parent will become our new tree, tmp is to make new nodes
  signed char state_flag = -1;
  while(current) {
    switch(state_flag) {
      case -1: //go left
        tmp = malloc(sizeof(avl_node));
        tmp->left = NULL;
        tmp->right = NULL;
        tmp->key = f(current->key); //map over the node
        tmp->height = current->height;
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

int avl_balance_factor(avl_node *current) {
  if(current->left == NULL && current->right == NULL) { return 0; }
  else if(current->left == NULL) { return -current->right->height; }
  else if(current->right == NULL) { return current->left->height; }
  else { return current->left->height - current->right->height; }
}

void avl_fix_height(avl_node *current) {
  int l,r;
  (current->left == NULL) ? (l = -1) : (l = current->left->height);
  (current->right == NULL) ? (r = -1) : (r = current->right->height);
  (l >= r) ? (current->height = (l + 1)) : (current->height = (r + 1));
}

avl_node *avl_rotate_left(AVL *T, avl_node *root) {
  avl_node *pivot = root->right;
  root->right = pivot->left;
  if(pivot->left != NULL) { pivot->left->parent = root; }
  pivot->parent = root->parent;
  if(root->parent == NULL) { T->root = pivot; }
  else if(root == root->parent->left) { root->parent->left = pivot; }
  else { root->parent->right = pivot; }
  pivot->left = root;
  root->parent = pivot;
  avl_fix_height(root);
  avl_fix_height(pivot);
  return pivot;
}

avl_node *avl_rotate_right(AVL *T, avl_node *root) {
  avl_node *pivot = root->left;
  root->left = pivot->right;
  if(pivot->right != NULL) { pivot->right->parent = root; }
  pivot->parent = root->parent;
  if(root->parent == NULL) { T->root = pivot; }
  else if(root == root->parent->right) { root->parent->right = pivot; }
  else { root->parent->left = pivot; }
  pivot->right = root;
  root->parent = pivot;
  avl_fix_height(root);
  avl_fix_height(pivot);
  return pivot;
}

avl_node *avl_balance(AVL *T, avl_node *n) {
  int diff = avl_balance_factor(n);
  if(diff < -1) { //tree is right heavy
    if(avl_balance_factor(n->right) < 0) { //sub tree is right heavy
      n = avl_rotate_left(T, n);
    }
    else { 
      n->right = avl_rotate_right(T, n->right);
      n = avl_rotate_left(T, n);
    }
  }
  else if(diff > 1) { //tree is left heavy
    if(avl_balance_factor(n->left) > 0) { //sub tree is left heavy
      n = avl_rotate_right(T, n);
    }
    else { 
      n->left =  avl_rotate_left(T, n->left);
      n = avl_rotate_right(T, n);
    }
  }
  //else tree is balanced
  return n;
}

AVL *avl_insert(AVL *T, void *key) {
  if(T->root == NULL) { 
    T->root = malloc(sizeof(avl_node));
    T->root->left = NULL;
    T->root->right = NULL;
    T->root->parent = NULL;
    T->root->height = 0;
    T->root->key = key; 
  }
  else {
    avl_node *parent, *current = T->root;
    int comp;
    while(current != NULL) {
      comp = T->comparator(key, current->key); 
      parent = current;
      if(comp == -1) { current = current->left; }
      else if(comp == 1) { current = current->right; }
      else { return T; } // (comp == 0) no duplicates allowed
    }
    current = malloc(sizeof(avl_node));
    current->left = NULL;
    current->right = NULL;
    current->parent = parent;
    current->height = 0;
    current->key = key;
    (comp == -1) ? (parent->left = current) : (parent->right = current);
    // SHOULD FIND FIRST SPOT WHICH WILL BECOME UNBALANCED AND THEN ONLY BALANCE
    // THAT NODE, FOR AVL WE HAVE ATMOST 1 ROTATION TO PERFORM ON AN INSERT
    while(current->parent != NULL) { //go up, fix heights, performing needed rotations
      current = current->parent;
      avl_fix_height(current);
      current = avl_balance(T, current);
    }
  }
  T->size++;
  return T;
}

AVL *avl_remove(AVL *T, avl_node *N, int free_key) {
  if(N == NULL) { return T; } //cannot remove if not contained 
  if(N->left != NULL && N->right != NULL) { //2 children
    avl_node *succ = avl_minimum(N->right); //will have at most only 1 child (right)
    void *key = N->key;
    N->key = succ->key;
    succ->key = key;
    N = succ;
  }
  avl_node *val = NULL;
  if(N->right == NULL && N->left != NULL) { //1 child (LEFT)
    N->left->parent = N->parent;
    val = N->left;
  }
  else if(N->left == NULL && N->right != NULL) { //1 child (RIGHT)
    N->right->parent = N->parent;
    val = N->right;
  } //else N is a leaf, now fix up N's parent pointer
  if(N->parent != NULL) { (N == N->parent->left) ? (N->parent->left = val) : (N->parent->right = val); }
  else { T->root = val; }
  val = N->parent; //first node whose height might have changed
  if(free_key) { free(N->key); }
  free(N);
  while(val != NULL) { 
    avl_fix_height(val);
    val = avl_balance(T, val);
    val = val->parent;
  }
  T->size--;
  return T;
}

avl_node *avl_search(AVL *T, void *key) {
  avl_node *current = T->root;
  int comp;
  while(current != NULL) {
    comp = T->comparator(key, current->key);
    if(comp == -1) { current = current->left; }
    else if(comp == 1) { current = current->right; }
    else { break; }
  }
  return current;
}

avl_node *avl_minimum(avl_node *current) {
  while(current->left != NULL) { current = current->left; }
  return current;
}

avl_node *avl_maximum(avl_node *current) {
  while(current->right != NULL) { current = current->right; }
  return current;
}

avl_node *avl_succ(avl_node *current) {
  if(current->right != NULL) { return avl_minimum(current->right); }
  else {
    avl_node *parent = current->parent;
    while(parent != NULL && parent->right == current) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}

avl_node *avl_pred(avl_node *current) {
  if(current->left != NULL) { return avl_maximum(current->left); }
  else {
    avl_node *parent = current->parent;
    while(parent != NULL && parent->left == current) {
      current = parent;
      parent = parent->parent;
    }
    return parent;
  }
}
