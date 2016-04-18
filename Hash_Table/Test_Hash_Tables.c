#include <stdio.h>
#include <time.h>
#include "Hash_Table.c"
#include "../Queue/Queue.c"

typedef struct junk {
  int foo;
  char baz;
  short i;
} junk;

junk *make_junk() {
  junk *new = malloc(sizeof(junk));
  new->foo = rand() % 32000;
  new->baz = rand() % 255;
  new->i = rand() % 9999;
  return new;
}

void *copier(void *data) {
  junk *j = (junk *)data;
  junk *cp = malloc(sizeof(junk));
  cp->foo = j->foo;
  cp->baz = j->baz;
  cp->i = j->i;
  return cp;
}

int jcomp(void *d1, void *d2) {
  junk *j1 = (junk *)d1;
  junk *j2 = (junk *)d2;
  if(j1->foo != j2->foo) { return 1; }
  if(j1->baz != j2->baz) { return 1; }
  if(j1->i != j2->i) { return 1; }
  return 0;
}

int main() {

  srand(time(NULL));

  Hash *H;
  Queue *K, *Kp;

  int HSIZE = 200000;
  int GENERATIONS = 20;
  int i, j, *v;
  junk *k;

  for(j = 0; j < GENERATIONS; j++) {
    H = hash_make();
    K = queue_make();
    for(i = 0; i < HSIZE; i++) {
      k = make_junk();
      while(hash_search(H, k, sizeof(junk))) {
        free(k);
        k = make_junk();
      }
      queue_enqueue(K, k);
      *(v = malloc(sizeof(int))) = i;
      hash_insert(H, k, sizeof(junk), v);
    }
    int bkts = 0;
    for(i = 0; i < HSIZE; i++) { if(H->tbl[i]) { bkts++; } } // maybe also find the max and min of bkt size
    printf("Hash %d\titems: %d\tsize: %d\tload factor: %f\tbuckets: %d\tavg bucket len: %f\n", \
        j, HSIZE, H->tbl_size, hash_load_factor(H), bkts, (float)HSIZE/(float)bkts);
    Kp = queue_map(K, copier);
    while(queue_size(K)) { 
      k = queue_dequeue(K);
      hash_remove(H, k, sizeof(junk), 0, 0);
    }
    while(queue_size(Kp)) { 
      k = queue_dequeue(Kp);
      if(hash_search(H, k, sizeof(junk)) != NULL) { printf("improper remove!!\n"); }
    }
    queue_free(K, 0);
    hash_free(H, 1, 1);
  }
  
  return 0;
}
