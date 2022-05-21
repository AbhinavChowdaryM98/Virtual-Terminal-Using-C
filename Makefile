CC=gcc

CFLAGS=-c -Wall

all: shell

shell: main.o parse.o prompt.o
	$(CC) main.o parse.o prompt.o -o shell

main.o: main.c
	$(CC) $(CFLAGS) main.c

parse.o: parse.c
	$(CC) $(CFLAGS) parse.c

prompt.o: prompt.c
	$(CC) $(CFLAGS) prompt.c

clean:
	rm -rf *o shell
