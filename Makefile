CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lm
SDLFLAGS = `sdl2-config --cflags --libs`

all: build main
	@echo "compilation done"

build:
	mkdir -p build
	@echo "build created"

main: build/main.o build/struct.o build/outside.o build/mazemaker.o build/solveur.o build/test.o build/cmd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(SDLFLAGS)
	@echo "main done"

build/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "main.o done"

build/struct.o: src/struct.c src/struct.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "struct.o done"

build/outside.o: src/outside.c src/outside.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "outside.o done"

build/mazemaker.o: src/mazemaker.c src/mazemaker.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "mazemaker.o done"

build/solveur.o: src/solveur.c src/solveur.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "solveur.o done"

build/test.o: src/test.c src/test.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "test.o done"

build/cmd.o: src/cmd.c src/cmd.h
	$(CC) $(CFLAGS) -c $< -o $@ $(SDLFLAGS)
	@echo "cmd.o done"

.PHONY: clean
clean:
	rm -f build/*.o main
	@echo "Clean done"