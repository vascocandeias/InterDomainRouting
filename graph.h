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

typedef struct Node * GraphNode;
typedef struct Graph * Graph;

Graph new_graph(char *);
void insert_edge(GraphNode, GraphNode, int); 
void delete_node(GraphNode);
bool has_edges(GraphNode);
void process(GraphNode, Fifo *, int, int);
void delete_graph(Graph);

#endif