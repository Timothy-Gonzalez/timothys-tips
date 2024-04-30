# Shared logic between the tests
# Do not run this directly!

# Constants
GREEN='\033[0;32m'
RED='\033[0;31m'
BOLD='\033[1m'
RESET='\033[0m'

SERVER="localhost:3000"

# Converts units like 1GB into their byte value
convert_to_bytes() {
    local input="$1"

    # Extract the numeric value
    local numeric_value=$(echo "$input" | sed -E 's/([0-9]+)([A-Za-z]*)/\1/g')

    # Extract the unit (if present)
    local unit=$(echo "$input" | sed -E 's/([0-9]+)([A-Za-z]*)/\2/g' | tr '[:upper:]' '[:lower:]')

    local multiplier=1
    case "$unit" in
        b)  multiplier=1 ;;
        kb|k) multiplier=$((1024)) ;;
        mb|m) multiplier=$((1024**2)) ;;
        gb|g) multiplier=$((1024**3)) ;;
        tb|t) multiplier=$((1024**4)) ;;
        *)  multiplier=1 ;;  # If no unit is provided, assume the value is already in bytes
    esac

    # Compute value
    local value=$(bc <<< "scale=0; $numeric_value * $multiplier")

    echo "$value"
}

# Find the server dir by PUTing a test file to the server, and then searching for it
# Returns 0 and echos dirname on success, returns 1 and echos errors on fail
find_server_dir() {
    # Put the test file
    put_out=$(./client-reference $SERVER PUT the_key test_files/hello.txt)
    if [ $? -ne 0 ]; then
        echo $put_out
        echo -e "${RED}${BOLD}Could not PUT test file to server on $SERVER ${RESET}"
        return 1
    fi

    # Find the most newly created key file (because there may be multiple junk ones left behind)
    local found=$(find . -name "the_key" -printf "%T@ %p\n" | sort -nr | head -n 1 | cut -f2 -d' ')

    if [ $? -ne 0 ]; then
        echo -e "${RED}${BOLD}Could not find created file (make sure your server PUT works!) ${RESET}"
        return 1
    fi

    # Echo the found dirname
    dirname $found

    return 0
}

# Creates a file of a certain size
# create_file [output] [size] [mode]
# Returns 0 on success, 1 on fail
create_file() {
    local output=$1
    local size=$2
    local mode=$3

    # dd outputs success output and error output to stderr,
    # when not verbose, we want to only output error output
    local dd_out
    if [ $mode = "v" ]; then
        dd_out=$(dd if=/dev/random of=$output bs=$size count=1)
    else
        dd_out=$(dd if=/dev/random of=$output bs=$size count=1 2>&1)
    fi

    if [ $? = 0 ]; then
        return 0
    else
        echo $dd_out
        return 1
    fi
}

# Creates n files of a certain size
# create_files [n] [output_prefix] [size] [mode]
# output_prefix is the prefix, like ./output/file will become ./output/file1, ./output/file2, etc.
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 on fail
create_files() {
    local n=$1
    local output_prefix=$2
    local size=$3

    for ((i = 1; i <= clients; i++)); do
        create_file ${output_prefix}${i} $size $mode &
        pids[$i]=$!
    done

    for ((i = 1; i <= clients; i++)); do
        pid=${pids[$i]}
        wait "$pid"
        status=$?
        if [ $status -ne 0 ]; then
            echo -e "${RED}${BOLD}Error: Failed to create file $i with $status status.${RESET}"
            return 1
        fi
    done

    return 0
}

# Runs a PUT, and validates
# run_put [server_temp_dir] [mode] [local] [remote]
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 on fail
run_put() {
    local server_temp_dir mode local remote
    server_temp_dir=$1 mode=$2 local=$3 remote=$4

    if [ "$mode" = "v" ]; then
        time ./client-debug $SERVER PUT $remote $local
    else
        ./client-debug $SERVER PUT $remote $local &>/dev/null
    fi

    diff $local $server_temp_dir/$remote -q &>/dev/null
    return $?
}

# Runs a LIST, and validates
# run_list [server_temp_dir] [mode]
# mode has no effect because we need the output
# Returns 0 on success, 1 on fail
run_list() {
    local server_temp_dir mode
    server_temp_dir=$1 mode=$2
    
    # Generate sorted expected result
    local expected_result=$(ls $server_temp_dir | sort)
    
    # Get sorted actual result
    local result
    
    if [ "$mode" = "v" ]; then
        result=$(./client-debug $SERVER LIST | sort)
    else
        result=$(./client-debug $SERVER LIST 2>/dev/null | sort)
    fi

    echo $expected_result > expected
    echo $result > result
    
    # Compare
    if [[ "$result" == "$expected_result" ]]; then
        return 0
    else
        return 1
    fi
}

# Runs a GET, and validates
# run_get [server_temp_dir] [mode] [local] [remote]
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 on fail
run_get() {
    local server_temp_dir mode local remote
    server_temp_dir=$1 mode=$2 local=$3 remote=$4

    if [ "$mode" = "v" ]; then
        time ./client-debug $SERVER GET $remote $local
    else
        ./client-debug $SERVER GET $remote $local &>/dev/null
    fi

    diff $server_temp_dir/$remote $local -q &>/dev/null
    return $?
}

# Runs a DELETE, and validates
# run_delete [server_temp_dir] [mode] [remote]
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 on fail
run_delete() {
    local server_temp_dir mode remote
    server_temp_dir=$1 mode=$2 remote=$3

    if [ "$mode" = "v" ]; then
        time ./client-debug $SERVER DELETE $remote
    else
        ./client-debug $SERVER DELETE $remote &>/dev/null
    fi

    # Check if still exists
    ls $server_temp_dir/$remote &>/dev/null

    if [ $? -ne 0 ]; then
        return 0
    else
        return 1
    fi
}

# Runs a PUT and GET, and validates
# run_put_get [server_temp_dir] [mode] [input] [remote] [output]
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 for get failed, 2 for put failed
run_put_get() {
    local server_temp_dir mode input remote output
    server_temp_dir=$1 mode=$2 input=$3 remote=$4 output=$5

    echo -e "${BOLD}Attempting PUT${RESET}"
    run_put $server_temp_dir $mode $input $remote
    if [ $? -eq 0 ]; then
        echo -e "${BOLD}Attempting GET${RESET}"
        run_get $server_temp_dir $mode $output $remote
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}${BOLD}PUT & GET Success${RESET}"
            return 0
        else
            echo -e "${RED}${BOLD}GET Failure${RESET}"
            return 1
        fi
    else
        echo -e "${RED}${BOLD}PUT failure${RESET}"
        return 2
    fi
}


# Runs a specified method
# run_put_get [method] [server_temp_dir] [mode] [input] [remote] [output]
# mode can be v for verbose or n for normal
# Returns 0 on success, 1 on fail
run_method() {
    local method server_temp_dir mode input remote output
    method=$1 server_temp_dir=$2 mode=$3 input=$4 remote=$5 output=$6

        case $method in
        PUT)
            run_put $server_temp_dir $mode $input $remote
            ;;
        LIST)
            run_list $server_temp_dir $mode
            ;;
        GET)
            run_get $server_temp_dir $mode $output $remote
            ;;
        DELETE)
            run_delete $server_temp_dir $mode $remote
            ;;
    esac
}

# Runs a specified method for all clients
# run_and_wait [temp_server_dir] [method] [clients] [mode] [input_dir] [remote_prefix] [output_dir]
# Returns 0 on success, 1 on fail
run_and_wait() {
    local temp_server_dir=$1
    local method=$2
    local clients=$3
    local mode=$4
    local input_dir=$5
    local remote_prefix=$6
    local output_dir=$7

    echo -e "${BOLD}Running $clients clients for $method...${RESET}"
    for ((i = 1; i <= clients; i++)); do
        run_method $method $temp_server_dir $mode $input_dir/$i ${remote_prefix}${i} $output_dir/$i &
        pids[$i]=$!
    done

    echo -e "${BOLD}Waiting for all clients to finish...${RESET}"
    for ((i = 1; i <= clients; i++)); do
        pid=${pids[$i]}
        wait "$pid"
        status=$?
        if [ $status -ne 0 ]; then
            echo -e "${RED}${BOLD}Error: Client $i failed to $method with $status status.${RESET}"
            return 1
        fi
    done

    echo -e "${GREEN}${BOLD}All clients $method successfully.${RESET}"
    return 0
}
