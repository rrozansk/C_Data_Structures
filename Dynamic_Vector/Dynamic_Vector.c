
/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 12/30/15
 Last Edited: 12/30/15
 
 A general purpose iterative dynamic vector library for arbitrary payloads
*/

/**********************************************************************

                  	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>

/**********************************************************************

                  	    	 M A C R O S	

***********************************************************************/
#define dvector_size(V) V->size
#define dvector_ref(V, i, v_ptr) V->arr[i] = v_ptr 
#define dvector_deref(V, i)  V->arr[i]
#define ARR_INIT_SIZE 16

/**********************************************************************

                  	    		V E C T O R S

***********************************************************************/
typedef struct Dynamic_Vector {
  void **arr; //void * ptr to a ptr
  int arr_size;
  int size;
} Dynamic_Vector;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Dynamic_Vector *dvector_make();                                                                //make a new vector
void dvector_free(Dynamic_Vector *V, int free_data);                                           //free the vector
void dvector_walk(Dynamic_Vector *V, void (*f)(void *data));                                   //walk over the vector and apply side-effect f to each elem
Dynamic_Vector *dvector_map(Dynamic_Vector *V, void *(*f)(void *data));                        //return a new vector resulting from applying f to each elem
Dynamic_Vector *dvector_insert_end(Dynamic_Vector *V, void *data);
Dynamic_Vector *dvector_insert_nth(Dynamic_Vector *V, int i, void *data);
Dynamic_Vector *dvector_remove_nth(Dynamic_Vector *V, int i, int free_data);
int dvector_search(Dynamic_Vector *V, void *data, int (*comparator)(void *data1, void *data2));//return index of item if in the vector, otherwise -1

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S

***********************************************************************/
Dynamic_Vector *dvector_make() {
  Dynamic_Vector *V = malloc(sizeof(Dynamic_Vector));
  V->arr = malloc(sizeof(void *) * ARR_INIT_SIZE);
  V->arr_size = ARR_INIT_SIZE;
  V->size = 0;
  return V;
}

void dvector_free(Dynamic_Vector *V, int free_keys) {
  int i = 0;
  if(free_keys) { for(;i < dvector_size(V); i++) { free(dvector_deref(V, i)); } }
  free(V->arr);
  V->arr = NULL;
  V->size = 0;
  free(V);
}

void dvector_walk(Dynamic_Vector *V, void (*f)(void *data)) {
  int i = 0;
  for(;i < dvector_size(V); i++) { f(dvector_deref(V,i)); }
}

Dynamic_Vector *dvector_map(Dynamic_Vector *V, void *(*f)(void *data)) {
  Dynamic_Vector *new_v = malloc(sizeof(Dynamic_Vector));
  new_v->arr = malloc(sizeof(void *) * dvector_size(V));
  new_v->arr_size = V->arr_size;
  new_v->size = dvector_size(V);
  int i = 0;
  for(;i < dvector_size(V); i++) { dvector_ref(new_v, i, f(dvector_deref(V, i))); } 
  return new_v;
}

Dynamic_Vector *dvector_insert_end(Dynamic_Vector *V, void *data) { 
  if(dvector_size(V) == V->arr_size) { V->arr = realloc(V->arr, (sizeof(void *) * (V->arr_size *= 2))); } //resize if necessary
  dvector_ref(V, V->size++, data);
  return V;
}

Dynamic_Vector *dvector_insert_nth(Dynamic_Vector *V, int i, void *data) { 
  if(dvector_size(V) == V->arr_size) { V->arr = realloc(V->arr, (sizeof(void *) * (V->arr_size *= 2))); } //resize if necessary
  int j = V->size++;
  for(; j > i; j--) { dvector_ref(V, j, dvector_deref(V, (j-1))); }
  dvector_ref(V, j, data); //should be same value as i
  return V;
}

Dynamic_Vector *dvector_remove_nth(Dynamic_Vector *V, int i, int free_data) { 
  if(free_data) { free(dvector_deref(V, i)); }
  int j = --V->size;
  for(; i < j; i++) { dvector_ref(V, i, dvector_deref(V, (i+1))); }
  return V;
}

int dvector_search(Dynamic_Vector *V, void *data, int (*comparator)(void *data1, void *data2)) {
  int i = 0;
  for(;i < dvector_size(V); i++) { if(comparator(data, dvector_deref(V, i)) == 0) { return i; } }
  return -1;
}
