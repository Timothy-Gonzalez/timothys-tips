#include "caught/caught.h"
#include "utils/test-utils.h"

static char *USAGE = "./shell -f <filename> -h <history_file>\n \
        <filename>\t\tFile to read commands from\n \
        <history_file>\t\tFile to load history from and/or write history to\n";

TEST("completely wrong usage")
{
    MOCK_STDOUT();
    EXPECT_EXIT(1, {
        run_shell(
            2, (char *[]){NULL, "-wow", NULL});
    });
    char *out = RESTORE_STDOUT();

    EXPECT_STR(out, ==, USAGE);

    free(out);
}

TEST("valid args but some wrong")
{
    MOCK_STDOUT();
    EXPECT_EXIT(1, {
        run_shell(
            6, (char *[]){NULL, "-h", TEST_HISTORY_FILE, "-f", TEST_SCRIPT_FILE, "-wow", NULL});
    });
    char *out = RESTORE_STDOUT();

    EXPECT_STR(out, ==, USAGE);

    free(out);
}

TEST("missing script file")
{
    MOCK_STDOUT();
    EXPECT_EXIT(1, {
        run_shell(
            3, (char *[]){NULL, "-f", "this_file_should_not_exist.txt", NULL});
    });
    char *out = RESTORE_STDOUT();

    EXPECT_STR(out, ==, "Unable to open script file!\n");

    free(out);
}
