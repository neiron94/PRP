/*
 * File name: stack_linked_list.c
 * Date:      2016/07/26 18:01
 * Author:    Jan Faigl
 */

#include <stdlib.h>

#include "my_malloc.h"
#include "stack_linked_list.h"

// - function ----------------------------------------------------------------
void stack_init(stack_t **stack)
{ // here we assume stack is valid pointer to the pointer of stack and it does 
  // not point to some allocated memory that should be freed later
  *stack = myMalloc(sizeof(stack_t));
  (*stack)->head = NULL;
}

// - function ----------------------------------------------------------------
void stack_delete(stack_t **stack)
{
   stack_free(*stack);
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
   stack_entry_t *new_entry = malloc(sizeof(stack_entry_t));
   if (new_entry) {
      new_entry->value = value;
      new_entry->next = stack->head;
      stack->head = new_entry;
   } else {
      ret = STACK_MEMFAIL;
   }
   return ret;
}

// - function ----------------------------------------------------------------
void* stack_pop(stack_t *stack)
{
   void *ret = NULL;
   if (stack->head) {
      ret = stack->head->value; //retrive the value
      stack_entry_t *tmp = stack->head;
      stack->head = stack->head->next;
      free(tmp);  // release stack_entry_t
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool stack_is_empty(const stack_t *stack)
{
   return stack->head == 0;
}

// - function ----------------------------------------------------------------
void* stack_peek(const stack_t *stack)
{
   return stack_is_empty(stack) ? NULL : stack->head->value;
}

/* end of stack_linked_list.c */
