#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

#define START_SIZE 1000
#define MAX_BUF_SIZE 4096
#define RESERVE_SPACE_TXT 33
#define RESERVE_SPACE_BIN 3


static void enlarge_edges(graph_t*);
static void write_number(char*, int*, int);


graph_t* allocate_graph() {
    graph_t *new_graph = (graph_t*)malloc(sizeof(graph_t));
    new_graph->size = START_SIZE;
    new_graph->edges = (edge_t*)malloc(sizeof(edge_t) * new_graph->size);
    new_graph->edge_count = 0;

    return new_graph;
}


void free_graph(graph_t **graph) {
    if (!graph || !(*graph))
        return;
    
    free((*graph)->edges);
    free(*graph);
    *graph = NULL;
}


void load_txt(const char *fname, graph_t *graph) {      // TODO - valgrind
    FILE *file = fopen(fname, "r");
    if (!file)
        fprintf(stderr, "Error: can't open file %s!", fname);

    size_t cur_c;
    int i = 0;  // 0 - from, 1 - to, 2 - cost
    int cur_from = 0;
    int cur_to = 0;
    int cur_cost = 0;

    while ((cur_c = getc_unlocked(file)) != EOF) {

        // if (graph->edge_count == 1317 && cur_from != 0)  // DEBUG
        //     printf("HERE");

        if (cur_c == ' ') { // read next number
            i++;
            continue;
        }
        else if (cur_c == '\n') {   // add the edge and go to next line
            if (graph->edge_count == graph->size)
                enlarge_edges(graph);

            graph->edges[graph->edge_count].from = cur_from;
            graph->edges[graph->edge_count].to = cur_to;
            graph->edges[graph->edge_count].cost = cur_cost;
            graph->edge_count++;

            i = cur_from = cur_to = cur_cost = 0;
            continue;
        }
        else {
            if (i == 0)
                cur_from = cur_from * 10 + (cur_c - '0');
            else if (i == 1)
                cur_to = cur_to * 10 + (cur_c - '0');
            else if (i == 2)
                cur_cost = cur_cost * 10 + (cur_c - '0');
        }
    }

    fclose(file);
}


void load_bin(const char *fname, graph_t *graph) {
    FILE *file = fopen(fname, "rb");
    if (!file)
        fprintf(stderr, "Error: can't open file %s!", fname);

    int *cur_line = (int*)malloc(sizeof(int) * 3);
    while (fread(cur_line, sizeof(int), 3, file) != 0 && cur_line) {

        if (graph->edge_count == graph->size)
            enlarge_edges(graph);

        graph->edges[graph->edge_count].from = cur_line[0];
        graph->edges[graph->edge_count].to = cur_line[1];
        graph->edges[graph->edge_count].cost = cur_line[2];
        graph->edge_count++;
    }
    free(cur_line);
    fclose(file);
}


static void enlarge_edges(graph_t *graph) {
    graph->size *= 2;
    edge_t *tmp = realloc(graph->edges, sizeof(edge_t) * graph->size);
    if (!tmp)
        fprintf(stderr, "Error: reallocation failed!");
    graph->edges = tmp;
}


void save_txt(const graph_t * const graph, const char *fname) {
    FILE *file = fopen(fname, "w");
    if (!file)
        fprintf(stderr, "Error: can't open file %s!", fname);

    char *buffer = (char*)malloc((MAX_BUF_SIZE + RESERVE_SPACE_TXT) * sizeof(char));
    int written_count = 0;

    for (size_t i = 0; i < graph->edge_count; i++) {

        // Write one line to buffer
        write_number(buffer, &written_count, graph->edges[i].from);
        buffer[written_count++] = ' ';
        write_number(buffer, &written_count, graph->edges[i].to);
        buffer[written_count++] = ' ';
        write_number(buffer, &written_count, graph->edges[i].cost);
        buffer[written_count++] = '\n';

        // Checking if buffer is full
        if (written_count >= MAX_BUF_SIZE) {
            fwrite(buffer, sizeof(char), written_count, file);
            written_count = 0;
        }
    }
    // Write the rest to the file
    fwrite(buffer, sizeof(char), written_count, file);

    free(buffer);
    fclose(file);
}


static void write_number(char *buffer, int *written_count, int number) {
    /* Writes one number to the buffer */

    int digit_count = 1;
    int mult = 1;
    int number_copy = number;

    while(number_copy > 9) {
        digit_count++;
        mult *= 10;
        number_copy /= 10;
    }

    for (int i = 0; i < digit_count; i++) {
        char cur_c = (number / mult) + '0';
        buffer[*written_count] = cur_c;
        (*written_count)++;

        number %= mult;
        mult /= 10;
    }
}


void save_bin(const graph_t * const graph, const char *fname) {
    FILE *file = fopen(fname, "wb");
    if (!file)
        fprintf(stderr, "Error: can't open file %s!", fname);

    int *buffer = (int*)malloc((MAX_BUF_SIZE + RESERVE_SPACE_BIN) * sizeof(int));
    int written_count = 0;

    for (size_t i = 0; i < graph->edge_count; i++) {

        // Write one line to buffer
        buffer[written_count++] = graph->edges[i].from;
        buffer[written_count++] = graph->edges[i].to;
        buffer[written_count++] = graph->edges[i].cost;

        // Checking if buffer is full
        if (written_count >= MAX_BUF_SIZE) {
            fwrite(buffer, sizeof(int), written_count, file);
            written_count = 0;
        }
    }
    // Write the rest to the file
    fwrite(buffer, sizeof(int), written_count, file);

    free(buffer);
    fclose(file);
}
