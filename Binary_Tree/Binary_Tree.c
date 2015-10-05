
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 9/7/15
 Last Edited: 9/29/15
 
 Just a simple binary tree library on which to build other tree data 
 structures with similiar properties including AVL, red black, and BST's
 it is also general purpose use for any data structure you would like to store
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include "../Queue/Queue.c"

/**********************************************************************

            	    	B I N A R Y   T R E E S	

***********************************************************************/
typedef struct tr_node {
  void *key;
  struct tr_node *l;
  struct tr_node *r;
  struct tr_node *parent;
} tr_node;

typedef struct Tree {
  int (*comparator)(void *key1, void *key2);
  void (*printer)(void *key);
  tr_node *root;
  int size;
  int height;
} Tree;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Tree *make_tr(int (*comparator)(void *key1, void* key2), void (*printer)(void *key));
void delete_tr(Tree *T);                //delete of each node of the tree
void tr_insert(Tree *T, void *key);     //insert a key into a new node into the tree
void tr_delete(Tree *T, void *key);     //delete the node of the tree which contains key
tr_node *tr_lookup(Tree *T, void *key); //return the node with given key from the tree
tr_node *tr_succ(tr_node *N);           //the next decendant of a node
tr_node *tr_pred(tr_node *N);           //the next ancestor of a node
int tr_contains(Tree *T, void *key);    //does the tree contain the key? 
int tr_height(Tree *T);                 //find the height
int tr_size(Tree *T);                   //returns the number of nodes in the tree
int tr_empty(Tree *T);                  //return whether the tree is empty
void tr_print(Tree *T, int walk);       //print out the tree
void tr_walk_pre(Tree *T);
void tr_walk_in(Tree *T);
void tr_walk_post(Tree *T);

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N T 

***********************************************************************/
Tree *make_tr(int (*comparator)(void *key1, void *key2), void (*printer)(void *key)) {
  Tree *T = malloc(sizeof(Tree));
  T->comparator = comparator;
  T->printer = printer;
  T->root = NULL;
  T->size = 0;
  T->height = 0;
  return T;
}

void delete_tr(Tree *T) {
}

void tr_insert(Tree *T, void *key) {
}

void tr_delete(Tree *T, void *key) {
}

tr_node *tr_lookup(Tree *T, void *key) {
}

tr_node *tr_succ(tr_node *N) {
}

tr_node *tr_pred(tr_node *N) {
}

int tr_contains(Tree *T, void *key) {
}

int tr_height(Tree *T) {
  return T->height;
}

int tr_size(Tree *T) {
  return T->size;
}

int tr_empty(Tree *T) {
  return T->size ? 0 : 1;
}

void tr_print(Tree *T, int walk) {
  switch(walk) {
    case -1: // -1 (preorder) --> root, L, R
      tr_walk_pre(T);
      break;
    case 0: // 0 (inorder)   --> L, root, R
      tr_walk_in(T);
      break;
    case 1: // 1 (postorder) --> L, R, root
      tr_walk_post(T);
      break;
  }
}

void tr_walk_pre(Tree *T) {
  Queue *Q = make_queue(T->printer);
  if(T->size) {
    queue_enqueue(Q, T->root);
    while(!queue_empty) {
      tr_node *tr_root = queue_dequeue(Q);
      if(tr_root->l != NULL) queue_enqueue(Q, tr_root->l);
      if(tr_root->r != NULL) queue_enqueue(Q, tr_root->r);
      T->printer(tr_root);
    }
  }
  delete_queue(Q);
}

void tr_walk_in(Tree *T) {
  Queue *Q = make_queue(T->printer);
  if(T->size) {
    if(T->root->l != NULL) queue_enqueue(Q, T->root->l);
    while(!queue_empty) {
      tr_node *tr_root = queue_dequeue(Q);
      if(tr_root->l != NULL) queue_enqueue(Q, tr_root->l);
      if(tr_root->l != NULL) queue_enqueue(Q, tr_root->r);
      T->printer(tr_root);
    }
  }
  delete_queue(Q);
}

void tr_walk_post(Tree *T) {
  Queue *Q = make_queue(T->printer);
  if(T->size) {
    queue_enqueue(Q, T->root);
    while(!queue_empty) {
      tr_node *tr_root = queue_dequeue(Q);
      if(tr_root->l != NULL) queue_enqueue(Q, tr_root->l);
      if(tr_root->l != NULL) queue_enqueue(Q, tr_root->r);
      T->printer(tr_root);
    }
  }
  delete_queue(Q);
}
