#include "queue.h"
#include <string.h>

#define EOL '\n'    // DEBUG


static bool enlarge_queue(queue_t*);
static bool reduce_queue(queue_t*);


queue_t* create_queue(int capacity) {
    queue_t *new_queue = (queue_t*)malloc(sizeof(queue_t));
    new_queue->values = (void**)malloc(sizeof(void*) * capacity);
    new_queue->head = -1;
    new_queue->tail = -1;
    new_queue->n = 0;
    new_queue->size = capacity;
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
    printf("CREATED QUEUE WITH CAPACITY %d\n", new_queue->size); // DEBUG
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
    return new_queue;
}


void delete_queue(queue_t *queue) {
    free(queue->values);
    free(queue);
    printf("QUEUE WAS DELETED\n"); // DEBUG
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
}


bool push_to_queue(queue_t *queue, void *data) {

    if (queue->size == 0)
        return false;

    if (queue->head == queue->tail) {
        // First item to insert
        if (queue->n == 0) {
            queue->values[0] = data;
            queue->head = 0;
            queue->tail = queue->size == 1 ? 0 : 1;
            (queue->n)++;
            return true;
        }
        // Not enough space, enlarge
        else if (!enlarge_queue(queue)) {
            printf("UNABLE TO PUSH %d\n", *((int *)data)); // DEBUG
                printf("------------------------------"); // DEBUG
                putchar(EOL); // DEBUG
            return false;
        }
    }

    queue->values[queue->tail] = data;
    queue->tail = (queue->tail + 1) % queue->size;
    (queue->n)++;
    
    printf("PUSHED %d\n", *((int *)data)); // DEBUG
        printf("------------------------------"); // DEBUG
        putchar(EOL); // DEBUG
    
    return true;
}


static bool enlarge_queue(queue_t *queue) {
    /* Enlarges array of values of queue */

    // Switch to parts of array
    if (queue->head != 0) {

        void **tmp_vals = (void**)malloc(sizeof(void*) * queue->head);
        if (!tmp_vals)
            return false;

        memcpy(tmp_vals, queue->values, sizeof(void*) * queue->head);
        memcpy(queue->values, queue->values + queue->head, sizeof(void*) * (queue->size - queue->head));
        memcpy(queue->values + (queue->size - queue->head), tmp_vals, sizeof(void*) * queue->head);

        free(tmp_vals);
    }
    
    // Enlarge array
    int prev_size = queue->size;
    queue->size *= 2;
    void **tmp = realloc(queue->values, sizeof(void*) * queue->size);
    if (!tmp)
        return false;
    queue->values = tmp;

    queue->head = 0;
    queue->tail = queue->n;

    // for (int i = queue->tail; i < queue->size; i++) {
    //     queue->values[i] = NULL;
    // }

    return true;
}


void* pop_from_queue(queue_t *queue) {
    if (queue->size == 0 || queue->n == 0)
        return NULL;

    void *ret = queue->values[queue->head];
    printf("POPPED %d\n", *((int *)queue->values[queue->head])); // DEBUG
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
    queue->values[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->size;
    (queue->n)--;
    if (queue->n == 0)
        return ret;

    if (queue->n < (queue->size / 3))
        if (!reduce_queue(queue))
            return NULL;

    

    return ret;
}


static bool reduce_queue(queue_t *queue) {
    
    void **tmp = (void**)malloc(sizeof(void*) * queue->n);
    if (!tmp)
        return false;
    
    if (queue->tail > queue->head)
        memcpy(tmp, queue->values[queue->head], sizeof(void*) * (queue->tail - queue->head));
    else {
        memcpy(tmp, queue->values[queue->head], sizeof(void*) * (queue->size - queue->head));
        if (queue->tail != 0)
            memcpy(tmp + (queue->size - queue->head), queue->values[0], sizeof(void*) * queue->tail);
    }

    free(queue->values);
    queue->size /= 3;
    queue->values = (void**)malloc(sizeof(void*) * queue->size);
    if (!queue->values)
        return false;
    memcpy(queue->values, tmp, sizeof(void*) * queue->n);
    queue->head = 0;
    queue->tail = queue->n;

    free(tmp);
    return true;
}


void* get_from_queue(queue_t *queue, int idx) {
    if (idx >= queue->n || queue->n <= 0) {
        printf("POSITION %d DOESN'T EXIST\n", idx); // DEBUG
        printf("------------------------------"); // DEBUG
        putchar(EOL); // DEBUG
        return NULL;
    }

    printf("POSITION %d IS %d\n", idx, *((int *)queue->values[(queue->head + idx) % queue->size])); // DEBUG
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
    return queue->values[(queue->head + idx) % queue->size];
}


int get_queue_size(queue_t *queue) {
    printf("QUEUE LENGTH IS %d\n", queue->n); // DEBUG
    printf("------------------------------"); // DEBUG
    putchar(EOL); // DEBUG
    return queue->n;
}


// Function to print queue // DEBUG
void print_queue(queue_t *queue) {
    for (int i = 0; i < queue->size; ++i) {
        if (queue->values[i]) {
            // printf("%3d ", *((int *)queue->values[i]));
            printf("  X ");
        } else {
            printf("  . ");
        }
    }
    putchar(EOL);
    printf("------------------------------");
    putchar(EOL);
}



// Main program function // DEBUG
int main(int argc, char *argv[]) {
    int a = 1;
    void *p_a = &a;
    int b = 2;
    void *p_b = &b;
    int c = 3;
    void *p_c = &c;
    int d = 4;
    void *p_d = &d;
    int e = 5;
    void *p_e = &e;
    //int f = 6;
    //void *p_f = &f;

    queue_t *queue = create_queue(6);
    get_queue_size(queue);
    push_to_queue(queue, p_a);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_b);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_c);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_d);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_e);
    print_queue(queue);
    get_queue_size(queue);
    pop_from_queue(queue);
    print_queue(queue);
    get_queue_size(queue);
    pop_from_queue(queue);
    print_queue(queue);
    get_queue_size(queue);
    pop_from_queue(queue);
    print_queue(queue);
    get_queue_size(queue);
    pop_from_queue(queue);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_a);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_b);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_c);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_d);
    print_queue(queue);
    get_queue_size(queue);
    push_to_queue(queue, p_e);
    print_queue(queue);
    
    delete_queue(queue);
    return EXIT_SUCCESS;
}
