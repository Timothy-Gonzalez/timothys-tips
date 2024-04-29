#!/bin/bash

# Usage: ./tests/very_many_put.sh -s [size] -n [clients] -m (mode)
# For example: ./tests/very_many_put.sh -s 512M -n 16
# Does PUTs with n clients, of size s.
# Mode is an optional argument that is by default v (verbose). You can set this to n (normal) if the output is overwhelming.
# For example: $0 -s 5M -n 1024 -m n
# Recommended to start with a smaller amount of clients, and increase.
# You should use large file sizes (> 256 M) to ensure blocking occurs.

INPUT_DIR=very_many_put_input

# Load shared functions
source tests/functions.sh

# Constants
USAGE="${RED}${BOLD}Usage: $0 -s [size] -n [clients] -m (mode)
${RESET}${RED}For example: ./tests/very_many_put.sh -s 512M -n 16
Does PUTs with n clients, of size s.
Mode is an optional argument that is by default v (verbose). You can set this to n (normal) if the output is overwhelming.
For example: $0 -s 5M -n 1024 -m n
Recommended to start with a smaller amount of clients, and increase.
You should use large file sizes (> 512 M) to ensure blocking occurs.${RESET}"

# Parse args
size_raw=""
clients=""
mode="v"

while getopts ":s:n:m:" opt; do
	case $opt in
		s)
			size_raw="$OPTARG"
			;;
		n)
			clients="$OPTARG"
			;;
		m)
			mode="$OPTARG"
			;;
		\?)
			echo -e "${USAGE}"
			exit 1
			;;
		:)
			echo -e "${USAGE}"
			exit 1
			;;
	esac
done

# Check if size and clients are provided
if [ -z "$size_raw" ] || [ -z "$clients" ]; then
  echo -e "${USAGE}"
  exit 1
fi

# Check if mode is valid
if [ $mode != "v" ] && [ $mode != "n" ]; then
	echo -e "${RED}${BOLD}mode must be v (verbose) or n (normal)!"
	echo -e "${USAGE}"
	exit 1
fi

size=$(convert_to_bytes "$size_raw")
size_readable="$size_raw ($size)"

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

# Create files
mkdir $INPUT_DIR -p

echo -e "${BOLD}Creating $clients files of size $size_readable...${RESET}"
for ((i = 1; i <= clients; i++)); do
    dd if=/dev/random of=$INPUT_DIR/random$i bs=$size count=1 &>/dev/null &
    if (( $i % 8 == 0)); then
        wait
    fi
done
wait

# Run tests
run_and_wait $SERVER_DIR PUT $clients $mode $INPUT_DIR

# Cleanup (you can remove this if you want to inspect further)
rm $INPUT_DIR -r &> /dev/null

# All clients finished successfully
echo -e "${GREEN}${BOLD}All clients finished successfully.${RESET}"
exit 0
