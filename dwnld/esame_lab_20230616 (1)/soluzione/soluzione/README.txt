Le funzioni main() sono contenute nei file passo*.c

La funzione giocatore() è contenuta nei file giocatore.[hc]

Le funzioni in comune ai passi 2, 3 e 4 sono contenute, per soli motivi di praticità, in funzioni_genitore.[hc]

I file passo[234].c differiscono per poche righe, si può vedere la differenza con l'utility diff. Ad esempio:

  diff passo2.c passo3.c

Il comando "make" compila tutti i passi.
