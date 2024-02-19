#include "caught/caught.h"
#include "utils/test-utils.h"

// If you fail this - you will fail most of these tests & ag tests
TEST("exit exit")
{
    MOCK_STDIN("exit\necho hello\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    // Should only be the prompt - nothing else
    EXPECT_STR(out, match, PROMPT_MATCH);

    free(out);
}

// If you fail this - you will fail ag tests
TEST("eof exit")
{
    MOCK_STDIN("");

    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    // Should only be the prompt - nothing else
    EXPECT_STR(out, match, PROMPT_MATCH);

    free(out);
}

// If you fail this - you will fail ag tests
TEST("eof cut off")
{
    MOCK_STDIN("echo hello");

    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    // If you output "hell", you need to be careful about trimming the result of getline
    EXPECT_STR(out, match, PROMPT_MATCH "Command executed by pid=$i\n"
                                        "hello\n");

    free(out);
}

TEST("echo hello")
{
    MOCK_STDIN("echo hello\nexit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "hello\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}

TEST("empty command")
{
    MOCK_STDIN("\nexit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  ": not found\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected)

    free(out);
}
