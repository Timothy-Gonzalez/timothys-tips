#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("basic script file")
{
    write_script_file("echo hello");

    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_script_file(););
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "echo hello\n"
                                  "Command executed by pid=$i\n"
                                  "hello\n";

    EXPECT_STR(out, match, expected);

    free(out);
    remove_script_file();
}

TEST("complicated script file")
{
    write_script_file("echo begin!!!\n"
                      "cd includes\n"
                      "ls -1\n"
                      "echo Finished!");

    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_with_script_file(););
    char *out = RESTORE_STDOUT();

    char *expected =
        PROMPT_MATCH "echo begin!!!\n\
Command executed by pid=$i\n\
begin!!!\n" PROMPT_MATCH "cd includes\n" PROMPT_MATCH_DIR("/includes") "ls -1\n\
Command executed by pid=$i\n\
callbacks.h\n\
sstring.h\n\
vector.h\n" PROMPT_MATCH_DIR("/includes") "echo Finished!\n"
                                          "Command executed by pid=$i\n"
                                          "Finished!\n";

    EXPECT_STR(out, match, expected);

    free(out);
    remove_script_file();
}
