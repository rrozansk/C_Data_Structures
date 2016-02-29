#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Hash_Table.c"
#include "../Queue/Queue.c"

typedef struct junk {
  int foo;
  char baz;
  short i;
} junk;

junk make_junk() {
  junk *new = malloc(sizeof(junk));
  new->foo = rand() % 32000;
  new->baz = rand() % 255;
  new->i = rand() % 9999;
  return *new;
}

int main() {

  srand(time(NULL));

  Hash *H;
//  Queue *K;

  int HSIZE = 200000;
  int GENERATIONS = 20;
  int i, j, *v;
  junk *k;

  for(j = 0; j < GENERATIONS; j++) {
    H = hash_make();
//    K = queue_make();
    for(i = 0; i < HSIZE; i++) {
      *(k = malloc(sizeof(int))) = make_junk();
//      queue_enqueue(K, k);
      *(v = malloc(sizeof(int))) = i;
      hash_insert(H, k, sizeof(junk), v);
    }
    int bkts = 0;
    for(i = 0; i < HSIZE; i++) { if(H->tbl[i]) { bkts++; } } // maybe also find the max and min of bkt size
    printf("Hash %d\titems: %d\tsize: %d\tload factor: %f\tbuckets: %d\tavg bucket len: %f\n", \
        j, HSIZE, H->tbl_size, hash_load_factor(H), bkts, (float)HSIZE/(float)bkts);
//    while(queue_size(K)) { if(hash_search(H, queue_dequeue(K), sizeof(junk)) == NULL) { printf("HASH TABLE REBUILD ERROR\n"); } }
//    queue_free(K, 0);
    hash_free(H, 1);
  }
  return 0;
}
