/*
 * File name: priority_queue.c
 * Date:      2016/07/31 17:08
 * Author:    Jan Faigl
 */

#include <stdlib.h>

#include "my_malloc.h"
#include "priority_queue.h"

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
int queue_push(void *value, int priority, queue_t *queue)
{
   int ret = QUEUE_OK;
   queue_entry_t *new_entry = malloc(sizeof(queue_entry_t));
   if (new_entry) { // fill the new_entry
      new_entry->value = value;
      new_entry->priority = priority;
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
      queue_entry_t* cur = queue->head->next;
      queue_entry_t* prev = queue->head;
      queue_entry_t* best = queue->head;
      queue_entry_t* bestPrev = NULL;

      while (cur) {
         if (cur->priority < best->priority) { // update the entry with the lowest priority 
            best = cur;
            bestPrev = prev;
         }
         prev = cur;
         cur = cur->next;
      }
      if (bestPrev) { // linked the list after best removal
         bestPrev->next = best->next;
      } else { // selected is the head
         queue->head = queue->head->next;
      }
      ret = best->value; //retrive the value
      if (queue->end == best) { //update the list end
         queue->end = bestPrev;
      }
      free(best);  // release queue_entry_t
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
   void *ret = NULL;
   if (queue && queue->head) {
      ret = queue->head->value;
      int lowestPriority = queue->head->priority;
      queue_entry_t *cur = queue->head->next;
      while (cur != NULL) {
         if (lowestPriority > cur->priority) {
            lowestPriority = cur->priority;
            ret = cur->value;
         }
         cur = cur->next;
      }
   }
   return ret;
}

/* end of priority_queue.c */
