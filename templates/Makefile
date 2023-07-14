all: xo1 xo2

xo1: xo1.c
	gcc -o xo1 xo1.c

xo2: xo2.c
	gcc -o xo2 xo2.c

pipe:
	mkfifo pipe

test: xo1 test.sh
	./test.sh 10

clean:
	rm -f *.o xo1 xo2

# $@ nome dell'obiettivo
# $^ lista delle dipendenze