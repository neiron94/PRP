/*
 * File name: queue_linked_list.h
 * Date:      2016/07/26 21:28
 * Author:    Jan Faigl
 */

#ifndef __QUEUE_LINKED_LIST_H__
#define __QUEUE_LINKED_LIST_H__

#define QUEUE_OK      0
#define QUEUE_MEMFAIL 1

typedef struct entry {
   void *value;
   struct entry *next;
} queue_entry_t;

typedef struct {
   queue_entry_t *head; 
   queue_entry_t *end; 
} queue_t;

void queue_init(queue_t **queue);
void queue_delete(queue_t **queue); 
void queue_free(queue_t *queue); 

int queue_push(void *value, queue_t *queue);
void* queue_pop(queue_t *queue);
_Bool queue_is_empty(const queue_t *queue);
void* queue_peek(const queue_t *queue);

#endif

/* end of queue_linked_list.h */
