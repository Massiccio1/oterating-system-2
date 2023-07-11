# "all" in questo caso è sinonimo di "main"
all: main

.PHONY: main
# Per creare main servono i tre file oggetto da linkare insieme
main: main.o interruttore.o centralina.o
	gcc -o main main.o interruttore.o centralina.o -pthread

# I tre file oggetto con le loro dipendenze
interruttore.o: interruttore.c interruttore.h levetta.h
	gcc -c interruttore.c

centralina.o: centralina.c centralina.h levetta.h
	gcc -c centralina.c

main.o: main.c interruttore.h centralina.h levetta.h
	gcc -c main.c

# Pulizia: elimina l'eseguibile main e i tre file oggetto
clean:
	rm -f *.o main
