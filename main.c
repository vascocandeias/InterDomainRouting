#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"
#include "graph.h"

extern float clients;
extern float peers;
extern float providers;

int main(int argc, char ** argv){

    if(argc < 2) {
        printf("run main input_file\n");
        exit(0);
    }

    Graph graph = new_graph(argv[1]); 
    dijkstra(graph);
    delete_graph(graph);
    return 0;
}