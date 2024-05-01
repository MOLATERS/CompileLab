#!/bin/bash

if [ $# -eq 0 ]
then  ls test | grep ^test > testlist
else
    ls test | grep test$1.cmm > testlist
fi      

echo "Program generating"
cd src
make parser
cd ..

if [ ! -d "./output/" ];
    then mkdir output
fi

rm ./output/log

echo `date` >> ./output/log
echo "" >> ./output/log
cat testlist | while read line
do 
echo "=========================$line==========================" >> ./output/log
echo " " >> ./output/log
./src/parser "test/$line" &>> ./output/log
echo " " >> ./output/log
done
echo " "
cat ./output/log
echo " "



