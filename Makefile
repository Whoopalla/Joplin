CC=gcc
CFLAGS= -Wall -Wextra -ggdb -g 

build: ./src/joplin.c
	$(CC) $(CFLAGS) -o joplin ./src/joplin.c
	./joplin
	magick output.ppm output.png

examples: ./src/examples.c
	$(CC) $(CFLAGS) -o examples ./src/examples.c
	mkdir examples_build
	./examples