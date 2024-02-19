#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("writes history file")
{
    remove_history_file();
    MOCK_STDIN("echo 1\n"
               "echo 2\n"
               "echo 3\n"
               "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_history_file(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "1\n" PROMPT_MATCH
                                  "Command executed by pid=$i\n"
                                  "2\n" PROMPT_MATCH
                                  "Command executed by pid=$i\n"
                                  "3\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    char *history = read_history_file();

    EXPECT_STR(history, ==, "echo 1\n"
                            "echo 2\n"
                            "echo 3");

    free(out);
    free(history);
}

TEST("reads history file & doesn't change it")
{
    write_history_file("echo 123\n"
                       "echo 456");
    MOCK_STDIN("!history\n"
               "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_history_file(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "0\techo 123\n"
                                  "1\techo 456\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    char *history = read_history_file();

    EXPECT_STR(history, ==, "echo 123\n"
                            "echo 456");

    free(out);
    free(history);
    remove_history_file();
}

TEST("reads history file & writes new commands")
{
    write_history_file("echo 123\n"
                       "echo 456");
    MOCK_STDIN("!e\n"
               "!echo 1\n"
               "echo 789\n"
               "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_history_file(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "echo 456\n"
                                  "Command executed by pid=$i\n"
                                  "456\n" PROMPT_MATCH "echo 123\n"
                                  "Command executed by pid=$i\n"
                                  "123\n" PROMPT_MATCH
                                  "Command executed by pid=$i\n"
                                  "789\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    char *history = read_history_file();

    EXPECT_STR(history, ==, "echo 123\n"
                            "echo 456\n"
                            "echo 456\n"
                            "echo 123\n"
                            "echo 789");

    free(out);
    free(history);
    remove_history_file();
}

TEST("reads history file & writes new commands, complex")
{
    write_history_file("echo 123; echo 456");
    MOCK_STDIN("!e\n"
               "echo 789; echo 000\n"
               "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_history_file(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "echo 123; echo 456\n"
                                  "Command executed by pid=$i\n"
                                  "123\n"
                                  "Command executed by pid=$i\n"
                                  "456\n" PROMPT_MATCH
                                  "Command executed by pid=$i\n"
                                  "789\n"
                                  "Command executed by pid=$i\n"
                                  "000\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    char *history = read_history_file();

    EXPECT_STR(history, ==, "echo 123; echo 456\n"
                            "echo 123; echo 456\n"
                            "echo 789; echo 000");

    free(out);
    free(history);
    remove_history_file();
}
