#include "Queue.c"
#include <stdio.h>

void printer(void *data) {
  int *info = data;
  printf("%d\n", *info);
}

void *addr(void *data) {
  int *num = malloc(sizeof(int));
  *num = *(int *)data + 5;
  return num;
}

int main() {

  Queue *my_queue = queue_make();
  printf("queue size: %d\n", queue_size(my_queue));
  queue_walk(my_queue, printer);

  int foo = 5;
  queue_enqueue(my_queue, &foo);
  queue_enqueue(my_queue, &foo);
  queue_enqueue(my_queue, &foo);
  printf("empty queue? %d\n", queue_empty(my_queue));
  if(!queue_empty(my_queue)) {
    printf("peek: ");
    printer(queue_peek(my_queue));
  }
  queue_walk(my_queue, printer);
  printf("queue size: %d\n", queue_size(my_queue));
  Queue *Q = queue_map(my_queue, addr);
  printf("empty queue( Q )? %d\n", queue_empty(Q));
  printf("queue size( Q ): %d\n", queue_size(Q));
  printf("Q walk\n");
  queue_walk(Q, printer);
  //throwing away all the return values
  queue_dequeue(my_queue);
  queue_dequeue(my_queue);
  queue_dequeue(my_queue);
  printf("my_queue walk\n");
  queue_walk(my_queue, printer);
  printf("queue size: %d\n", queue_size(my_queue));
  if(!queue_empty(my_queue)) {
    printf("peek: ");
    printer(queue_peek(my_queue));
  }
  printf("empty queue? %d\n", queue_empty(my_queue));
  printf("queue size: %d\n", queue_size(my_queue));
  queue_free(Q, 1);
  queue_free(my_queue, 0);

  return 0;
}
