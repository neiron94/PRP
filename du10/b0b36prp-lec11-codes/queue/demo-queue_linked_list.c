/*
 * File name: demo-queue_linked_list.c
 * Date:      2016/07/26 21:28
 * Author:    Jan Faigl
 */

#include <stdio.h>
#include <stdlib.h>

#include "my_malloc.h"
#include "queue_linked_list.h"

int* getRandomInt() 
{
   int *r = myMalloc(sizeof(int));
   *r = rand() % 256;
   return r;
}

int main(void)
{
   queue_t *queue;
   queue_init(&queue);

   for (int i = 0; i < 3; ++i) {
      int *pv = getRandomInt();
      int r = queue_push(pv, queue);
      printf("Add %2i entry '%3i' to the queue r = %i\n", i, *pv, r);
      if (r != QUEUE_OK) {
         fprintf(stderr, "Errro: Queue is full!\n");
         fprintf(stderr, "Info:  Release pv memory and quit pushing\n");
         free(pv);
         break;
      }
   }
   printf("\nPop the entries from the queue\n");
   while (!queue_is_empty(queue)) {
      int *pv = (int*)queue_pop(queue);
      printf("Popped value is %3i\n", *pv);
      free(pv);
   }
   queue_delete(&queue);
   return 0;
}

/* end of demo-queue_linked_list.c */
