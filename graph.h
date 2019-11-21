/*****************************************************************************
 * File name: graph.h
 *
 *  Author: Sofia Estrela nº 84186 | Vasco Candeias nº 84196
 *
 *  Release date: 2019/11/29
 *b
 *  Description: Headers file for the graph.c functions
 *		
 ****************************************************************************/

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#include "fifo.h"

#define MAX_LENGTH 65536

typedef enum {NONE, PROVIDER, PEER, CLIENT} route;
typedef struct Node * GraphNode;
typedef struct Graph * Graph;

Graph new_graph(char *);
void insert_edge(GraphNode, GraphNode, int); 
void delete_node(GraphNode);
void process(GraphNode, Fifo *, int);
void delete_graph(Graph);
void dijkstra(Graph, route, route, void(*)(), void(*)());
void dijkstra_2(Graph, route, route, void(*)(), void(*)());
GraphNode get_min(Fifo fifo1, Fifo fifo2, Fifo fifo3);
void print_percentages(Graph);
void print_hops(Graph, int *);
void n_bfs(Graph graph);
void bfs(GraphNode, Fifo, int, int *);
void hops_process(GraphNode, Fifo *, int, int *);
bool check_connectivity(Graph);
void reset_cur_nodes(Graph);
bool check_cycles(Graph);
bool dfs(Graph, GraphNode, int, int);

#endif
