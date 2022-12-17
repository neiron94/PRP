/*
 * File name: stack_array.h
 * Date:      2016/07/26 13:54
 * Author:    Jan Faigl
 */

#ifndef __STACK_ARRAY_H__
#define __STACK_ARRAY_H__

#define STACK_OK      0
#define STACK_MEMFAIL 1

typedef struct {
   void **stack; // array of void pointers
   int count;
} stack_t;

void stack_init(stack_t **stack);
void stack_delete(stack_t **stack); 
void stack_free(stack_t *stack); 

int stack_push(void *value, stack_t *stack);
void* stack_pop(stack_t *stack);
_Bool stack_is_empty(const stack_t *stack);
void* stack_peek(const stack_t *stack);

#endif

/* end of stack_array.h */
