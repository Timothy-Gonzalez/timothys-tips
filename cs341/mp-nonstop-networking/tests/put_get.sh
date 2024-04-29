#!/bin/bash

# Usage: ./tests/put_get.sh [size]
# For example: ./tests/put_get.sh 1G
# This is a simple script that attempts a PUT, and then GET, of the size requested.

# Load shared functions
source tests/functions.sh

# Compile if needed
set -e
make debug
set +e

# Constants
USAGE="${RED}${BOLD}Usage: $0 [size]\n\
${RESET}${RED}For example: $0 1G\n\
This is a simple script that attempts a PUT, and then GET, of the size requested.${RESET}"

# Parse args

## Check size is actually provided
if [ -z "$1" ]; then
    echo -e $USAGE
    exit 1
fi

size=$(convert_to_bytes "$1")
size_readable="$1 ($size)"

# Find server
echo -e "${BOLD}Attempting to find server...${RESET}"
SERVER_DIR=$(find_server_dir)

if [ $? -ne 0 ]; then
    echo -e $SERVER_DIR
    exit 1
fi

echo -e "${GREEN}${BOLD}Found server dir: $SERVER_DIR ${RESET}"

# PUT/GET test
echo -e "${BOLD}Creating file of size $size_readable${RESET}"
dd if=/dev/random of=random-in bs=$size count=1

if [ $? -ne 0 ]; then
    exit 1
fi

run_put_get $SERVER_DIR v random-in random random-out
status=$?

# Cleanup (you can remove this if you want to inspect further)
rm random-in random-out &> /dev/null

# Exit
exit $status

