all: somma

somma: somma.c
	gcc -o somma somma.c -pthread

test: somma test.sh
	./test.sh 10

clean:
	rm -f somma
