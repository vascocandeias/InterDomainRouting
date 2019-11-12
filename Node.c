#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fifo.h"

int clients;
int peers;
int providers;

typedef struct Node {
    int cur_node;
    fifo edges[3];
    int type;
    int hops;
    int id;
    int parent;
} * node;

int counter = 0;

node new() {

    node n = (node) malloc(sizeof(node));

    n->cur_node = -1;
    for(int i = 0; i < 3; i++) n->edges[i] = NULL;
    n->type = 0;
    n->hops = 0;
    n->id = 0;
    n->parent = -1;
    
    return n;
}

bool has_edges(node n){ return n? n->edges[0] != NULL && n->edges[1] != NULL && n->edges[2] != NULL : false; }

void process(node n, fifo ** lists, int cur_node){

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

    for edge in n->edges:
        node = edge.head
        if node.cur_node < cur_node:
            if list_num == 3:
                if (node.type is None or node.type < edge.relationship) and edge.relationship != 1:
                    node.parent = n->id
                    node.hops = n->hops + 1
                    node.type = edge.relationship
                    lists[edge.relationship - 1].append(node)
}

// def process(n-> lists, cur_node):
//     n->cur_node = not n->cur_node
//     n->type = None
//     for edge in n->edges:
//         node = edge.head
//         if node.cur_node < cur_node and node.type is None: 
//             node.hops = n->hops + 1
//             node.type = 0
//             lists[0].append(node)
    
//     n->hops = 0


// def commercialDijkstra(n-> nlists, cur_node):
//     lists = [deque()] * nlists
//     lists[0].append(n->
//     for i in range(nlists):
//         fifo = lists[i]
//         while fifo:
//             node = fifo.popleft()
//             // breakpoint()
//             if node.cur_node < cur_node:
//                 node.process(lists, cur_node)