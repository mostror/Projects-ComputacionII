TARGETS=histo
CC=gcc 
CFLAGS= -Wall

all: $(TARGETS)


histo: histo.c inic.c setpointer.c analize.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)
red: histo
	clear
	./histo -a dog.ppm -c rojo
green: histo
	clear
	./histo -a dog.ppm -c verde
blue: histo
	clear
	./histo -a dog.ppm -c azul
test: histo
	clear
	./histo -a dog.ppm
