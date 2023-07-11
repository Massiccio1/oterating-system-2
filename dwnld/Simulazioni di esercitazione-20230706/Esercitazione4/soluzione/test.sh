#!/bin/bash

# Apertura di un secondo terminale nel quale avviare ./main
# notare la "&" alla fine: non vogliamo attendere la conclusione del processo per proseguire.
gnome-terminal -- ./main &

# Il nuovo processo scrive il proprio PID nella pipe, noi lo leggiamo con "cat" e lo memorizziamo nella variabile d'ambiente PID1
PID1=$(cat pipe)
# Stampiamo il PID del processo appena avviato
echo Processo $PID1 avviato.

sleep 5 # Attesa di 5s da specifiche; il processo ./main stamperà le prime 5 righe

# Per chiedere il numero progressivo a cui è arrivato il processo,
# inviamogli il segnale SIGUSR1 e leggiamo il numero dalla pipe
kill -SIGUSR1 $PID1
PROGRESSIVO=$(cat pipe)
echo Il processo $PID1 è arrivato al numero $PROGRESSIVO.

sleep 5 # Attesa di 5s da specifiche; il processo ./main stamperà altre 5 righe

# Per sdoppiare il processo,
# inviamogli il segnale SIGALRM e leggiamo dalla pipe il PID del processo figlio nella variabile d'ambiente PID2:
kill -SIGALRM $PID1
PID2=$(cat pipe)
echo Processo $PID2 avviato.

sleep 5 # Attesa di 5s da specifiche; i due processi stamperanno altre 5 righe ciascuno.

# Per rallentare il secondo processo,
# inviamogli il segnale SIGUSR2 seguito dalla scrittura del periodo (in secondi) nella pipe
# NOTA BENE - la scrittura nella pipe deve essere eseguita *dopo* l'invio del segnale perché
# tiene lo script bloccato finché il processo dall'altro capo non la apre e la legge.
kill -SIGUSR2 $PID2
echo 2 > pipe
echo Processo $PID2 rallentato.

sleep 30 # Attesa di 30s da specifiche; il primo processo stamperà altre 30 righe, il secondo la metà circa.

# Per terminare i processi, invio il segnale di default (SIGTERM)
echo Termino i processi.
kill $PID1 $PID2
