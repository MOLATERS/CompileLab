#!/bin/bash
echo "Program generating"
cd src
make parser
cd ..
if [ ! -d "./output/" ];
    then mkdir output
fi
rm ./output/log
ls text | while read line
do 
echo "=========================$line==========================" >> ./output/log
./src/parser "text/$line" &>> ./output/log
done

cd src 
make clean
echo Program clean


