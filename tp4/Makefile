TARGETS=editor starter
CC=gcc 
CFLAGS= -Wall -lrt -lm

all: $(TARGETS)


editor: editor.c inic.c openImg.c filter.c ppm.c
	$(CC) $(CFLAGS) -o $@ $^

starter: starter.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS)


testlowpass: editor starter
	clear
	./starter -i dog.ppm -o lowpass.ppm -f lowpass & ./editor

testweigmean: editor starter
	clear
	./starter -i dog.ppm -o weigmean.ppm -f weigmean & ./editor

testlaplace: editor starter
	clear
	./starter -i dog.ppm -o laplace.ppm -f laplace & ./editor

