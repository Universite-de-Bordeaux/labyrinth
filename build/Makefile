CC = @gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

all : ../main
	@echo "compilation done"

../main : main.o struct.o outside.o mazemaker.o solveur.o test.o cmd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)
	@echo "main done"

main.o : ../src/main.c
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "main.o done"

struct.o : ../src/struct.c ../src/struct.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "struct.o done"

outside.o : ../src/outside.c ../src/outside.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "outside.o done"

mazemaker.o : ../src/mazemaker.c ../src/mazemaker.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "mazemaker.o done"

solveur.o : ../src/solveur.c ../src/solveur.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "solveur.o done"

test.o : ../src/test.c ../src/test.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "test.o done"

cmd.o : ../src/cmd.c ../src/cmd.h
	$(CC) $(CFLAGS) -c $< $(SDLFLAGS)
	@echo "cmd.o done"

.PHONY : clean
clean :
	@rm -f ../src/*.o ../src/main
	@echo "Clean done in src (security)"
	@rm -f ../*.o ../main
	@echo "Clean done in root"
	@rm -f *.o main
	@echo "Clean done in build"
