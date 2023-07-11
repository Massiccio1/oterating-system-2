#!/bin/bash

# Leggi la lista dei PID dalla riga di comando
PIDs=$*

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
