#!/bin/bash

#Compile the C program
gcc main1.c weather.c email.c -o binary1 -lcurl -ljson-c

#Run the compiled program
./binary1


