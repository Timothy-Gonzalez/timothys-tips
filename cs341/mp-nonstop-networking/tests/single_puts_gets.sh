#!/bin/bash

# This is a simple script which PUTs and GETs a random file of $size (defined on the line below)
# You should try sizes up to 2^32. many_puts_gets.sh is a good stress test for what size you need to test
# This script does not hide any output

# Change the size below
size=32768

# Constants
GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
RESET='\033[0m'

# Compile if needed
make debug

# Find server
./client-reference localhost:3000 PUT the_key test_files/hello.txt
DIR=$(dirname $(find . -name the_key) 2>/dev/null)

if [ $? -ne 0 ]; then
    echo -e "${RED}${BOLD}Could not find server on localhost:3000 ${RESET}"
    exit 1
fi

echo -e "${GREEN}Found server dir: $DIR${RESET}"

# PUT/GET test
echo -e "${BOLD}Attempting size $size${RESET}"
dd if=/dev/random of=random-in bs=$size count=1

./client-debug localhost:3000 PUT random random-in
diff $DIR/random random-in -q &>/dev/null
if [ $? -eq 0 ]; then
    ./client-debug localhost:3000 GET random random-out
    diff random-in random-out -q &>/dev/null
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}${BOLD}Size $size PUT & GET Success${RESET}"
        ((successful++))
    else
        echo -e "${RED}${BOLD}Size $size GET Failure${RESET}"
    fi
else
    echo -e "${RED}${BOLD}PUT failure${RESET}"
fi

rm random-in random-out &>/dev/null
