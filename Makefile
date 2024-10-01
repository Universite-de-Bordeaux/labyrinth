CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

ALL : main

main : src/main.o src/case.o src/outside.o src/mazemaker.o src/solveur.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)

main.o : src/main.c src/case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

case.o : src/case.c src/case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

outside.o : src/outside.c src/outside.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

mazemaker.o : src/mazemaker.c src/mazemaker.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)

solveur.o : src/solveur.c src/solveur.h
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
