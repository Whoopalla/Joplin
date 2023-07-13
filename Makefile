CC=gcc
CFLAGS= -Wall -ggdb

build: ./src/pixeling.c
	$(CC) $(CFLAGS) -o pixeling ./src/pixeling.c
	./pixeling
	magick output.ppm output.png