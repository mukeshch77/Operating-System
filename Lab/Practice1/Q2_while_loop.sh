#!/bin/bash
while true; 
do
    read -p "Enter password: " password
    if [ "$password" == "12345" ]; then
        echo "Password correct!"
        break
    else
        echo "Incorrect password. Try again."
    fi
done


