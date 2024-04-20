#!/bin/bash

# This is a large PUT/GET script to test random file PUTs & GETs of 2 to 2^32 bytes
# If a test fails, it is recommended to test that specific test outside of this script,
# since all success/error output is stripped (&>/dev/null)

# Constants
GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
RESET='\033[0m'

# Compile if needed
make debug

# Find server
./client-reference localhost:3000 PUT the_key test_files/hello.txt &>/dev/null
DIR=$(dirname $(find . -name the_key) 2>/dev/null)

if [ $? -ne 0 ]; then
    echo -e "${RED}${BOLD}Could not find server on localhost:3000 ${RESET}"
    exit 1
fi

echo -e "${GREEN}Found server dir: $DIR${RESET}"

# PUT/GET test
size=2
max_size=$((2**32))
successful=0
total=0

while [ $size -le $max_size ]; do
    echo "Attempting size $size"
    dd if=/dev/random of=random-in bs=$size count=1 &>/dev/null
    ./client-debug localhost:3000 PUT random random-in &>/dev/null
    diff $DIR/random random-in -q &>/dev/null
    if [ $? -eq 0 ]; then
        ./client-debug localhost:3000 GET random random-out &>/dev/null
        diff random-in random-out -q &>/dev/null
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}PUT & GET Success${RESET}"
            ((successful++))
        else
            echo -e "${RED}GET Failure${RESET}"
        fi
    else
        echo -e "${RED}PUT failure${RESET}"
    fi

    rm random-in random-out &>/dev/null
    ((total++))
    size=$((size * 2))
done

echo

if [ $successful -eq $total ]; then
    echo -e "${GREEN}${BOLD}All $total tests passed${RESET}"
else
    echo -e "${RED}${BOLD}$successful/$total failed${RESET}"
fi
