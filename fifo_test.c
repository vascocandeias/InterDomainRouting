
#include <stdio.h>
#include <stdlib.h>

#include "fifo.h"

int main(int argc, char* argv[]) {

    Fifo list = NULL;

    for(int i = 0; i < 3; i++) {
        int * num = (int *) malloc(sizeof(int));
        *num = i;
        list = append(list, num);
        printlist(list); 
    }

    int * num;

    printf("\npop:\n");
    pop(list);
    printlist(list);
    pop(list);
    printlist(list);
    printf("\nappend:\n");
    num = (int *) malloc(sizeof(int));
    *num = 7;
    list = append(list, num);
    printlist(list);

    num = NULL;

    printf("\n");

    for(FifoNode node = get_head(list); get_data(node) != NULL; node = next(node)){
        num = get_data(node);
        printf("%d\n", *num);
    }
    
    printf("\n");

    for(int * num = pop(list); num != NULL; num = pop(list)){
        printf("%d\n", *num);
        printlist(list);
        free(num);
    }

    delete_fifo(list);
}