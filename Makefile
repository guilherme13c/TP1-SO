CC = gcc
CFLAGS = -lpthread

sh: sh.c
	$(CC) sh.c -o sh

top: top/meutop.c
	$(CC) top/meutop.c -o meutop $(CFLAGS)

all: sh.c top/meutop.c
	$(CC) sh.c -o sh
	$(CC) top/meutop.c -o meutop $(CFLAGS)

clean: sh meutop
	rm -f sh meutop
