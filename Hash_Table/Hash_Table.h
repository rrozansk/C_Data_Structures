/**********************************************************************

                      F I L E   I N F O R M A T I O N

***********************************************************************/
/*
 Author: Ryan Rozanski
 Created: 3/23/16
 Last Edited: 3/23/16
*/

#ifndef HASH_DEFS
#define HASH_DEFS

#include "../Dynamic_Vector/Dynamic_Vector.h"

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
  void **tbl;            // array of buckets
  int tbl_size;
  int load_size;
} Hash;

typedef struct Bucket {
  void **entries;       // array of entries
  int entries_size;
} Bucket;

typedef struct Entry {
  void *key;
  int k_len;
  void *val;
} Entry;

/**********************************************************************

             		F U N C T I O N   P R O T O T Y P E S

***********************************************************************/
Hash *hash_make();                                                              // make a new Hash
void hash_free(Hash *H, int free_keys, int free_values);                        // free the Hash and optionally its keys/values
void hash_walk(Hash *H, void (*f)(void *value));                                // walk over the Hash and apply side-effect f to each elem
Hash *hash_map(Hash *H, void *(*f)(void *value));                               // make a copy of the hash where each elem is replaced by f(old_elem)
void *hash_search(Hash *H, void *key, int len);                                 // search returns value if key is valid, else NULL
void hash_insert(Hash *H, void *key, int key_size, void *value);                // add an key, value assoc to the Hash
void hash_remove(Hash *H, void *key, int key_size, int free_key, int free_val); // remove the key, value assoc from the Hash and optionally free its key/val
void hash_keys_values(Hash *H, void *keys[], void *values[]);

#endif
