#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"
#include "graph.h"

extern int clients;
extern int peers;
extern int providers;

int main(int argc, char ** argv){

    if(argc < 2) {
        printf("run main input_file\n");
        exit(0);
    }

    Graph graph = new_graph(argv[1]); 
    delete_graph(graph);
    return 0;
}