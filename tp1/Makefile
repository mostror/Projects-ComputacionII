TARGETS=tp1
CC=gcc
CFLAGS=-Wall

parse: parse.c
	$(CC) $^ -c

tp1: parse.c tp1-64.o
	$(CC) -o $@ $^ 

make test: tp1
	cat firewall.log | ./tp1
