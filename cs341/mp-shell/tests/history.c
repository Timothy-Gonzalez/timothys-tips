#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("history #")
{
    MOCK_STDIN(
        "echo Echo This!\n\
echo Another echo\n\
!history\n\
#1\n\
#9001\n\
!history\n\
exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "Echo This!\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "Another echo\n" PROMPT_MATCH "0\techo Echo This!\n"
                                  "1\techo Another echo\n" PROMPT_MATCH "echo Another echo\n"
                                  "Command executed by pid=$i\n"
                                  "Another echo\n" PROMPT_MATCH "Invalid Index!\n" PROMPT_MATCH "0\techo Echo This!\n"
                                  "1\techo Another echo\n"
                                  "2\techo Another echo\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}

TEST("history !")
{
    MOCK_STDIN(
        "echo Echo This!\n\
echo Another echo\n\
!e\n\
!echo E\n\
!d\n\
!\n\
!history\n\
exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected = PROMPT_MATCH "Command executed by pid=$i\n"
                                  "Echo This!\n" PROMPT_MATCH "Command executed by pid=$i\n"
                                  "Another echo\n" PROMPT_MATCH "echo Another echo\n"
                                  "Command executed by pid=$i\n"
                                  "Another echo\n" PROMPT_MATCH "echo Echo This!\n"
                                  "Command executed by pid=$i\n"
                                  "Echo This!\n" PROMPT_MATCH "No Match!\n" PROMPT_MATCH "echo Echo This!\n"
                                  "Command executed by pid=$i\n"
                                  "Echo This!\n" PROMPT_MATCH "0\techo Echo This!\n"
                                  "1\techo Another echo\n"
                                  "2\techo Another echo\n"
                                  "3\techo Echo This!\n"
                                  "4\techo Echo This!\n" PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}
