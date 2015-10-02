
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
#include "../Stack/Stack.c"

/**********************************************************************

            	    	B I N A R Y   T R E E S	

***********************************************************************/
typedef struct Node {
  void *key_value;
  struct Node *l;
  struct Node *r;
  struct Node *parent;
} Node;

typedef struct Tree {
  int (*comparator)(void *key1, void *key2);
  void (*printer)(void *key);
  Node *root;
  int size;
  int height;
} Tree;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Tree *make_tr(int (*comparator)(void *key1, void* key2), void (*printer)(void *key));
void delete_tr(Tree *T, void *key);     //delete of each node of the tree
void tr_insert(Tree *T, void *key);     //insert a key into a new node into the tree
void tr_delete(Tree *T);                //delete the node of the tree which contains key

Node *tr_lookup(Tree *T, void *key);    //return the node with given key from the tree
Node *succ(Node *N);                    //the next decendant of a node
Node *pred(Node *N);                    //the next ancestor of a node

int tr_contains(Tree *T, void *key);    //does the tree contain the key? 
int tr_height(Tree *T);                 //find the height
int tr_size(Tree *T);                   //returns the number of nodes in the tree
int tr_empty(Tree *T);                  //return whether the tree is empty
void tr_print(Tree *T, int walk);       //print out the tree

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N T 

***********************************************************************/
Tree *make_tree(int (*comparator)(void *key1, void *key2), void (*printer)(void *key)) {
  Tree *T = malloc(sizeof(Tree));
  T->comparator = comparator;
  T->printer = printer;
  T->root = NULL;
  T->size = 0;
  T->height = 0;
  return T;
}

void delete_tr(Tree *T, void *key) {
}

void tr_insert(Tree *T, void *key) {
}

void tr_delete(Tree *T) {
}

int tr_contains(Tree *T, void *key) {
}

int tr_empty(Tree *T) {
  return T->size ? 0 : 1;
}

int tr_height(Tree *T) {
}

Node *tr_lookup(Tree *T, void *key) {
}

Node *succ(Node *N) {
}

Node *pred(Node *N) {
}

// -1 (preorder) --> root, L, R
// 0 (inorder)   --> L, root, R
// 1 (postorder) --> L, R, root
void tr_print(Tree *T, int walk) {
  Stack *S = make_stack(T->printer);
  Node *current = T->root;
  while(current != NULL) {
    //
  }
}
