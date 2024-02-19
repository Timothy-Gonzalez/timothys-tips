#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("logical separator")
{
    MOCK_STDIN(
        "echo 1; echo 2\n"
        "bad_cmd; echo 3\n"
        "echo 4; bad_cmd2\n"
        "bad_cmd3; bad_cmd4\n"
        "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "1\n"
                                  "Command executed by pid=$i\n"
                                  "2\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd: not found\n"
                                  "Command executed by pid=$i\n"
                                  "3\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "4\n"
                                  "Command executed by pid=$i\n"
                                  "bad_cmd2: not found\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd3: not found\n"
                                  "Command executed by pid=$i\n"
                                  "bad_cmd4: not found\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}

TEST("logical and")
{
    MOCK_STDIN(
        "echo 1 && echo 2\n"
        "bad_cmd && echo 3\n"
        "echo 4 && bad_cmd2\n"
        "bad_cmd3 && bad_cmd4\n"
        "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "1\n"
                                  "Command executed by pid=$i\n"
                                  "2\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd: not found\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "4\n"
                                  "Command executed by pid=$i\n"
                                  "bad_cmd2: not found\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd3: not found\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}

TEST("logical or")
{
    MOCK_STDIN(
        "echo 1 || echo 2\n"
        "bad_cmd || echo 3\n"
        "echo 4 || bad_cmd2\n"
        "bad_cmd3; bad_cmd4\n"
        "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "1\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd: not found\n"
                                  "Command executed by pid=$i\n"
                                  "3\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "4\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "bad_cmd3: not found\n"
                                  "Command executed by pid=$i\n"
                                  "bad_cmd4: not found\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}

TEST("logic && history")
{
    MOCK_STDIN(
        "echo 1; echo 2\n"
        "echo 3 && echo 4\n"
        "echo 5 || echo 6\n"
        "!history\n"
        "#0\n"
        "#1\n"
        "#2\n"
        "exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "1\n"
                                  "Command executed by pid=$i\n"
                                  "2\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "3\n"
                                  "Command executed by pid=$i\n"
                                  "4\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "5\n" PROMPT_MATCH "0\techo 1; echo 2\n"
                                  "1\techo 3 && echo 4\n"
                                  "2\techo 5 || echo 6\n" PROMPT_MATCH "echo 1; echo 2\n"
                                  "Command executed by pid=$i\n"
                                  "1\n"
                                  "Command executed by pid=$i\n"
                                  "2\n" PROMPT_MATCH "echo 3 && echo 4\n"
                                  "Command executed by pid=$i\n"
                                  "3\n"
                                  "Command executed by pid=$i\n"
                                  "4\n" PROMPT_MATCH "echo 5 || echo 6\n"
                                  "Command executed by pid=$i\n"
                                  "5\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}
