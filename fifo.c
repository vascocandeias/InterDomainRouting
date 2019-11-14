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

static node new_node(Item data) {
    
    node n = (node) malloc(sizeof(struct Node));

    if(!n) exit(0);
    
    n->data = data;
    n->next = NULL;

    return n;
}

Fifo append(Fifo list, Item data) {

    if(!list) {
        list = (Fifo) malloc(sizeof(struct Fifo));
        if(!list) exit(0);
        list->head = NULL;
        list->tail = NULL;
    }

    // if(list->tail) {
    //     printf("tail: %p\n", list->tail);
    //     printf("tail->next: %p\n", list->tail->next);
    // }

    if(list->tail) {
        if(!list->tail->data) {
            list->tail->data = data;
            return list;
        }
        if(list->tail->next) {
            list->tail->next->data = data;
            list->tail = list->tail->next;
            return list;
        }
    }

    node n = new_node(data);

    if(!list->head) 
        list->head = n;
    else 
        list->tail->next = n;
 
    list->tail = n;

    // printf("%p\n", n);

    return list;
}

Item pop(Fifo list) {
    // printf("Before\n");
    // printf("tail: %p\n", list->tail);
    // printf("head: %p\n", list->head);

    if(list == NULL || list->head == NULL)
        return NULL;

    node head = list->head;
    Item data = head->data;

    head->data = NULL;

    if(!list->head->next || !list->head->next->data)
        return data;

    // printf("DEBUG\n");
    // printf("list->head: %p\n", list->head);
    // printf("head->next: %p\n", head->next);
    // printf("list->tail->next: %p\n", list->tail->next);
    // printf("head: %p\n", head);

    list->head = head->next;
    head->next = list->tail->next;
    list->tail->next = head;
    
    // if(list->tail == head) 
    // list->tail = head;
    // free(head);
    // printf("After\n");
    // printf("tail: %p\n", list->tail);
    // printf("head: %p\n", list->head);
    return data;
}

void delete_fifo(Fifo list){
    node next_node;
    // for(Item aux = pop(list); aux != NULL; aux = pop(list));
    if(!list)
        return;
        
    for(node n = list->head; n != NULL; n = next_node){
        next_node = n->next;
        free(n);
    }

    if(list)
        free(list);
}

void printlist(Fifo list) {
    node next_node;
    printf("\n");
    for(node n = list->head; n != NULL; n = next_node) {
        printf("%p: %d, ", n, n->data? *(int*)(n->data) : -1);
        next_node = n->next;
    }
    printf("\n");
    printf("\n");
}

node get_head(Fifo list) { return list? list->head : NULL; }

Item get_data(node node) { return node? node->data : NULL; }

node next(node node) { return node? node->next : NULL; }