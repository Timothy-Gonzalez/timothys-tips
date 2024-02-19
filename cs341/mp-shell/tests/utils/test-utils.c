#include "test-utils.h"
#include "../shell.h"

#include <sys/types.h>
#include <dirent.h>

void remove_script_file()
{
    remove(TEST_SCRIPT_FILE);
}

void write_script_file(char *content)
{
    FILE *file = fopen(TEST_SCRIPT_FILE, "w+");
    fprintf(file, "%s", content);
    fclose(file);
}

void remove_history_file()
{
    remove(TEST_HISTORY_FILE);
}

char *read_history_file()
{
    FILE *file = fopen(TEST_HISTORY_FILE, "r");

    // Credit for fseek: https://stackoverflow.com/a/14002993
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_contents = (char *)malloc(file_size + 1);

    fread(file_contents, 1, file_size, file);
    fclose(file);

    file_contents[file_size] = '\0';

    return file_contents;
}

void write_history_file(char *content)
{
    FILE *file = fopen(TEST_HISTORY_FILE, "w+");
    fprintf(file, "%s", content);
    fclose(file);
}

void remove_test_file()
{
    remove(TEST_TEST_FILE);
}

char *read_test_file()
{
    FILE *file = fopen(TEST_TEST_FILE, "r");

    // Credit for fseek: https://stackoverflow.com/a/14002993
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *file_contents = (char *)malloc(file_size + 1);

    fread(file_contents, 1, file_size, file);
    fclose(file);

    file_contents[file_size] = '\0';

    return file_contents;
}

void write_test_file(char *content)
{
    FILE *file = fopen(TEST_TEST_FILE, "w+");
    fprintf(file, "%s", content);
    fclose(file);
}

void run_shell(size_t argc, char **args)
{
    char *path = get_full_path("./shell-test");
    args[0] = path;

    int exit_code = shell(argc, args);
    exit(exit_code);
}

void run_shell_no_args()
{
    run_shell(1, (char *[]){
                     NULL,
                     NULL,
                 });
}

void run_shell_with_script_file()
{
    run_shell(3, (char *[]){
                     NULL,
                     "-f",
                     TEST_SCRIPT_FILE,
                     NULL,
                 });
}

void run_shell_with_history_file()
{
    run_shell(3, (char *[]){
                     NULL,
                     "-h",
                     TEST_HISTORY_FILE,
                     NULL,
                 });
}

int get_first_child(int pid)
{
    // Reference source: https://man7.org/linux/man-pages/man5/proc.5.html
    char path[1024];
    snprintf(path, sizeof(path), "/proc/%d/task/%d/children", pid, pid);

    FILE *file = fopen(path, "r");

    char *line = NULL;
    size_t line_cap = 0;

    while (getline(&line, &line_cap, file) != -1)
    {
        int child = atoi(line);
        return child;
    }

    fclose(file);
    return -1;
}
