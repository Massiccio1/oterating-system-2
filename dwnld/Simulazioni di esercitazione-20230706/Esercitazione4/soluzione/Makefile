# Obiettivo di default: compila main, crea la pipe
all: main pipe

# Compilazione di main.c nell'eseguibile main
main: main.c
	gcc -o main main.c

# creazione della pipe con nome
pipe:
	mkfifo pipe

# invocazione dello script di test: è necessario creare l'eseguibile e la pipe, se non sono già presenti
test: all test.sh
	./test.sh

# Pulizia: rimuovi l'eseguibile e la pipe.
clean:
	rm -f main pipe
