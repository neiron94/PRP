/*
 * File name: stack_array-alloc.c
 * Date:      2016/07/26 17:04
 * Author:    Jan Faigl
 */

#include <stdlib.h>

#include "my_malloc.h"
#include "stack_array-alloc.h"

#ifndef INIT_STACK_SIZE
#define INIT_STACK_SIZE 3
#endif

#ifndef STACK_RESIZE
#define STACK_RESIZE 3
#endif

// - function ----------------------------------------------------------------
void stack_init(stack_t **stack)
{ // here we assume stack is valid pointer to the pointer of stack and it does 
   // not point to some allocated memory that should be freed later
   *stack = myMalloc(sizeof(stack_t));
   (*stack)->stack = myMalloc(sizeof(void*)*INIT_STACK_SIZE);
   (*stack)->count = 0;
   (*stack)->size = INIT_STACK_SIZE;
}

// - function ----------------------------------------------------------------
void stack_delete(stack_t **stack)
{
   stack_free(*stack);
   free((*stack)->stack);
   free(*stack);
   *stack = NULL;
}

// - function ----------------------------------------------------------------
void stack_free(stack_t *stack)
{
   while (!stack_is_empty(stack)) {
      void *value = stack_pop(stack);
      free(value);
   }
}

// - function ----------------------------------------------------------------
int stack_push(void *value, stack_t *stack)
{
   int ret = STACK_OK;
   if (stack->count == stack->size) { // try to realloc the stack size
      void **tmp = (void**)realloc(stack->stack, sizeof(void*) * (stack->size + STACK_RESIZE));
      if (tmp) { // realloc has been successful, stack->stack has been freed
         stack->stack = tmp;
         stack->size += STACK_RESIZE;
      }
   }
   if (stack->count < stack->size) {
      stack->stack[stack->count++] = value;
   } else {
      ret = STACK_MEMFAIL;
   }
   return ret;
}

// - function ----------------------------------------------------------------
void* stack_pop(stack_t *stack)
{
   return stack->count > 0 ? stack->stack[--(stack->count)]: NULL;
}

// - function ----------------------------------------------------------------
_Bool stack_is_empty(const stack_t *stack)
{
   return stack->count == 0;
}

// - function ----------------------------------------------------------------
void* stack_peek(const stack_t *stack)
{
   return stack_is_empty(stack) ? NULL : stack->stack[stack->count - 1];
}

/* end of stack_array-alloc.c */
