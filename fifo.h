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
typedef struct Fifo * Fifo;
typedef struct Node * FifoNode;

Fifo append(Fifo, Item);
Item pop(Fifo);
void delete_fifo(Fifo); 
FifoNode get_head(Fifo);
Item get_data(FifoNode);
FifoNode next(FifoNode);
void printlist(Fifo);

#endif