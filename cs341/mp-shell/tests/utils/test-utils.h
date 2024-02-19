#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sstring.h>
#include <vector.h>

#include "../format.h"

#ifndef TEST_UTILS
#define TEST_UTILS

#define TEST_SCRIPT_FILE "./test_script_file.txt"
#define TEST_HISTORY_FILE "./test_history_file.txt"
#define TEST_TEST_FILE "./test_test_file.txt"

#define PROMPT_MATCH_DIR(dir) "(pid=$i)/$s/shell" dir "$$ "
#define PROMPT_MATCH PROMPT_MATCH_DIR("")

void remove_script_file();
void write_script_file(char *content);
void remove_history_file();
char *read_history_file();
void write_history_file(char *content);
void remove_test_file();
char *read_test_file();
void write_test_file(char *content);

// Provide arguments to run shell with (0th arg should be NULL)
void run_shell(size_t argc, char **argv);
// Runs shell with no arguments, just binary
void run_shell_no_args();
// Runs shell with script file argument
void run_shell_with_script_file();
// Runs shell with history file argument
void run_shell_with_history_file();

int get_first_child(int pid);

#endif
