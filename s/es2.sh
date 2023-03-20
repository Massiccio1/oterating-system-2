#!/bin/bash
nargs=$#
args=( "$@"  )
echo $@
echo $*
echo $args
while [[ $1 != "" ]]; do
    echo "ARG=$1"
    shift
done
echo --------------
echo ${args[0]}
if test -e ${args[0]}
    then echo esiste
    
else echo non esiste
fi
echo ++++++++++++
echo vado
echo $args
#cat $DATA
tmp=$(cat $args)
(( tmp )) && echo ok || echo non ok
echo fatto
