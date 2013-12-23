TARGETS=server
CC=gcc 
CFLAGS= -Wall

all: $(TARGETS)


server: server.c setConfig.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)

test: server
	clear
	./server -f tp3.conf
