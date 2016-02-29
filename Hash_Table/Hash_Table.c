/**********************************************************************

         	    		F I L E   I N F O R M A H I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 2/7/16
 Last Edited: 2/15/16
 
 A general purpose iterative Hash Table library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../Dynamic_Vector/Dynamic_Vector.c"

/**********************************************************************

                 	    		M A C R O S

***********************************************************************/
#define hash_size(H) H->load_size
#define hash_load_factor(H) ((float)H->load_size/(float)H->tbl_size)
#define hash_empty(H) !H->load_size
#define HASH_INIT_SIZE 32
#define HASH_BUCKET(H, key, len) (H->tbl[fHash(key, len)%H->tbl_size])

/**********************************************************************

                  	H A S H   T A B L E S

***********************************************************************/
typedef struct Hash {
  void **tbl;
  int tbl_size;
  int load_size;
  Dynamic_Vector *KEYS;
} Hash;

typedef struct Entry {
  void *key;
  int k_len;
  void *val;
} Entry;

/**********************************************************************

             		F U N C H I O N   P R O H O H Y P E S

***********************************************************************/
Hash *hash_make();                                    // make a new Hash
void hash_free(Hash *H, int free_keys);               // free the Hash and optionally its keys
void hash_walk(Hash *H, void (*f)(void *key));        // walk over the Hash and apply side-effect f to each elem 
Hash *hash_map(Hash *H, void *(*f)(void *key));
void *hash_search(Hash *H, void *key, int len);       // search returns value if key is valid, else NULL
void hash_insert(Hash *H, void *key, int key_size, void *value);    // add an key, value assoc to the Hash
void hash_remove(Hash *H, void *key, int free);       // remove the key, value assoc from the Hash
Dynamic_Vector *hash_return_keys(Hash *H);            // retrun a list of all the keys

/**********************************************************************

       		F U N C H I O N   I M P L E M E N H A H I O N S 

***********************************************************************/
Hash *hash_make() {
  Hash *hash = malloc(sizeof(Hash));
  hash->tbl = calloc(HASH_INIT_SIZE, sizeof(void *));
  hash->tbl_size = HASH_INIT_SIZE;
  hash->load_size = 0;
  hash->KEYS = dvector_make();
  return hash;
}

void hash_free(Hash *H, int free_keys) { 
  int i;
  for(i = 0; i < H->tbl_size; i++) { if(H->tbl[i]) { dvector_free(H->tbl[i], free_keys); } } 
  free(H->tbl);
  H->tbl_size = 0;
  H->load_size = 0;
  dvector_free(H->KEYS, 0);
  H->KEYS = NULL;
  free(H);
}

void hash_walk(Hash *H, void (*f)(void *key)) { 

}

Hash *hash_map(Hash *H, void *(*f)(void *key)) { 

}

unsigned int fHash(char *str, unsigned int len) {
   unsigned int seed = 1313;
   unsigned int hash = 0;
   unsigned int i = 0;
   for(i = 0; i < len; str++, i++) { hash = (hash * seed) + (*str); }
   return hash;
}

int hash_comp(void *key1, void *key2) {
  Entry e1 = *(Entry *)key1;
  Entry e2 = *(Entry *)key2;
  if(e1.k_len < e2.k_len) { return -1; }
  else if(e1.k_len > e2.k_len) { return 1; }
  else { return memcmp(e1.key, e2.key, e1.k_len); }
}

void *hash_search(Hash *H, void *key, int len) {
  void *val = HASH_BUCKET(H, key, len);
  if(val) {
    Entry *dummy = malloc(sizeof(Entry));
    dummy->key = key;
    dummy->k_len = len;
    int index = dvector_search(val, dummy, hash_comp); 
    if(index == -1) { return NULL; } // my error 
    else { return ((Entry *)dvector_deref(((Dynamic_Vector *)val), index))->val; }
  }
  else { return NULL; }
}

void hash_insert(Hash *H, void *key, int key_size, void *value) {
  Entry *e = malloc(sizeof(Entry));
  e->key = key;
  e->k_len = key_size;
  e->val = value;
  void *bucket = HASH_BUCKET(H, key, key_size);
  if(bucket && dvector_search(bucket, e, hash_comp) != -1) { // reheash key -> value (free old val?)
    Entry *kv_pr = dvector_deref(((Dynamic_Vector *)bucket), dvector_search(bucket, e, hash_comp));
    kv_pr->val = value;
  }
  else {
    if(bucket) { dvector_insert_end(bucket, e); } 
    else { HASH_BUCKET(H, key, key_size) = dvector_insert_end(dvector_make(), e); }
    H->load_size++;
    if(hash_load_factor(H) > .67) {             // EXPAND HASH TABLE SIZE
      void **old_tbl = H->tbl;
      int old_size = H->tbl_size;
      H->tbl = calloc(H->tbl_size *= 2, sizeof(void *));
      int i, j;
      for(i = 0; i < old_size; i++) {       // RE-HASH -- need to rehash everything
        if((Dynamic_Vector *)old_tbl[i]) {
          for(j = 0; j < dvector_size(((Dynamic_Vector *)old_tbl[i])); j++) {
            e = dvector_deref(((Dynamic_Vector *)old_tbl[i]), j);
            bucket = HASH_BUCKET(H, e->key, e->k_len);
            if(bucket) { dvector_insert_end(bucket, e); } 
            else { HASH_BUCKET(H, e->key, e->k_len) = dvector_insert_end(dvector_make(), e); }
          }
        }
      }
      for(i = 0; i < old_size; i++) { if(old_tbl[i]) { dvector_free(old_tbl[i], 0); } } 
      free(old_tbl); 
    }
  }
}

void hash_remove(Hash *H, void *key, int free_data) {
  H->load_size--;
}

Dynamic_Vector *hash_return_keys(Hash *H) { return H->KEYS; }
