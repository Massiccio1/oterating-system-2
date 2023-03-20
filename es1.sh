#!/bin/bash
#echo $DATA ; [ -f $DATA ] && echo file ;  [ -d $DATA ] && echo dir ; [ ! -e $DATA ] && echo non esiste ;
echo $DATA
if test -e $DATA
    then echo esiste
    
else echo non esiste
fi
echo vado
#cat $DATA
tmp=$(cat $DATA)
(( tmp )) && echo ok || echo non ok
echo fatto
