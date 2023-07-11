#! /bin/bash

# Invia un segnale SIGUSR1/2 ogni tre secondi al processo il cui PID è
# passato come argomento.
#
# Esempio: ./xo.sh 12345

# Il PID da invocare è passato come primo argomento
PID=$1

# Ripeti all'infinito
while true
do
    # Attendi tre secondi
    sleep 3
    # Invia il segnale per inibile la conversione o->x
    kill -SIGUSR1 $PID
    # Attendi altri tre secondi
    sleep 3
    # Invia il segnale per ripristinare la conversione o->x
    kill -SIGUSR2 $PID
done
