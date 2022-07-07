COMPILER = gcc
DEBUG_COMPILER = -g -DDEBUG=1
OUTPUT_FILE = pedestrians.out
FILES = pedestrians.c dataADT.c lectura.c querys.c
FLAGS = -g -std=c99 -pedantic -Wall -fsanitize=address

all:
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)
clean:
	rm -r $(OUTPUT_FILE) *.o