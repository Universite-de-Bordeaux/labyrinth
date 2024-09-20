CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm

ALL : main

main : main.o case.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

main.o : main.c case.h
	$(CC) $(CFLAGS) -c $^

case.o : case.c case.h
	$(CC) $(CFLAGS) -c $^

.PHONY : clean
clean :
	@rm -f *.o main
	@echo "Clean done"
cleangch :
	@rm -f *.gch
	@echo "Clean gch done"
