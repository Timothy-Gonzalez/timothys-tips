#!/bin/bash

# Usage: ./tests/multi_put_get.sh (mode)
# This is a simple script which does a PUT and GET for ech of the sizes specified below
# Basically, ./tests/put_get.sh for each one

SIZES="2 4 8 16 32 64 128 256 512 1K 2K 4K 8K 16K 32K 64K 128K 256K 512K 1M 2M 4M 8M 16M 32M 64M 128M 256M 512M 1G 2G"

# Load shared functions
source tests/functions.sh

# Compile if needed
set -e
make debug
set +e

# Find server
echo -e "${BOLD}Attempting to find server...${RESET}"
SERVER_DIR=$(find_server_dir)

if [ $? -ne 0 ]; then
    echo -e $SERVER_DIR
    exit 1
fi

echo -e "${GREEN}${BOLD}Found server dir: $SERVER_DIR ${RESET}"

# PUT/GET tests

for size_str in $SIZES; do
    size=$(convert_to_bytes "$size_str")
    size_readable="$size_str ($size)"
    
    echo -e "${BOLD}Running test for size: $size_readable${RESET}"
    echo -e "${BOLD}Creating file...${RESET}"
    dd if=/dev/random of=random-in bs=$size count=1

    if [ $? -ne 0 ]; then
        exit 1
    fi

    run_put_get $SERVER_DIR v random-in random random-out
    status=$?

    # Exit if any failure
    if [ $status -ne 0 ]; then
        echo -e "${RED}${BOLD}Test failed for size: $size_readable${RESET}"
        exit $status
    fi
done

echo -e "${GREEN}${BOLD}All tests passed successfully.${RESET}"

# Cleanup (you can remove this if you want to inspect further)
rm random-in random-out &> /dev/null