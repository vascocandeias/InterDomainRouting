CC = gcc								# compilador a usar
# CFLAGS = -g -Wall -O3 -pedantic			# opções de compilação
CFLAGS = -g 			# opções de compilação
LFLAGS = -lprofiler							# opções de linkagem
OBJS = main.c fifo.o graph.o 			# ficheiros objecto
EXEC = main 							# nome do executável

$(EXEC): $(OBJS)
	@$(CC) $(LFLAGS) -o $(EXEC) $(OBJS) 

fifo.o: fifo.c fifo.h
	@$(CC) $(CFLAGS) -c fifo.c

graph.o: graph.c graph.h
	@$(CC) $(CFLAGS) -c graph.c

valgrind:
	@valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main $(file)

clean:
	@rm -f *.o $(EXEC)
