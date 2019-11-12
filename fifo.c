#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"

typedef struct Node {
    Item data;
    struct Node * next;
} * node;

struct Fifo {
    node head;
    node tail;    
};

node new_node(Item data) {
    
    node n = (node) malloc(sizeof(struct Node));

    if(!n) exit(0);
    
    n->data = data;
    n->next = NULL;

    return n;
}

fifo append(fifo list, Item data) {

    node n = new_node(data);

    if(list == NULL) {
        list = (fifo) malloc(sizeof(struct Fifo));
        if(!list) exit(0);
        list->head = n;
    } else
        list->tail->next = n;

    list->tail = n;

    return list;
}

Item pop(fifo list) {

    if(list == NULL || list->head == NULL)
        return NULL;

    node head = list->head;
    Item data = head->data;

    list->head = head->next;
    free(head);

    return data;
}

void delete(fifo list){
    for(Item aux = pop(list); aux != NULL; aux = pop(list))
        free(aux);

    free(list);
}

node get_head(fifo list) { return list? list->head : NULL; }

Item get_data(node node) { return node? node->data : NULL; }

node next(node node) { return node? node->next : NULL; }