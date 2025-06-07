#!/bin/bash
for i in {2..20}
do
    if [ $((i % 2)) -eq 0 ]; then
        echo "Even number: $i"
    fi
done
