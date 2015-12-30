
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 11/24/15
 Last Edited: 11/24/15
 
 A general purpose iterative vector library for arbitrary payloads
*/

/**********************************************************************

                  	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                  	    	 M A C R O S	

***********************************************************************/
#define vector_size(V) V->size
#define vector_ref(V, i, v_ptr) V->arr[i] = v_ptr 
#define vector_deref(V, i)  V->arr[i]

/**********************************************************************

                  	    		V E C T O R S

***********************************************************************/
typedef struct Vector {
  void **arr; //void * ptr to a ptr
  int size;
} Vector; // Vector Vector

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Vector *vector_make(int size);                                                        //make a new vector
void vector_free(Vector *V, int free_data);                                                          //free the vector
void vector_walk(Vector *V, void (*f)(void *data));                                   //walk over the vector and apply side-effect f to each elem
Vector *vector_map(Vector *V, void *(*f)(void *data));                                //return a new vector resulting from applying f to each elem
int vector_search(Vector *V, void *data, int (*comparator)(void *data1, void *data2));//return index of item if in the vector, otherwise -1

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S

***********************************************************************/
Vector *vector_make(int size) {
  Vector *V = malloc(sizeof(Vector));
  V->arr = malloc(sizeof(void *) * size);
  V->size = size;
  return V;
}

void vector_free(Vector *V, int free_keys) {
  int i = 0;
  if(free_keys) { for(;i < vector_size(V); i++) { free(vector_deref(V, i)); } }
  free(V->arr);
  V->arr = NULL;
  V->size = 0;
}

void vector_walk(Vector *V, void (*f)(void *data)) {
  int i = 0;
  for(;i < vector_size(V); i++) { f(vector_deref(V,i)); }
}

Vector *vector_map(Vector *V, void *(*f)(void *data)) {
  Vector *new_v = malloc(sizeof(Vector));
  new_v->arr = malloc(sizeof(void *) * vector_size(V));
  new_v->size = vector_size(V);
  int i = 0;
  for(;i < vector_size(V); i++) { vector_ref(new_v, i, f(vector_deref(V, i))); } 
  return new_v;
}

int vector_search(Vector *V, void *data, int (*comparator)(void *data1, void *data2)) {
  int i = 0;
  for(;i < vector_size(V); i++) { if(comparator(data, vector_deref(V, i)) == 0) { return i; } }
  return -1;
}
