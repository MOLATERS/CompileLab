#!/bin/bash

if [ $# -eq 0 ]
then  ls text | grep ^test > testlist
else
if [ $1 == -o ]
    then ls text | grep ^op > testlist
    else ls text | grep ^test > testlist
fi
if [ $1 == -a ]
    then ls text > testlist
    else ls text | grep ^test > testlist
fi      
fi
echo "Program generating"
cd src
make parser
cd ..

if [ ! -d "./output/" ];
    then mkdir output
fi

rm ./output/log
cat testlist | while read line

do 
echo "=========================$line==========================" >> ./output/log
./src/parser "text/$line" &>> ./output/log
done

cd src 
make clean
echo Program clean


