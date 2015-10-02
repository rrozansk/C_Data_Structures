#include "Stack.c"
#include <stdio.h>

void print(void *data) {
  int *info = data;
  printf("%d\n", *info);
}

int main() {

  Stack *my_stack = make_stack(print);
  printf("stack size: %d\n", stack_size(my_stack));
  stack_print(my_stack);

  int foo = 5;
  push(my_stack, &foo);
  printf("empty stack? %d\n", stack_empty(my_stack));
  if(!stack_empty(my_stack)) {
    printf("peek: ");
    print(peek(my_stack));
  }
  stack_print(my_stack);
  printf("stack size: %d\n", stack_size(my_stack));
  printf("contains 5? %d\n", stack_contains(my_stack, &foo));
  void *data = pop(my_stack);
  stack_print(my_stack);
  printf("stack size: %d\n", stack_size(my_stack));
  if(!stack_empty(my_stack)) {
    printf("HERE\n");
    printf("peek: ");
    print(peek(my_stack));
  }
  printf("empty stack? %d\n", stack_empty(my_stack));
  printf("stack size: %d\n", stack_size(my_stack));
  stack_print(my_stack);

  return 0;
}
