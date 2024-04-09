#!/bin/bash
echo "Program generating"
cd src
make parser
cd ..
ls text | while read line
do 
if [ ! -d "./output/" ];
    then mkdir output
fi
cd output
if [ -d "log" ];then
    rm log
fi
cd ..
echo "=========================$line==========================" >> ./output/log
./src/parser "text/$line" &>> ./output/log
done

cd src 
make clean
echo Program clean


