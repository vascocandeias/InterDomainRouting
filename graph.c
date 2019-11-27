#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "fifo.h"
#include "graph.h"

double clients;
double peers;
double providers;
double connections;
int sum = 0;
int flag = 0;

typedef struct Node {
    int cur_node;
    Fifo edges[4];
    route type;
    int hops;
    int id;
    int parent;
    bool verified;
    bool tier1;
} * Node;

typedef struct Graph {
    Node * data;
    int total_nodes;
    int tier1;
} * Graph;

int counter = 0;

static Node new_node() {

    Node n;

    if(!(n = (Node) malloc(sizeof(struct Node))))
        return NULL;

    n->cur_node = -1;
    for(int i = 0; i < 4; i++) n->edges[i] = NULL;
    n->type = NONE;
    n->hops = 0;
    n->id = 0;
    n->parent = -1;
    n->tier1 = true;
    n->verified = false;

    return n;
}

void insert_edge(Node head, Node tail, int relationship) {
    head->edges[relationship] = append(head->edges[relationship], tail);
    sum++;
}

void delete_node(Node n) {

    if(n == NULL) return;

    for(int i = 0; i < 4; i++) delete_fifo(n->edges[i]);

    free(n);
}

void process(Node n, Fifo * lists, int cur_node) {

    Fifo edge = NULL;
    FifoNode head = NULL;
    route type = n->type;

    n->cur_node = cur_node;

    switch(type) {
        case CLIENT:
            clients++;
            break;
        case PEER:
            peers++;
            break;
        case PROVIDER:
            providers++;
            break;
        default:
            break;
    }

    n->type = NONE;

    if(type != CLIENT && type != NONE) return;

    for(int i = CLIENT; i >= PEER; --i) {

        edge = n->edges[i];

        for(FifoNode aux = get_head(edge); aux != NULL; aux = next(aux)) {
            head = get_data(aux);
            if(head->cur_node < cur_node && head->type < i) {
                head->parent = n->id;
                head->type = i;
                lists[i] = append(lists[i], head);
            }
        }
    }
}

void dijkstra(Graph graph, route up_bound, route low_bound, void (*process_node)(Node, Fifo *, int, int*), void (*print)(Graph, int *)) {

    clock_t t0 = clock(), begin = t0;
    Node n;
    Fifo lists[4], aux;
    int hops[MAX_LENGTH] = {0};
    int percentage = graph->total_nodes / 10, i = 0;

    reset_cur_nodes(graph);

    printf("\rLoading:  0%%");
    fflush(stdout);

    for(int i = 0; i < 4; ++i)
        lists[i] = NULL;

    providers = clients = peers = connections = 0;

    for(int cur_node = 0; cur_node < MAX_LENGTH; cur_node++) {
        if(!(n = graph->data[cur_node]))
            continue;
        
                // printf("\nNode %d\n", i);
        i++;

        if(cur_node >= percentage) {
            printf("\rLoading: %2d%%", i * 100 /graph->total_nodes);
            fflush(stdout);
            percentage += graph->total_nodes / 10;
        }

        lists[CLIENT] = append(lists[CLIENT], n);

        for(int i = up_bound; i >= low_bound; --i) {
            aux = lists[i];
            for(Node auxnode = pop(aux); auxnode != NULL; auxnode = pop(aux)) {
                if(auxnode->cur_node < cur_node)
                    process_node(auxnode, lists, cur_node, hops);
            }
       }
    }

    printf("\rLoading: 100%%\n");

    print(graph, hops);

    printf("\nElapsed time: %f\n", (float) (clock() - begin) / CLOCKS_PER_SEC);

    for(int i = 0; i < 4; i++)
        delete_fifo(lists[i]);
}

void print_percentages(Graph graph) {
    printf("\nStats:\n");
    double total_connections = graph->total_nodes*(graph->total_nodes - 1);
    if(!providers)
        providers = total_connections - peers - clients;
    else {
        printf("There are %.0f routes missing\n", total_connections - peers - clients - providers);
        total_connections = peers + clients + providers;
    }
    printf("Providers: %f\n", providers/total_connections*100);
    printf("Peers: %f\n", peers/total_connections*100);
    printf("Clients: %f\n", clients/total_connections*100);
}

void reset_cur_nodes(Graph graph) {
	Node n = NULL;
	for(int cur_node = 0; cur_node < MAX_LENGTH; cur_node++) {
		if(!(n = graph->data[cur_node]))
			continue;
		n->cur_node = -1;
	}
}

void print_hops(Graph graph, int * hops) {
    unsigned long int accum = 0;
    // unsigned long int total_connections = graph->total_nodes*(graph->total_nodes - 1);
    unsigned long int total_connections = peers + clients + providers;
    for(int i = 1; i < graph->total_nodes && accum < total_connections; ++i) {
        printf("P(#hops) >= %d: %f%%\n", i, (float) (total_connections - accum)/total_connections * 100);
        accum += hops[i];
    }
}


void dijkstra_2(Graph graph, route up_bound, route low_bound, void (*process_node)(Node, Fifo *, int, int*), void (*print)(Graph, int *)) {

    clock_t t0 = clock(), begin = t0;
    Node n;
    Fifo lists[6], aux;
    int hops[MAX_LENGTH] = {0};
    int percentage = graph->total_nodes / 10, i = 0;

    reset_cur_nodes(graph);

    printf("\rLoading:  0%%");
    fflush(stdout);

    for(int i = 0; i < 6; ++i)
        lists[i] = NULL;

    providers = clients = peers = connections = 0;

    for(int cur_node = 0; cur_node < MAX_LENGTH; cur_node++) {
        if(!(n = graph->data[cur_node]))
            continue;
        
        i++;
                // printf("\nNode %d\n", i);
        if(cur_node >= percentage) {
            printf("\rLoading: %2d%%", i * 100 /graph->total_nodes);
            fflush(stdout);
            percentage += graph->total_nodes / 10;
        }

        lists[CLIENT] = append(lists[CLIENT], n);

        for(int i = up_bound; i >= low_bound + 1; --i) {
            aux = lists[i];
            for(Node auxnode = pop(aux); auxnode != NULL; auxnode = pop(aux)) {
                if(auxnode->cur_node < cur_node)
                    process_node(auxnode, lists, cur_node, hops);
            }
        }

        for(Node auxnode = get_min(lists[PROVIDER], lists[4], lists[5]); auxnode != NULL; auxnode = get_min(lists[PROVIDER], lists[4], lists[5])) {
            // printf("Provider: %d\n", auxnode->id);
            if(auxnode->cur_node < cur_node)
                process_node(auxnode, lists, cur_node, hops);
        }
    }

    printf("\rLoading: 100%%\n");

    print(graph, hops);

    printf("\nElapsed time: %f\n", (float) (clock() - begin) / CLOCKS_PER_SEC);

    for(int i = 0; i < 4; i++)
        delete_fifo(lists[i]);
}

Node get_min(Fifo fifo1, Fifo fifo2, Fifo fifo3) {
    Node node1 = get_data(get_head(fifo1));
    Node node2 = get_data(get_head(fifo2));
    Node node3 = get_data(get_head(fifo3));

    if(!node1) {
        if(node2) {
            if(node3)
                return node2->hops < node3->hops ? pop(fifo2) : pop(fifo3);
            return pop(fifo2);
        }
        return pop(fifo3);
    }

    if(node2) {
        if(node3) { // no fifo is empty
            if(node1->hops < node2->hops && node1->hops < node3->hops)
                return pop(fifo1);
            return node2->hops < node3->hops ? pop(fifo2) : pop(fifo3);
        }
        return node2->hops < node1->hops ? pop(fifo2) : pop(fifo1);
    } else {
        if(node3)
            return node1->hops < node3->hops ? pop(fifo1) : pop(fifo3); 
        return pop(fifo1);
    }
}

void hops_process(Node n, Fifo * lists, int cur_node, int * hops) {

    Fifo edge = NULL;
    FifoNode head = NULL;
    route type = n->type, up_bound, low_bound = PROVIDER;
    Fifo aux_lists[4] = {lists[NONE], lists[PROVIDER], lists[PEER], lists[CLIENT]};

    n->cur_node = cur_node;
    hops[n->hops]++;
    n->type = NONE;

    switch(type) {
        case CLIENT:
            clients++;
            break;
        case PEER:
            peers++;
            break;
        case PROVIDER:
            providers++;
            break;
        default:
            break;
    }

    // printf("\n%d: route: %d hops: %d\n", n->id, type, n->hops);

	if(type == CLIENT || type == NONE) {
        up_bound = CLIENT;
        if(type == CLIENT) aux_lists[PROVIDER] = lists[4];
    } else {
        up_bound = PROVIDER;
        if(type == PEER)
            aux_lists[PROVIDER] = lists[5];
    }
    
    for(int i = up_bound; i >= low_bound; --i){
        edge = n->edges[i];
        for(FifoNode aux = get_head(edge); aux != NULL; aux = next(aux)) {
            head = get_data(aux);
            // printf("Inserting: %d - %d\n", head->id, i);
            if(head->cur_node < cur_node) {
                if(head->type < i || (head->type == PROVIDER && head->hops > n->hops + 1)) {
                    // printf("Inserted\n");
                    head->parent = n->id;
                    head->hops = n->hops + 1;
                    head->type = i;
                    aux_lists[i] = append(aux_lists[i], head);
                    // if(get_data(get_head(aux_lists[PROVIDER])))
                    //     printf("List Head: %d\n", ((FifoNode) get_data(get_head(aux_lists[PROVIDER])))->id);
                    // else
                    // {
                    //     printf("List Head: NULL\n");
                    // }
                    
                }
                // } else if(up_bound == PROVIDER && head->hops > n->hops + 1) {
                //     head->parent = n->id;
                //     head->hops = n->hops + 1;
                //     head->type = i;               
                // }
            }
        }
    }
    lists[CLIENT] = aux_lists[CLIENT];
    lists[PEER] = aux_lists[PEER];
    if(type == CLIENT) lists[4] = aux_lists[PROVIDER];
    else if(type == PEER) lists[5] = aux_lists[PROVIDER];
    else lists[PROVIDER] = aux_lists[PROVIDER];

    // printf("Lists:\n");
    // printlist(lists[PROVIDER]);
    // printlist(lists[4]);
    // printlist(lists[5]);
	n->hops = 0;
}


void n_bfs(Graph graph) {

    clock_t t0 = clock(), begin = t0;
    Node n;
    Fifo list = NULL;
    unsigned long int accum = 0;
    unsigned long int total_connections = graph->total_nodes*(graph->total_nodes - 1);
    int hops[MAX_LENGTH] = {0};
    int percentage = graph->total_nodes / 10, i = 0;

    reset_cur_nodes(graph);

    providers = clients = peers = connections = 0;

    printf("\rLoading:  0%%");
    fflush(stdout);

    for(int cur_node = 0; cur_node < MAX_LENGTH; cur_node++) {
        if(!(n = graph->data[cur_node]))
            continue;
        i++;
        if(cur_node >= percentage) {
            printf("\rLoading: %2d%%", i * 100 /graph->total_nodes);
            fflush(stdout);
            percentage += graph->total_nodes / 10;
        }

        list = append(list, n);

        for(Node auxnode = pop(list); auxnode != NULL; auxnode = pop(list)) {
            bfs(auxnode, list, cur_node, hops);
        }
    }

    clients -= graph->total_nodes;

    printf("\rLoading: 100%%\n");

    printf("Elapsed time: %f\n", (float) (clock() - begin) / CLOCKS_PER_SEC);
    print_hops(graph, hops);
    delete_fifo(list);
}



void bfs(Node n, Fifo list, int cur_node, int * hops) {

    Fifo edges = NULL;
    FifoNode head = NULL;

    n->cur_node = cur_node;
    hops[n->hops]++;
    clients++;
    for(route i = CLIENT; i >= PROVIDER; --i){

        edges = n->edges[i];

        for(FifoNode edge = get_head(edges); edge != NULL; edge = next(edge)) {
            head = get_data(edge);
            if(head->cur_node < cur_node) {
				head->cur_node = cur_node;
                head->parent = n->id;
                head->hops = n->hops + 1;
                list = append(list, head);
            }
        }
    }
    n->hops = 0;
}


Graph new_graph(char * filename) {

    FILE * file = NULL;
    int tail, head, type;
    Graph graph = NULL;

    if(!(graph = (Graph) malloc(sizeof(struct Graph)))) return NULL;
    if(!(graph->data = (Node *) malloc(MAX_LENGTH * sizeof(Node)))) return NULL;

    for(int i = 0; i < MAX_LENGTH; i++)
        graph->data[i] = NULL;

    graph->total_nodes = 0;
    graph->tier1 = 0;

    if(!(file = fopen (filename, "r"))) return NULL;

	while(fscanf(file, "%d %d %d", &tail, &head, &type) == 3) {

        if(!graph->data[tail]) {
            if(!(graph->data[tail] = new_node())) return NULL;
            graph->total_nodes++;
            graph->data[tail]->id = tail;
        }

        if(!graph->data[head]) {
            if(!(graph->data[head] = new_node())) return NULL;
            graph->total_nodes++;
            graph->data[head]->id = head;
        }

		if(type == 1 && graph->data[head]->tier1) {
			graph->data[head]->tier1 = false;
			graph->tier1++;
		}

        insert_edge(graph->data[tail], graph->data[head], type);
    }

    printf("\nNumber of nodes: %d\n", graph->total_nodes);
	graph->tier1 = graph->total_nodes - graph->tier1;
    fclose(file);

    return graph;
}

bool check_connectivity(Graph graph) {
	Node n, head;
	int counter = 0;

	for(int i = 0; i < MAX_LENGTH; ++i) {
		if(!(n = graph->data[i]) || !n->tier1)
			continue;

		counter = 1;

        for(FifoNode edge = get_head(n->edges[2]); edge != NULL; edge = next(edge)) {
            head = get_data(edge);
            if(head->tier1 && head->cur_node < i) {
				counter++;
				head->cur_node = i;
            }
        }
		if(counter != graph->tier1) return false;
		counter = 1;
	}

	return true;
}

bool check_cycles(Graph graph) {
	Node n, head;

	reset_cur_nodes(graph);

	for(int i = 0; i < MAX_LENGTH; ++i) {
		if(!(n = graph->data[i]) || !n->tier1)
			continue;
      flag = i;
      for(FifoNode edge = get_head(n->edges[1]); edge != NULL; edge = next(edge)) {
          head = get_data(edge);
          if(!head->verified && head->cur_node >= i) continue;
		      if(!dfs(graph, head, i, 0)) return false;
      }
	}

	return true;
}

bool dfs(Graph graph, Node n, int cur_node, int level){
    Node head = NULL;
    if(n->verified) return true;
	n->cur_node = cur_node;
    for(FifoNode edge = get_head(n->edges[1]); edge != NULL; edge = next(edge)) {
        head = get_data(edge);
        // printf("%d - %d - %d - %d - %d\n", n->id, head->id, level, head->cur_node, cur_node);
        if(!head->verified && head->cur_node >= cur_node) return false;
        if(!dfs(graph, head, cur_node, level+1)) return false;
        // printf("%p\n", next(edge));
    }
    n->verified = true;
    return true;
}

void delete_graph(Graph graph) {
    if(!graph) return;
    for(int i = 0; i < MAX_LENGTH; i++)
        delete_node(graph->data[i]);
    free(graph->data);
    free(graph);
}
