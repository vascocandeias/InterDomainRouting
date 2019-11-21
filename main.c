#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"
#include "graph.h"

extern double clients;
extern double peers;
extern double providers;

int main(int argc, char ** argv){

    int choice = 0, error = 0;

    if(argc < 2) {
        printf("run main input_file\n");
        exit(0);
    }

    Graph graph = new_graph(argv[1]); 
    if(!graph) {
        printf("There was an error reading the input file. Try again\n");
        exit(0);
    }

    bool connected = check_connectivity(graph);
    if(!connected)
        printf("The graph is not comercially connected. Option 1 might be slower\n");

    if(!check_cycles(graph))
        printf("The graph has client cycles\n");
    do
    {
        printf("\nChoose and option\n\n");
        printf("1. Get types of routes\n");
        printf("2. Get number of hops in commercial network without finding routes first\n");
        printf("3. Get number of hops in commercial network by finding routes first\n");
        printf("4. Get number of hops in non commercial network\n");
        printf("5. Exit\n");
        error = scanf("%d",&choice);

        if(error != 1) choice = 0;
        
        switch (choice) {
            case 1:
                if(connected) dijkstra(graph, CLIENT, PEER, process, print_percentages);
                else dijkstra(graph, CLIENT, PROVIDER, hops_process, print_percentages);
                break;
            case 2:
                dijkstra(graph, CLIENT, PROVIDER, hops_process, print_hops);
                break;
            case 3:
                break;
            case 4:
                n_bfs(graph);
                break;
            case 5: break;
            default:
                printf("Invalid option\n");
                break;
        } 
    } while (choice != 5);


    delete_graph(graph);
    return 0;
}
