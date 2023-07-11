#!/bin/bash

# Leggi il numero di test dal primo parametro
NUMERO_TEST=$1

# Ripeti per il numero di iterazioni indicato
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
