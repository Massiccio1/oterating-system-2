#! /bin/bash

PID=$1 #prendo il primo parametro passato

for ((i = 1; i <= $NUMERO_TEST; i++))
do
    echo Test $i
    # esegui il programma verificando se ha successo o meno
    # Ridirigi l'output su /dev/null per evitare di ingombrare l'output con stampe inutili
    if ./somma > /dev/null
    then
        echo Riuscito.
    else
        echo Fallito.
        # in questo caso non procedere oltre
        exit
    fi
done
echo Tutti i test sono riusciti.

PIDs=$*
#TUTTE LE VARABILI PASSATE  

# Ciclo infinito
while true
do
    # Alterna fra i due segnali
    for signal in SIGUSR1 SIGUSR2
    do
        # Scorri tutti i PID degli interruttori
        for pid in $PIDs
        do
            echo invio $signal a $pid
            # Invia il segnale
            kill -$signal $pid
            # Attendi un secondo
            sleep 1
        done
    done
done

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
