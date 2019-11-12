/*****************************************************************************
 * File name: fifo.h
 *
 *  Author: Sofia Estrela nº 84186 | Vasco Candeias nº 84196
 *
 *  Release date: 2019/11/29
 *b
 *  Description: Headers file for the fifo.c functions
 *		
 ****************************************************************************/

#ifndef FIFO_H
#define FIFO_H

typedef void * Item;
typedef struct Fifo * fifo;
typedef struct Node * fifo_node;


fifo_node new_node(Item);
fifo append(fifo, Item);
Item pop(fifo);
void delete(fifo); 
fifo_node get_head(fifo list);
Item get_data(fifo_node);
fifo_node next(fifo_node);

#endif
