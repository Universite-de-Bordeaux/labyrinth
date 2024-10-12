CC = @gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

all : src/main

src/main : src/main.o src/case.o src/outside.o src/mazemaker.o src/solveur.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)
	@echo "Compilation done"

main.o : src/main.c
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "main.o done"

case.o : src/case.c src/case.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "case.o done"

outside.o : src/outside.c src/outside.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "outside.o done"

mazemaker.o : src/mazemaker.c src/mazemaker.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "mazemaker.o done"

solveur.o : src/solveur.c src/solveur.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "solveur.o done"

.PHONY : clean
clean :
	@rm -f src/*.o src/main
	@echo "Clean done in src"
	@rm -f *.o main
	@echo "Clean done in root (security)"

Cmain :
	@make
	@echo "Lancement du main\n"
	@./src/main
	@echo "\nFin du main"
	 @if [ $$? -eq 0 ]; then echo "Test ok"; else echo "UNEXPECTED ERROR"; fi

main : src/main
	@echo "Lancement du main\n"
	@./src/main
	@echo "\nFin du main"
	 @if [ $$? -eq 0 ]; then echo "Test ok"; else echo "UNEXPECTED ERROR"; fi
