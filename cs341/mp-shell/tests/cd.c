#include "caught/caught.h"
#include "utils/test-utils.h"

TEST("shell - cd")
{
    MOCK_STDIN(
        "cd includes\n\
ls -1\n\
cd ..\n\
cd libs\n\
ls -1\n\
cd ..\n\
exit\n");
    MOCK_STDOUT();
    EXPECT_EXIT(0, run_shell_no_args(););
    RESTORE_STDIN();
    char *out = RESTORE_STDOUT();

    char *expected =
        PROMPT_MATCH PROMPT_MATCH_DIR("/includes") "Command executed by pid=$i\ncallbacks.h\nsstring.h\nvector.h\n" PROMPT_MATCH_DIR("/includes")
            PROMPT_MATCH PROMPT_MATCH_DIR("/libs") "Command executed by pid=$i\nlibcallbacks.a\nlibsstring.a\nlibvector.a\n" PROMPT_MATCH_DIR("/libs")
                PROMPT_MATCH;

    EXPECT_STR(out, match, expected);

    free(out);
}
