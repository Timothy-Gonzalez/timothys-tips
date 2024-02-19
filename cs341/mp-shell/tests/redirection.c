#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("basic output redirection")
{
    remove_test_file();
    MOCK_STDIN(
        "echo Hello, world! > " TEST_TEST_FILE);
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n";

    EXPECT_STR(out, match, expected);

    char *out_file = read_test_file();
    char *expected_file = "Hello, world!\n";

    EXPECT_STR(out_file, ==, expected_file);

    free(out);
    free(out_file);
    remove_test_file();
}

TEST("basic append redirection")
{
    write_test_file("123\n");
    MOCK_STDIN("echo Hi >> " TEST_TEST_FILE);
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n";

    EXPECT_STR(out, match, expected);

    char *out_file = read_test_file();
    char *expected_file = "123\nHi\n";

    EXPECT_STR(out_file, ==, expected_file);

    free(out);
    free(out_file);
    remove_test_file();
}

TEST("true append redirection")
{
    write_test_file("123\nsome other content");
    MOCK_STDIN("echo You say goodbye >> " TEST_TEST_FILE "\n"
               "echo and I say hello >> " TEST_TEST_FILE);
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n" PROMPT_MATCH "Command executed by pid=$i\n";

    EXPECT_STR(out, match, expected);

    char *out_file = read_test_file();
    char *expected_file = "123\nsome other contentYou say goodbye\nand I say hello\n";

    EXPECT_STR(out_file, ==, expected_file);

    free(out);
    free(out_file);
    remove_test_file();
}

TEST("basic input redirection")
{
    write_test_file("Hello, world!\n");
    MOCK_STDIN(
        "wc --words --chars < " TEST_TEST_FILE);
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  " 2 14\n";

    EXPECT_STR(out, match, expected);

    free(out);
    remove_test_file();
}
