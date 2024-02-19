#include "utils/test-utils.h"
#include "caught/caught.h"
#include <signal.h>

// This test is very unpredictable and does not work very well, use at own risk

// TEST("sleep command with SIGINT")
// {
//     write_script_file("sleep 5\n"
//                       "echo this should run");
//     MOCK_STDOUT();
//     pid_t shell_pid = fork();

//     if (shell_pid < 0)
//     {
//         perror("Fork failed");
//         exit(EXIT_FAILURE);
//     }
//     else if (shell_pid == 0)
//     {
//         caught_internal_cleanup_state(false);
//         run_shell_with_script_file();
//     }

//     // Sleep then send SIGINT
//     usleep(500);
//     int foreground_pid = get_first_child(shell_pid);
//     kill(foreground_pid, SIGINT);

//     int status;
//     waitpid(shell_pid, &status, 0);
//     char *out = RESTORE_STDOUT();

//     EXPECT_INT(WIFEXITED(status) && WEXITSTATUS(status), ==, 0);

//     EXPECT_STR(out, match, PROMPT_MATCH "sleep 5\n"
//                                         "Command executed by pid=$i\n" PROMPT_MATCH "echo this should run\n"
//                                         "Command executed by pid=$i\n"
//                                         "this should run\n");

//     free(out);
//     remove_script_file();
// };
