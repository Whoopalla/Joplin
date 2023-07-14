CC=gcc
CFLAGS= -Wall -ggdb -g 

build: ./src/joplin.c
	$(CC) $(CFLAGS) -o joplin ./src/joplin.c
	./joplin
	magick output.ppm output.png