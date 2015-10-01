#include "Queue.c"
#include <stdio.h>

void printer(void *data) {
  int *info = data;
  printf("%d\n", *info);
}

int main() {

  Queue *my_queue = make_queue(printer);
  printf("queue size: %d\n", queue_size(my_queue));
  queue_print(my_queue);

  int foo = 5;
  enqueue(my_queue, &foo);
  enqueue(my_queue, &foo);
  enqueue(my_queue, &foo);
  printf("empty queue? %d\n", queue_empty(my_queue));
  if(queue_peek(my_queue) != NULL) {
    printf("peek: ");
    printer(queue_peek(my_queue));
  }
  queue_print(my_queue);
  printf("queue size: %d\n", queue_size(my_queue));
  //throwing away all the return values
  dequeue(my_queue);
  dequeue(my_queue);
  dequeue(my_queue);
  queue_print(my_queue);
  printf("queue size: %d\n", queue_size(my_queue));
  if(!queue_empty(my_queue)) {
    printf("peek: ");
    printer(queue_peek(my_queue));
  }
  printf("empty queue? %d\n", queue_empty(my_queue));
  printf("queue size: %d\n", queue_size(my_queue));
  queue_print(my_queue);

  return 0;
}
