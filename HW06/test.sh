#!/bin/sh
HWBIN=hw06
for i in `seq -w 1 10`
do
    PROBLEM=datapub/pub$i
    echo "Solve public input $i"
    ./$HWBIN < $PROBLEM.in >$PROBLEM.myout 2>$PROBLEM.myerr
done

for i in `seq -w 1 10`
do
    PROBLEM=datapub/pub$i
    echo "Compare public input $i"
    md5sum $PROBLEM.out $PROBLEM.myout
done

