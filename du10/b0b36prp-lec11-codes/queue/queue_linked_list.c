/*
 * File name: queue_linked_list.c
 * Date:      2016/07/26 21:28
 * Author:    Jan Faigl
 */

#include <stdlib.h>

#include "my_malloc.h"
#include "queue_linked_list.h"

// - function ----------------------------------------------------------------
void queue_init(queue_t **queue)
{ 
   *queue = myMalloc(sizeof(queue_t));
   (*queue)->head = NULL;
   (*queue)->end = NULL;
}

// - function ----------------------------------------------------------------
void queue_delete(queue_t **queue)
{
   queue_free(*queue);
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
   queue_entry_t *new_entry = malloc(sizeof(queue_entry_t));
   if (new_entry) { // fill the new_entry
      new_entry->value = value;
      new_entry->next = NULL;
      if (queue->end) { // if queue has end 
         queue->end->next = new_entry; // link new_entry 
      } else { // queue is empty
         queue->head = new_entry; // update head as well
      }
      queue->end = new_entry; // set new_entry as end
   } else {
      ret = QUEUE_MEMFAIL;
   }
   return ret;
}

// - function ----------------------------------------------------------------
void* queue_pop(queue_t *queue)
{
   void *ret = NULL;
   if (queue->head) { // having at least one entry
      ret = queue->head->value; //retrive the value
      queue_entry_t *tmp = queue->head;
      queue->head = queue->head->next;
      free(tmp);  // release queue_entry_t
      if (queue->head == NULL) { // update end if last
         queue->end = NULL;      // entry has been 
      }                          // popped
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool queue_is_empty(const queue_t *queue)
{
   return queue->head == 0;
}

// - function ----------------------------------------------------------------
void* queue_peek(const queue_t *queue)
{
   return queue_is_empty(queue) ? NULL : queue->head->value;
}

/* end of queue_linked_list.c */
