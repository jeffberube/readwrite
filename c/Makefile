CC = gcc
OBJ = readwrite.o 
FLAGS = -Wall -std=c99 -g -o
LIB = -lpthread

readwrite: $(OBJ)
	$(CC) $(OBJ) $(LIB) $(FLAGS) $@

.c.o:
	$(CC) -g -c $<

clean:
	rm *.o readwrite 
