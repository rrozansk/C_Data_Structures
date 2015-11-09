#include "Stack.c"
#include <stdio.h>

void print(void *data) {
  int *info = data;
  printf("%d\n", *info);
}

void *addr(void *data) {
  void *data2 = malloc(sizeof(int));
  *(int *)data2 = *(int *)data + 5;
  return data2;
}

int main() {

  Stack *my_stack = stack_make();
  printf("stack size: %d\n", stack_size(my_stack));
  stack_walk(my_stack, print);

  int foo = 5;
  stack_push(my_stack, &foo);
  printf("empty stack? %d\n", stack_empty(my_stack));
  if(!stack_empty(my_stack)) {
    printf("peek: ");
    print(stack_peek(my_stack));
  }
  stack_push(my_stack, &foo);
  stack_push(my_stack, &foo);
  stack_push(my_stack, &foo);
  stack_push(my_stack, &foo);
  stack_push(my_stack, &foo);
  stack_walk(my_stack, print);
  printf("stack size: %d\n", stack_size(my_stack));
//  printf("contains 5? %d\n", stack_contains(my_stack, &foo));
  void *data = stack_pop(my_stack);
  stack_walk(my_stack, print);
  printf("stack size: %d\n", stack_size(my_stack));
  if(!stack_empty(my_stack)) {
    printf("HERE\n");
    printf("peek: ");
    print(stack_peek(my_stack));
  }
  printf("empty stack? %d\n", stack_empty(my_stack));
  printf("stack size: %d\n", stack_size(my_stack));
  stack_walk(my_stack, print);
  Stack *new_stk = stack_map(my_stack, addr);
  stack_walk(new_stk, print);
  stack_free(my_stack, 0);

  return 0;
}
