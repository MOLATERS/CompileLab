#!/bin/bash

echo -e "Program clean"
make clean
echo "Program generating"
make parser

filepath="./text"
filetype=".cmm"
filename="optest"

cat textfiles | while read line
do 
echo "=======================$line=======================" 
./parser "$filepath/$line" 
done




