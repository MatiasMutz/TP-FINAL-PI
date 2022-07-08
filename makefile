COMPILER = gcc
DEBUG_COMPILER = -g -DDEBUG=1
OUTPUT_FILE = pedestrians
FILES = main.c dataADT.c lectura.c queries.c
FLAGS = -std=c99 -pedantic -Wall -g -fsanitize=address

all:
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)

debug: FLAGS += DEBUG_COMPILER
debug: all

clean:
	rm -f $(OUTPUT_FILE) *.o