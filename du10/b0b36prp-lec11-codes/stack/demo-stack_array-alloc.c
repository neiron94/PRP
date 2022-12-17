/*
 * File name: demo-stack_array-alloc.c
 * Date:      2016/07/26 17:04
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"
#include "stack_array-alloc.h"

int* getRandomInt() 
{
   int *r = myMalloc(sizeof(int));
   *r = rand() % 256;
   return r;
}

int main(void)
{
   stack_t *stack;
   stack_init(&stack);

   for (int i = 0; i < 5; ++i) {
      int *pv = getRandomInt();
      int r = stack_push(pv, stack);
      printf("Add %2i entry '%3i' to the stack r = %i\n", i, *pv, r);
      if (r != STACK_OK) {
         fprintf(stderr, "Errro: Stack is full!\n");
         fprintf(stderr, "Info:  Release pv memory and quit pushing\n");
         free(pv);
         break;
      }
   }
   printf("\nPop the entries from the stack\n");
   while (!stack_is_empty(stack)) {
      int *pv = (int*)stack_pop(stack);
      printf("Popped value is %3i\n", *pv);
      free(pv);
   }
   stack_delete(&stack);
   return 0;
}

/* end of demo-stack_array-alloc.c */
