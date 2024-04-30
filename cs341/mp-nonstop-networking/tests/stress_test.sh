#!/bin/bash

# Usage: ./tests/stress_test.sh -s [size] -n [clients]
# For example: ./tests/stress_test.sh -s 50M -n 100
# Does PUTs, LISTs, GETs, and DELETEs with n clients, of size s.
# Mode is an optional argument that is by default v (verbose). You can set this to n (normal) if the output is overwhelming.
# For example: $0 -s 50M -n 100 -m n
# Recommended to start with a smaller amount of clients, and increase.
# You should also test with the inverse - large files sizes but less clients
# Note that the vm only has so much space - so you can't just test 1024 clients with 500M

INPUT_DIR=stress_test_input
OUTPUT_DIR=stress_test_output

# Load shared functions
source tests/functions.sh

# Constants
USAGE="${RED}${BOLD}Usage: $0 -s [size] -n [clients] -m (mode)
${RESET}${RED}# For example: $0 -s 50M -n 100
Does PUTs, LISTs, GETs, and DELETEs with n clients, of size s.
Mode is an optional argument that is by default v (verbose). You can set this to n (normal) if the output is overwhelming.
For example: $0 -s 50M -n 100 -m n
Recommended to start with a smaller amount of clients, and increase.
You should also test with the inverse - large files sizes but less clients
Note that the vm only has so much space - so you can't just test 1024 clients with 500M${RESET}"

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
mkdir $INPUT_DIR $OUTPUT_DIR -p
create_files $clients $INPUT_DIR/ $size $mode

if [ $? -ne 0 ]; then
    exit 1
fi

# Run tests
runwm() {
	run_and_wait $SERVER_DIR $1 $clients $mode $INPUT_DIR $OUTPUT_DIR
}

## Tests

### PUT
runwm PUT
if [ $? -ne 0 ]; then
    exit 1
fi

### LIST
runwm LIST
if [ $? -ne 0 ]; then
    exit 1
fi

### GET
runwm GET
if [ $? -ne 0 ]; then
    exit 1
fi

### DELETE
runwm DELETE
if [ $? -ne 0 ]; then
    exit 1
fi

# Cleanup (you can remove this if you want to inspect further)
rm $INPUT_DIR $OUTPUT_DIR -r &> /dev/null

# All clients finished successfully
echo -e "${GREEN}${BOLD}All clients finished successfully.${RESET}"
exit 0
