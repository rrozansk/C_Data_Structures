
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
void tr_free(Tree *T);                                      //free the tree
void tr_delete(Tree *T);                                    //delete of each node of the tree (free's data)
Tree *tr_copy(Tree *T);                                     //copy the tree
Tree *tr_insert(Tree *T, void *key);                        //insert a key into a new node into the tree
Tree *tr_node_free(Tree *T, void *key);                     //free the node of the tree which contains key
Tree *tr_node_delete(Tree *T, void *key);                   //delete the node of the tree which contains key (free's data)
void *tr_maximum(tr_node *N);                               //return the biggest element in the tree
void *tr_minimum(tr_node *N);                               //return the smallest element in the tree
tr_node *tr_search(Tree *T, void *key);                     //return the node with given key from the tree
tr_node *tr_succ(tr_node *N);                               //the next decendant of a node
tr_node *tr_pred(tr_node *N);                               //the next ancestor of a node
int tr_height(Tree *T);                                     //find the height
int tr_size(Tree *T);                                       //returns the number of nodes in the tree
int tr_empty(Tree *T);                                      //return whether the tree is empty
void tr_print(Tree *T, int walk);                           //print out the tree
void tr_walk_pre(Tree *T, void (visitor)(void *key));       //3 different version of depth first search
void tr_walk_in(Tree *T, void (visitor)(void *key));
void tr_walk_post(Tree *T, void (visitor)(void *key));
void tr_breadth_first(Tree *T, void (visitor)(void *key));  //breadth first search of tree
Tree *tr_map(Tree *T, void (*func)(void *key));             //return a new tree 

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

//post order op
void tr_free(Tree *T) {
}

void tr_delete(Tree *T) {
}

Tree *tr_copy(Tree *T) {
}

Tree *tr_insert(Tree *T, void *key) {
}

Tree *tr_node_free(Tree *T, void *key) {
}

Tree *tr_node_delete(Tree *T, void *key) {
}

void *tr_maximum(tr_node *N) {
  if(N != NULL) {
    tr_node *current = N;
    while(current->right != NULL) {
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
    while(current->left != NULL) {
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
  if(N != NULL) { //null tr
    tr_node *current = N;
    if(current->parent == NULL) { //am root of tree
      current = current->right;
      if(current != NULL) { //right sub tr exists
        while(current->left != NULL)
          current = current->left;
        return current;
      }
    }
    else { //must have a parent
      current = current->parent; 
      if(current->right != NULL) { //right sub tr exists
        current = current->left; //something HERE idk if left
        while(current->right != NULL)
          current = current->right;
        return current;
      }
      return current;
    }
  }
  return NULL;

}

tr_node *tr_pred(tr_node *N) {
  if(N != NULL) { //null tr
    tr_node *current = N;
    if(current->parent == NULL) { //am root of tree
      current = current->left;
      if(current != NULL) { //left sub tr exists
        while(current->right != NULL)
          current = current->right;
        return current;
      }
    }
    else { //must have a parent
      if(current->parent->left == current) { //i am the left child
      
      }
      else { //i am the right child
        current = current->parent;
      }
      if(current->left != NULL) { //left sub tr exists
        current = current->left;
        while(current->right != NULL)
          current = current->right;
        return current;
      }
      return current;
    }
  }
  return NULL;
}

int tr_height(Tree *T) {
  tr_node *current = T->root;
  while(current != NULL) {
    tr_node *tmp = T->root;
    T->root = T->root->left;
    int lh = tr_height(T);
    T->root = tmp->right;
    int rh = tr_height(T);
    if(rh>=lh)
      return 1+rh;
    else
      return 1+lh;
  }
  return 0;
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
      tr_walk_pre(T, T->printer);
      break;
    case 0: // 0 (inorder)   --> L, root, R
      tr_walk_in(T, T->printer);
      break;
    case 1: // 1 (postorder) --> L, R, root
      tr_walk_post(T, T->printer);
      break;
  }
}

void tr_walk_pre(Tree *T, void(visitor)(void *key)) {
  tr_node *root = T->root;
  tr_node *current;
  while (root) {
    if (root->left == NULL) {
      visitor(root->key);
      root = root->right;
    }
    else {
      current = root->left;
      while (current->right && current->right != root)
        current = current->right;
      if (current->right == root) { 
        current->right
          =
          NULL;
        root
          =
          root->right;
      } 
      else {
        visitor(root->key);
        current->right = root;
        root = root->left;
      }
    }
  }
}

void tr_walk_in(Tree *T, void(visitor)(void *key)) {
  tr_node *current_parent = T->root;
  while(current_parent != NULL) {
    if(current_parent->left != NULL) current_parent = current_parent->left;
    else if(current_parent->right != NULL) current_parent = current_parent->right;
    else {
      while((current_parent->parent != NULL) && (current_parent != current_parent->parent->left))
        current_parent = current_parent->parent;
      if(current_parent->parent == NULL) break;
      else current_parent = current_parent->parent->right;
    }
  }
}

void tr_walk_post(Tree *T, void(visitor)(void *key)) {
}

void tr_breadth_first(Tree *T, void (visitor)(void *key)) {
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

//mutates the tree so data is replaced by void* returned from func
//for func just copy and call map
//should i delete old data?
Tree *tr_map(Tree *T, void (*func)(void *key)) {
  //just use same code as some walk but assign _->key to the retrun of func
}

