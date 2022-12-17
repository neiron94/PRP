/*
 * File name: demo-stack.c
 * Date:      2016/07/26 18:49
 * Author:    Jan Faigl
 */

#if STACK_ARRAY
# include "stack_array.h"
#elif STACK_ARRAY_ALLOC
# include "stack_array-alloc.h"
#elif STACK_LINKED_LIST
#include "stack_linked_list.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"

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

   for (int i = 0; i < 15; ++i) {
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

/* end of demo-stack.c */
