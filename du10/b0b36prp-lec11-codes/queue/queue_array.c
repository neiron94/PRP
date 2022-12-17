/*
 * File name: queue_array.c
 * Date:      2016/07/26 20:37
 * Author:    Jan Faigl
 */

#include <stdlib.h>

#ifndef MAX_QUEUE_SIZE
#define MAX_QUEUE_SIZE 3
#endif

#include "my_malloc.h"
#include "queue_array.h"

// - function ----------------------------------------------------------------
void queue_init(queue_t **queue)
{ // here we assume stack is valid pointer to the pointer of stack and it does 
   // not point to some allocated memory that should be freed later
   *queue = myMalloc(sizeof(queue_t));
   (*queue)->queue = myMalloc(sizeof(void*)*MAX_QUEUE_SIZE);
   (*queue)->count = 0;
   (*queue)->start = 0;
   (*queue)->end = 0;
}

// - function ----------------------------------------------------------------
void queue_delete(queue_t **queue)
{
   queue_free(*queue);
   free((*queue)->queue);
   free(*queue);
   *queue = NULL;
}

// - function ----------------------------------------------------------------
void queue_free(queue_t *queue)
{
   while (!queue_is_empty(queue)) {
      void *value = queue_pop(queue);
      free(value);
   }
}

// - function ----------------------------------------------------------------
int queue_push(void *value, queue_t *queue)
{
   int ret = QUEUE_OK;
   if (queue->count < MAX_QUEUE_SIZE) {
      queue->queue[queue->end] = value;
      queue->end = (queue->end + 1) % MAX_QUEUE_SIZE; // circulate around
      queue->count += 1;
   } else {
      ret = QUEUE_MEMFAIL;
   }
   return ret;
}

// - function ----------------------------------------------------------------
void* queue_pop(queue_t *queue)
{
   void* ret = NULL;
   if (queue->count > 0) {
      ret = queue->queue[queue->start];
      queue->start = (queue->start + 1) % MAX_QUEUE_SIZE;
      queue->count -= 1;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool queue_is_empty(const queue_t *queue)
{
   return queue->count == 0;
}

// - function ----------------------------------------------------------------
void* queue_peek(const queue_t *queue)
{
   return queue_is_empty(queue) ? NULL : queue->queue[queue->start];
}

/* end of queue_array.c */
