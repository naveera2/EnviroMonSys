#!/bin/bash

while true; do
    # Compile the C program
    gcc main2.c weather.c email.c -o binary2 -lcurl -ljson-c

    # Run the compiled program
    ./binary2

    sleep 60
done

