#!/bin/bash

count=10

until [ $count -lt 1 ] 
do
    echo $count
    ((count--))
done
echo "Go"
