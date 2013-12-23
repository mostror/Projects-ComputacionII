TARGETS=server
CC=gcc 
CFLAGS= -Wall -lrt -pthread

all: $(TARGETS)


server: server.c setConfig.c getFile.c parser.c mode.c safemalloc.c handle.c
	clear
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)

test: server
	clear
	./server -f tp5.conf -4
