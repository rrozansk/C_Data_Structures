/**********************************************************************

                      F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 3/23/16
 Last Edited: 3/23/16
*/

#ifndef DYNAMIC_VECTOR_DEFS
#define DYNAMIC_VECTOR_DEFS

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

#endif
