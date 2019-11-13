#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "fifo.h"
#include "graph.h"

float clients;
float peers;
float providers;
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
    // printf(n->id, r, n->parent)

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
                        lists[i - 1] = append(lists[i - 1], head);
                    }
                }
            }
        }
    }
    n->hops = 0;
}

void _dijkstra(node n, int cur_node) {
    Fifo lists[3], aux;

    for(int i = 0; i < 3; i++)
        lists[i] = NULL;

    lists[2] = append(lists[2], n);

    for(int i = 2; i >= 0; i--) {
        aux = lists[i];
        for(node auxnode = pop(aux); auxnode != NULL; auxnode = pop(aux)){
            if(auxnode->cur_node < cur_node)
                process(auxnode, lists, cur_node, i + 1);}
    }

    for(int i = 0; i < 3; i++)
        delete_fifo(lists[i]);
}

void dijkstra(Graph graph) {

        _dijkstra(graph->data[1], 1);

        // clock_t t0 = clock();
        // node n;

        // for(int i = 0; i < MAX_LENGTH; i++) {
        //     n = graph->data[i];
        //     if(n != NULL)
        //         _dijkstra(n, i);
        //     if(i % 500 == 0) {
        //         printf("%d %f\n", i, (float) (clock() - t0) / CLOCKS_PER_SEC);
        //         t0 = clock();
        //     }
        // }

        // printf("Elapsed time: %f\n", (float) (clock() - t0) / CLOCKS_PER_SEC);
        printf("\nStats:\n");
        float total_connections = graph->total_nodes*(graph->total_nodes - 1);
        providers = total_connections - peers - clients;
        printf("Providers: %f\n", providers/total_connections*100);
        printf("Peers: %f\n", peers/total_connections*100);
        printf("Clients: %f\n", clients/total_connections*100);
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
