#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "fifo.h"
#include "graph.h"

double clients;
double peers;
double providers;
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

    Fifo edge = NULL;
    FifoNode head = NULL;

    n->cur_node = cur_node;
    char r = 'a';
    if(n->type == 3) {
        clients += 1;
        r = 'c';
    }
    else if(n->type == 2) {
        peers += 1;
        r = 'p';
    }
    else if(n->type == 1) {
        providers += 1;
        r = 'v';
    }

    // printf("%d %c %d\n", n->id, r, n->parent);

    n->type = 0;

    if(list_num != 3) return;

    for(int i = 2; i < 4; i++){
        edge = n->edges[i];

        for(FifoNode aux = get_head(edge); aux != NULL; aux = next(aux)) {
            head = get_data(aux);
            if(head->cur_node < cur_node) {
                // if((head->type == 0 || head->type < i) && i != 1) {
                if(head->type < i) {
                    head->parent = n->id;
                    // head->hops = n->hops + 1;
                    head->type = i;
                    lists[i - 1] = append(lists[i - 1], head);
                    // printf("List %d: ", i-1);
                    // printlist(lists[i-1]);
                }
            }
        }
    }
    n->hops = 0;
}

void _dijkstra(node n, int cur_node) {
    Fifo lists[3], aux;

    if(n == NULL)
        return;

    for(int i = 0; i < 3; i++)
        lists[i] = NULL;

    lists[2] = append(lists[2], n);

    for(int i = 2; i >= 1; i--) {
        aux = lists[i];
        for(node auxnode = pop(aux); auxnode != NULL; auxnode = pop(aux)) {
            printf("Pop %d: ", i);
            printlist(aux);
            if(auxnode->cur_node < cur_node)
                process(auxnode, lists, cur_node, i + 1);
        }
    }

    for(int i = 0; i < 3; i++)
        delete_fifo(lists[i]);
}

void dijkstra(Graph graph) {

        // int n = MAX_LENGTH-1;

        // _dijkstra(graph->data[n-1], n-1);
        // _dijkstra(graph->data[n], n);

        clock_t t0 = clock(), begin = t0;
        node n;
        Fifo lists[3], aux;

        for(int i = 0; i < 3; i++)
            lists[i] = NULL;

        for(int cur_node = 0; cur_node < MAX_LENGTH; cur_node++) {
            n = graph->data[cur_node];
            if(!n)
                continue;

            lists[2] = append(lists[2], n);
            // printlist(lists[2]);

            for(int i = 2; i >= 1; i--) {
                aux = lists[i];
                for(node auxnode = pop(aux); auxnode != NULL; auxnode = pop(aux)) {
                    // printf("Pop %d: ", i);
                    // printlist(aux);
                    if(auxnode->cur_node < cur_node)
                        process(auxnode, lists, cur_node, i + 1);}
            }

            // if(cur_node % 500 == 0) {
            //     printf("%d %f\n", cur_node, (float) (clock() - t0) / CLOCKS_PER_SEC);
            //     t0 = clock();        // printf("Elapsed time: %f\n", (float) (clock() - t0) / CLOCKS_PER_SEC);
            //     printf("%d\n", cur_node);
            //     float total_connections = graph->total_nodes*(graph->total_nodes - 1);
            //     providers = total_connections - peers - clients;
            //     printf("Peers: %f\n", peers/total_connections*100);
            //     printf("Clients: %f\n", clients/total_connections*100);
            //     printf("Peers: %f\n", peers);
            //     printf("Clients: %f\n", clients);
            // }
        }

        // printf("%d %f\n", MAX_LENGTH, (float) (clock() - t0) / CLOCKS_PER_SEC);
        printf("Elapsed time: %f\n", (float) (clock() - begin) / CLOCKS_PER_SEC);
        printf("\nStats:\n");
        double total_connections = graph->total_nodes*(graph->total_nodes - 1);
        providers = total_connections - peers - clients;
        printf("Providers: %f\n", providers/total_connections*100);
        printf("Peers: %f\n", peers/total_connections*100);
        printf("Clients: %f\n", clients/total_connections*100);
        for(int i = 0; i < 3; i++)
            delete_fifo(lists[i]);
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
            graph->data[tail]->id = tail;
        }
        if(!graph->data[head]) {
            graph->data[head] = new_node();
            graph->total_nodes++;
            graph->data[head]->id = head;
        }

        insert_edge(graph->data[tail], graph->data[head], type);
    }
    printf("Nodes: %d\n", graph->total_nodes);
    fclose(file);

    return graph;
}

void delete_graph(Graph graph) {
    for(int i = 0; i < MAX_LENGTH; i++)
        delete_node(graph->data[i]);
    free(graph->data);
    free(graph);
}