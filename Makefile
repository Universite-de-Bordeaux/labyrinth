CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

ALL : main

main : main.o case.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

main.o : main.c case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

case.o : case.c case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

.PHONY : clean
clean :
	@rm -f *.o main
	@echo "Clean done"
cleangch :
	@rm -f *.gch
	@echo "Clean gch done"
deftest :
	@make
	@./main