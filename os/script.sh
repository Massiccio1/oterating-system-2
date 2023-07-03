#!/usr/bin/env bash

lista=$1

l=$(cat lista)
k=0

echo ${l[@]}

for i in ${!lista[@]}; do

    echo ${lista[$i]} 

    ((k=i%2))
    echo $k;

    if [ $k -lt 1 ]; then
	    echo ${lista[$i]}
        
	else
	    echo ${lista[$i]} 1>&2
	fi
    shift
done