#include "Dynamic_Vector.c"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

void print(void *data) { printf("%d\t", *(int *)data); }
void randomize(void *data) { *(int *)data = rand(); }
void *mapper(void *data) {
  void *new_d = malloc(sizeof(int));
  *(int *)new_d = 5;
  return new_d;
}
int comparator(void *data1, void *data2) {
  int d1 = *(int *)data1;
  int d2 = *(int *)data2;
  if (d1 == d2) return 0;
  if (d1 < d2) return -1;
  if (d1 > d2) return 1;
}

int main() {
  
  srand(time(NULL));

  int size = 15;
  int generations = 15;
  int print_gen = 1;
  
  Dynamic_Vector *V = dvector_make();
/*
  int *tmp;
  tmp = malloc(sizeof(int));
  *tmp = 5;
  V = dvector_insert_end(V, tmp);
  tmp = malloc(sizeof(int));
  *tmp = 4;
  V = dvector_insert_end(V, tmp);
  tmp = malloc(sizeof(int));
  *tmp = 3;
  V = dvector_insert_end(V, tmp);
  tmp = malloc(sizeof(int));
  *tmp = 2;
  V = dvector_insert_end(V, tmp);
  tmp = malloc(sizeof(int));
  *tmp = 1;
  V = dvector_insert_end(V, tmp);
  printf("vec:\t");
  dvector_walk(V, print);
  printf("\n");
  printf("size: %d\n", dvector_size(V));
  dvector_remove_nth(V, 1, 1);
  printf("vec:\t");
  dvector_walk(V, print);
  printf("\n");
  printf("size: %d\n", dvector_size(V));
*/

  int i;
  int j = 1;
  for(;j <= generations; j++) {
    V = dvector_make();
    if(j % print_gen == 0) { printf("on gen %d\n", j); }
    i = 0;
    for(;i<size;i++) {
      int *tmp = malloc(sizeof(int));
      *tmp = rand() % 2;
      V = dvector_insert_nth(V, i, tmp);
    }
    int *tmp = malloc(sizeof(int));
    *tmp = 5;
    if(dvector_search(V, tmp, comparator) != -1) { printf("found \n"); }
    free(tmp);
    dvector_walk(V, print);
    printf("\n");
    dvector_free(V, 1);
  }
  return 0;
}
