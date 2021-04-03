#!/bin/bash
echo "Give the program file name(binary file):"
read Program_file
echo "Give value of n:"
read n
min=0
max=4924967295
echo -n "">test/T
for ((i=1; i<=n; i++)); do
    a=$(shuf -i $min-$max -n 1)
    a=$(( $a-2147483648 ))
    b=$(shuf -i $min-$max -n 1)
    b=$(( $b-2147483648 ))
    echo "$a $b">>T
done
