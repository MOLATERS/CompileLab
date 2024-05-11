#!/bin/bash

if [ $# -eq 0 ]
then  ls test | grep ^test > testlist
else
    ls test | grep test$1.cmm > testlist
fi      

echo "Program generating"
cd src
make
cd ..

if [ ! -d "./output/" ];
    then mkdir output
fi

rm ./output/*.ir

cat testlist | while read line
do 
./src/parser "test/$line" &> ./output/$line.ir
done



