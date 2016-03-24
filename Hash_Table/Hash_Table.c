/**********************************************************************

         	    		F I L E   I N F O R M A T I O N

***********************************************************************/
/* 
 Author: Ryan Rozanski
 Created: 2/7/16
 Last Edited: 3/4/16
 
 A general purpose iterative Hash Table library for arbitrary payloads
*/

/**********************************************************************

                 	    		I N C L U D E S

***********************************************************************/
#include <stdlib.h>
#include <string.h>
#include "Hash_Table.h"
#include "../Dynamic_Vector/Dynamic_Vector.h"

/**********************************************************************

       		F U N C T I O N   I M P L E M E N T A T I O N S 

***********************************************************************/
unsigned int fHash(char *str, unsigned int len) {                // internal hash function
   unsigned int seed = 1313;
   unsigned int hash = 0;
   unsigned int i = 0;
   for(i = 0; i < len; str++, i++) { hash = (hash * seed) + (*str); }
   return hash;
}

Hash *hash_make() {
  Hash *hash = malloc(sizeof(Hash));
  hash->tbl = calloc(HASH_INIT_SIZE, sizeof(Bucket *));
  hash->tbl_size = HASH_INIT_SIZE;
  hash->load_size = 0;
  return hash;
}

void hash_free(Hash *H, int free_keys, int free_values) { 
  int i,j;
  Bucket *curr;
  for(i = 0; i < H->tbl_size; i++) { 
    if((curr = H->tbl[i])) {
      for(j = 0; j < curr->entries_size; j++) {
        if(free_keys) { free(((Entry *)curr->entries[j])->key); }
        if(free_values) { free(((Entry *)curr->entries[j])->val); }
        free((Entry *)curr->entries[j]);
      }
      free(curr->entries);
      free(curr);
    }
  }
  free(H->tbl);
  free(H);
}

// really f should take key and values
void hash_walk(Hash *H, void (*f)(void *value)) {
  int i,j;
  Bucket *curr;
  for(i = 0; i < H->tbl_size; i++) { 
    if((curr = H->tbl[i])) {
      for(j = 0; j < curr->entries_size; j++) { f(((Entry *)curr->entries[j])->val); }
    }
  }
}

Hash *hash_map(Hash *H, void *(*f)(void *key)) { 
  Hash *COPY = malloc(sizeof(Hash));
  COPY->tbl_size = H->tbl_size;
  COPY->load_size = H->load_size;
  COPY->tbl = calloc(H->tbl_size, sizeof(Bucket *));
  int i,j;
  Bucket *bucket;
  Entry *entry;
  for(i = 0; i < H->tbl_size; i++) { 
    if((bucket = H->tbl[i])) {
      Bucket *BUCKET_COPY = malloc(sizeof(Bucket));
      BUCKET_COPY->entries = malloc(sizeof(Entry *) * bucket->entries_size);
      BUCKET_COPY->entries_size = bucket->entries_size;
      COPY->tbl[i] = BUCKET_COPY;
      for(j = 0; j < bucket->entries_size; j++) {
        entry = bucket->entries[j];
        Entry *ENTRY_COPY = malloc(sizeof(Entry));
        ENTRY_COPY->key = entry->key;
        ENTRY_COPY->k_len = entry->k_len;
        ENTRY_COPY->val = f(entry->val);
        BUCKET_COPY->entries[j] = ENTRY_COPY;
      }
    }
  }
  return COPY;
}

void *hash_search(Hash *H, void *key, int len) {
  Bucket *bucket = HASH_BUCKET(H, key, len);
  if(bucket) {
    int i = 0;
    for(; i < bucket->entries_size; i++) {
      Entry *entry = bucket->entries[i];
      if((entry->k_len == len) && !memcmp(entry->key, key, len)) { return entry->val; }
    }
    return NULL;
  }
  else { return NULL; }
}

void hash_insert(Hash *H, void *key, int key_size, void *value) {
  Entry *entry;
  Bucket *bucket = HASH_BUCKET(H, key, key_size);
  if(bucket) {
    int i = 0;
    for(; i < bucket->entries_size; i++) {
      entry = bucket->entries[i];
      if((entry->k_len == key_size) && !memcmp(entry->key, key, key_size)) { 
        entry->val = value; 
        return;
      }
    }
  }
  entry = malloc(sizeof(Entry));
  entry->key = key;
  entry->k_len = key_size;
  entry->val = value;
  if(bucket) {
    bucket->entries = realloc(bucket->entries, (sizeof(Entry *) * ++bucket->entries_size));
    bucket->entries[bucket->entries_size-1] = entry;
  }
  else {
    bucket = malloc(sizeof(Bucket));
    bucket->entries_size = 1;
    bucket->entries = malloc(sizeof(Entry *) * 1);
    bucket->entries[0] = entry;
    HASH_BUCKET(H, key, key_size) = bucket;
  }
  H->load_size++;
  if(hash_load_factor(H) > .67) {             // EXPAND HASH TABLE SIZE
    void **old_tbl = H->tbl;
    int old_size = H->tbl_size;
    H->tbl = calloc(H->tbl_size *= 2, sizeof(Bucket *));
    int i, j;
    for(i = 0; i < old_size; i++) {       // RE-HASH -- need to rehash everything
      if((bucket = old_tbl[i])) {
        for(j = 0; j < bucket->entries_size; j++) {
          entry = bucket->entries[j];
          Bucket *bucket_p = HASH_BUCKET(H, entry->key, entry->k_len);
          if(bucket_p) { 
            bucket_p->entries = realloc(bucket_p->entries, (sizeof(Entry *) * ++bucket_p->entries_size));
            bucket_p->entries[bucket_p->entries_size-1] = entry;
          } 
          else {
            bucket_p = malloc(sizeof(Bucket));
            bucket_p->entries = malloc(sizeof(Entry *) * 1);
            bucket_p->entries_size = 1;
            bucket_p->entries[0] = entry;
            HASH_BUCKET(H, entry->key, entry->k_len) = bucket_p;
          }
        }
        free(bucket->entries);
        free(bucket);
      }
    }
    free(old_tbl); 
  }
}

// maybe rewrite this to just put NULL in buck place and insert searches for a
// NULL bf it extends
void hash_remove(Hash *H, void *key, int key_size, int free_key, int free_val) {
  H->load_size--;
  Bucket *bucket = HASH_BUCKET(H, key, key_size);
  int i = 0;
  if(bucket) {
    for(; i < bucket->entries_size; i++) {       // SEG FAULTS HERE
      Entry *entry = bucket->entries[i];
      if((entry->k_len == key_size) && !memcmp(entry->key, key, key_size)) {
        if(bucket->entries_size == 1) {
          HASH_BUCKET(H, key, key_size) = '\0';
          free(bucket->entries);
          free(bucket);
          bucket = '\0';
        }
        if(free_key) { free(entry->key); }
        if(free_val) { free(entry->val); }
        free(entry);
        break;
      }
    }
    if(bucket) {
      for(; i < bucket->entries_size-1; i++) { bucket->entries[i] = bucket->entries[i+1]; }
      bucket->entries_size--;
    }
  }
}

Dynamic_Vector *hash_keys(Hash *H) {
  Dynamic_Vector *keys = dvector_make();
  int i,j;
  Bucket *curr;
  for(i = 0; i < H->tbl_size; i++) { 
    if((curr = H->tbl[i])) {
      for(j = 0; j < curr->entries_size; j++) { dvector_insert_end(keys, ((Entry *)curr->entries[j])->key); }
    }
  }
  return keys;
}

Dynamic_Vector *hash_values(Hash *H) {
  Dynamic_Vector *values = dvector_make();
  int i,j;
  Bucket *curr;
  for(i = 0; i < H->tbl_size; i++) { 
    if((curr = H->tbl[i])) {
      for(j = 0; j < curr->entries_size; j++) { dvector_insert_end(values, ((Entry *)curr->entries[j])->val); }
    }
  }
  return values;
}
