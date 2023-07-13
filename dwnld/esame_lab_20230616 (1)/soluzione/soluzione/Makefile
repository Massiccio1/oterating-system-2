# Il Makefile potrebbe essere notevolmente semplificato, ma scelgo di esplicitare tutte le dipendenze e i comandi

# Obiettivo di default (il primo)
all: passo1 passo2 passo3 passo4 passo1b

passo1: passo1.o giocatore.o
	gcc -o passo1 passo1.o giocatore.o

giocatore.o: giocatore.c giocatore.h
	gcc -c giocatore.c

passo1.o: passo1.c giocatore.h
	gcc -c passo1.c

passo2: passo2.o funzioni_genitore.o giocatore.o
	gcc -o passo2 passo2.o funzioni_genitore.o giocatore.o -pthread

passo2.o: passo2.c funzioni_genitore.h giocatore.h
	gcc -c passo2.c

funzioni_genitore.o: funzioni_genitore.c funzioni_genitore.h
	gcc -c funzioni_genitore.c

passo3: passo3.o funzioni_genitore.o
	gcc -o passo3 passo3.o funzioni_genitore.o -pthread

passo3.o: passo3.c funzioni_genitore.h
	gcc -c passo3.c

passo4: passo4.o funzioni_genitore.o
	gcc -o passo4 passo4.o funzioni_genitore.o -pthread

passo4.o: passo4.c funzioni_genitore.h
	gcc -c passo4.c

passo1b: passo1b.o giocatore.o
	gcc -o passo1b passo1b.o giocatore.o

passo1b.o: passo1b.c giocatore.h
	gcc -c passo1b.c

clean:
	rm -f *.o passo1 passo2 passo3 passo4 passo1b
