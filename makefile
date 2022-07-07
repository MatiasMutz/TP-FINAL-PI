COMPILER = gcc
DEBUG_COMPILER = -g -DDEBUG=1
OUTPUT_FILE = pedestrians
FILES = pedestrians.c dataADT.c lectura.c queries.c
FLAGS = -std=c99 -pedantic -Wall -g -fsanitize=address

all: dataADT
	$(COMPILER) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)

dataADT: dataADT.c
clean:
	rm -r $(OUTPUT_FILE) *.o