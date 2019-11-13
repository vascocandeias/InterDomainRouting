#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fifo.h"
#include "graph.h"

int clients;
int peers;
int providers;
int sum = 0;

typedef struct Node {
    int cur_node;
    Fifo edges[4];
    int type;
    int hops;
    int id;
    int parent;
} * node;

typedef struct Graph {
    node * data;
    int total_nodes;
} * Graph;

int counter = 0;

static node new_node() {

    node n = (node) malloc(sizeof(struct Node));

    n->cur_node = -1;
    for(int i = 0; i < 4; i++) n->edges[i] = NULL;
    n->type = 0;
    n->hops = 0;
    n->id = 0;
    n->parent = -1;
    
    return n;
}

void insert_edge(node head, node tail, int relationship) {
    head->edges[relationship] = append(head->edges[relationship], tail);
    sum++;
}

void delete_node(node n) {
    
    if(n == NULL) return;

    for(int i = 0; i < 4; i++) delete_fifo(n->edges[i]);

    free(n);
}

bool has_edges(node n){ return n? n->edges[3] != NULL && n->edges[1] != NULL && n->edges[2] != NULL : false; }

void process(node n, Fifo * lists, int cur_node, int list_num) {

    Fifo list = NULL;
    FifoNode head = NULL;

    n->cur_node = cur_node;
    // r = 0
    if(n->type == 3)
        clients += 1;
        // r = "c"
    else if(n->type == 2)
        peers += 1;
        // r = "p"
    else if(n->type == 1)
        providers += 1;
        // r = "pv"
    // print(n->id, r, n->parent)

    n->type = 0;
    for(int i = 1; i < 4; i++){
        list = n->edges[i];

        for(FifoNode aux = get_head(list); aux != NULL; aux = next(aux)) {
            head = get_data(aux);
            if(head->cur_node < cur_node) {
                if(list_num == 3) {
                    if((head->type == 0 || head->type < i) && i != 1) {
                        head->parent = n->id;
                        // head->hops = n->hops + 1;
                        head->type = i;
                        append(lists[i - 1], head);
                    }
                }
            }
        }
    }
}

Graph new_graph(char * filename) {

    FILE * file = NULL;
    int tail, head, type;
    Graph graph;
    
    graph = (Graph) malloc(sizeof(struct Graph));
    
    graph->data = (node *) malloc(MAX_LENGTH * sizeof(node));
    for(int i = 0; i < MAX_LENGTH; i++)
        graph->data[i] = NULL;

    graph->total_nodes = 0;

    if(!(file = fopen (filename, "r"))) {
        delete_graph(graph);
        exit(0);
    }
        
	while(fscanf(file, "%d %d %d", &tail, &head, &type) == 3) { 
        
        if(!graph->data[tail]) {
            graph->data[tail] = new_node();
            graph->total_nodes++;
        }
        if(!graph->data[head]) {
            graph->data[head] = new_node();
            graph->total_nodes++;
        }

        insert_edge(graph->data[tail], graph->data[head], type);
    }

    fclose(file);

    return graph;
}

void delete_graph(Graph graph) {
    for(int i = 0; i < MAX_LENGTH; i++)
        delete_node(graph->data[i]);
    free(graph->data);
    free(graph);
}
