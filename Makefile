CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

ALL : main

main : main.o case.o outside.o mazemaker.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

main.o : main.c case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

case.o : case.c case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

outside.o : outside.c outside.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

mazemaker.o : mazemaker.c mazemaker.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

.PHONY : clean
clean :
	@rm -f *.o main
	@echo "Clean done"
cleangch : #en cas de compilation de .h (erreur d'utilisateur)
	@rm -f *.gch
	@echo "Clean gch done"
deftest :
	@make
	@./main
	 @if [ $$? -eq 0 ]; then echo "Test ok"; else echo "UNEXPECTED ERROR"; fi
