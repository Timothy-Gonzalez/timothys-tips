/*
==================================== Caught ====================================

                    A lightweight & simple C testing library

                      Copyright (c) 2024 Timothy Gonzalez

================================================================================

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

================================================================================

The following file is a generated combination of all source files into one.

See https://github.com/Timothy-Gonzalez/caught for the full open source.

================================================================================
*/



#include "caught.h"

/* ========================> ./src/assertion-result.c <======================= */

#include <stddef.h>
#include <stdio.h>


// General purpose converter from string (==) to operator enum (CAUGHT_OP_EQUAL)
enum caught_operator
caught_str_to_operator(char *str)
{
    int len = sizeof(CAUGHT_OPERATOR_STRS) / sizeof(CAUGHT_OPERATOR_STRS[0]);
    int i;
    for (i = 0; i < len; ++i)
    {
        if (strcmp(str, CAUGHT_OPERATOR_STRS[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

// General purpose converter from enum (CAUGHT_OP_EQUAL) to operator string (==)
const char *caught_operator_to_str(enum caught_operator operator)
{
    return CAUGHT_OPERATOR_STRS[operator];
}

// General purpose converter from enum (CAUGHT_OP_EQUAL) to a to be statement (to be, to not be, etc.)
const char *caught_operator_to_to_be_statement(enum caught_operator operator)
{
    return CAUGHT_OPERATOR_TO_BES[operator];
}

// Processes the assertion result by updating the internal state & outputting result of assertion if needed.
// Finally, returns true if assertion failed and test should exit
bool caught_internal_handle_assertion_result(caught_internal_assertion_result assertion_result)
{
    caught_internal_state.assertions += 1;
    if (assertion_result.pass)
    {
        caught_internal_state.passed_assertions += 1;
    }

    int show_regardless_of_pass = 0;

#ifdef EXPLICIT_ASSERTION_PASS
    show_regardless_of_pass = 1;
#endif

    if (!assertion_result.pass || show_regardless_of_pass)
    {
        caught_output_assertion_result(assertion_result);
    }

    free(assertion_result.lhs);
    free(assertion_result.rhs);

    return !assertion_result.pass;
}

/* ========================< ./src/assertion-result.c >======================= */


/* ===========================> ./src/evaluators.c <========================== */

#include <stdlib.h>

// Evaluators take in a left hand size, operator, and right hand side
// they then evaluate the result of that expression
// CAUGHT_GENERATE_GENERIC_EVALUATOR just uses the default operators (==, <=, >=, ...)
// but more advanced definitions are needed for things like strings

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_ptr_ptr(void **lhs, enum caught_operator operator, void ** rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_ptr(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_ptr_array(void **lhs, enum caught_operator operator, void ** rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_ptr);
}

bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_bool_ptr(bool *lhs, enum caught_operator operator, bool * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_bool(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_bool_array(bool *lhs, enum caught_operator operator, bool * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_bool);
}

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_int_ptr(int *lhs, enum caught_operator operator, int * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_int(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_int_array(int *lhs, enum caught_operator operator, int * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, operator, rhs, length, caught_internal_evaluator_int);
}

bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)
}
bool caught_internal_evaluator_char_ptr(char *lhs, enum caught_operator operator, char * rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_int(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_char_array(char *lhs, enum caught_operator operator, char * rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, operator, rhs, length, caught_internal_evaluator_char, '\0');
}

bool caught_internal_evaluator_str(char *lhs, enum caught_operator operator, char * rhs)
{
    bool null_exists = (lhs == NULL) || (rhs == NULL);
    switch (operator)
    {
    case CAUGHT_OP_EQUAL:
        return (null_exists && lhs == rhs) || (!null_exists && strcmp(lhs, rhs) == 0);
    case CAUGHT_OP_NOT_EQUAL:
        return (null_exists && lhs != rhs) || (!null_exists && strcmp(lhs, rhs) != 0);
    case CAUGHT_OP_IN:
        return !null_exists && (strstr(rhs, lhs) != NULL);
    case CAUGHT_OP_NOT_IN:
        return null_exists || (strstr(rhs, lhs) == NULL);
    case CAUGHT_OP_MATCH:
        return !null_exists && caught_internal_match(lhs, rhs);
    case CAUGHT_OP_NOT_MATCH:
        return null_exists || !caught_internal_match(lhs, rhs);
    default:
        caught_output_errorf("Cannot compare strings with %s, only == and != are supported!", caught_operator_to_str(operator));
        return false;
    }
}
bool caught_internal_evaluator_str_ptr(char **lhs, enum caught_operator operator, char ** rhs)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    return caught_internal_evaluator_str(*lhs, operator, * rhs);
}
bool caught_internal_evaluator_str_array(char **lhs, enum caught_operator operator, char ** rhs, ssize_t length)
{
    CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs);
    CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, operator, rhs, length, caught_internal_evaluator_str, NULL);
}

bool caught_internal_evaluator_exit_status(caught_internal_process_status lhs, enum caught_operator operator, caught_internal_process_status rhs)
{
    if (operator!= CAUGHT_OP_EQUAL)
    {
        caught_output_errorf("Cannot compare exit statuses with %s, only == is supported!", caught_operator_to_str(operator));
    }

    return lhs.type == rhs.type && lhs.status == rhs.status;
}

/* ===========================< ./src/evaluators.c >========================== */


/* ==============================> ./src/fork.c <============================= */


caught_internal_process_status create_caught_internal_process_status(int type, int status)
{
    caught_internal_process_status new = {
        .type = type,
        .status = status,
        .status_str = NULL,
    };
    if (type == 1 && status >= 1 && status <= 31)
    {
        new.status_str = signal_names[status - 1];
    }
    else if (type == 0 && status >= 0 && status <= 1)
    {
        new.status_str = exit_status_names[status];
    }
    return new;
}

/* ==============================< ./src/fork.c >============================= */


/* ===========================> ./src/formatters.c <========================== */


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>

char *caught_internal_formatter_ptr(void *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%p", value);
}
char *caught_internal_formatter_ptr_ptr(void **value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_ptr(*value);
}
char *caught_internal_formatter_ptr_array(void **value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_ptr)
}

char *caught_internal_formatter_bool(bool value)
{
    return (value) ? strdup("true") : strdup("false");
}
char *caught_internal_formatter_bool_ptr(bool *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_bool(*value);
}
char *caught_internal_formatter_bool_array(bool *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_bool)
}

char *caught_internal_formatter_int(int value)
{
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%i", value);
}
char *caught_internal_formatter_int_ptr(int *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_int(*value);
}
char *caught_internal_formatter_int_array(int *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY(value, length, caught_internal_formatter_int)
}

char *caught_internal_formatter_char(char value)
{
    if (value == '\0')
    {
        return strdup("'\\0'");
    }
    CAUGHT_INTERNAL_FORMATTER_FORMAT("'%c'", value)
}
char *caught_internal_formatter_char_ptr(char *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_char(*value);
}
char *caught_internal_formatter_char_array(char *value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(value, length, caught_internal_formatter_char, '\0')
}

char *caught_internal_formatter_str(char *value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_FORMAT("\"%s\"", value)
}
char *caught_internal_formatter_str_ptr(char **value)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    return caught_internal_formatter_str(*value);
}
char *caught_internal_formatter_str_array(char **value, ssize_t length)
{
    CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value)
    CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(value, length, caught_internal_formatter_str, NULL)
}

char *caught_internal_formatter_exit_status(caught_internal_process_status value)
{
    const char *type = value.type ? "Signal" : "Exit code";
    if (value.status_str)
    {
        CAUGHT_INTERNAL_FORMATTER_FORMAT("%s (%i)", value.status_str, value.status)
    }
    CAUGHT_INTERNAL_FORMATTER_FORMAT("%s %i", type, value.status)
}

/* ===========================< ./src/formatters.c >========================== */


/* ==============================> ./src/main.c <============================= */

#include <stdio.h>


int main()
{
    caught_internal_initialize_color_output();

    caught_output_printf("\n");
    caught_output_header();

    int passed_tests = 0;
    for (int i = 0; i < caught_internal_state.tests_num; ++i)
    {
        int prev_assertions = caught_internal_state.assertions;
        int prev_passed_assertions = caught_internal_state.passed_assertions;

        caught_internal_test test = caught_internal_state.tests[i];

        test.handler();

        if (caught_internal_state.original_stdin != -1)
        {
            caught_output_errorf("Must restore stdin after mocking it, did you forget to call RESTORE_STDIN()?");
        }
        if (caught_internal_state.original_stdout != -1)
        {
            caught_output_errorf("Must restore stdout after mocking it, did you forget to call RESTORE_STDOUT()?");
        }

        int this_assertions = caught_internal_state.assertions - prev_assertions;
        int this_passed_assertions = caught_internal_state.passed_assertions - prev_passed_assertions;
        int this_failed_assertions = this_assertions - this_passed_assertions;

        if (!this_failed_assertions)
        {
            passed_tests += 1;
        }

        int output = this_failed_assertions > 0;

#ifdef EXPLICIT_TEST_PASS
        output = 1;
#endif

        if (!output)
            continue;

        caught_output_printf("\n");
        caught_output_test_summary(test.name, this_passed_assertions, this_failed_assertions);
    }

    int tests = caught_internal_state.tests_num;
    int failed_tests = tests - passed_tests;
    int passed_assertions = caught_internal_state.passed_assertions;
    int failed_assertions = caught_internal_state.assertions - caught_internal_state.passed_assertions;

    caught_output_printf("\n");
    caught_output_summary("Tests:      ", passed_tests, failed_tests);
    caught_output_summary("Assertions: ", passed_assertions, failed_assertions);
    caught_output_overall_result(failed_tests == 0);

    caught_internal_cleanup_state(true);

    return failed_tests != 0;
}

/* ==============================< ./src/main.c >============================= */


/* =============================> ./src/match.c <============================= */


#include <assert.h>
#include <stdlib.h>

static caught_internal_matcher MATCHERS[] = {
    {
        .specifiers = "id",
        .whitelist = "0123456789",
        .blacklist = NULL,
        .match_all = true,
        .only_one = '\0',
    },
    {
        .specifiers = "f",
        .whitelist = "0123456789.",
        .blacklist = NULL,
        .only_one = '.',
        .match_all = true,
    },
    {
        .specifiers = "c",
        .whitelist = NULL,
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = false,
    },
    {
        .specifiers = "a",
        .whitelist = "abcdefghijklmnopqrstuvwxyABDEFGHIKLMNOPQRSWTUVWXYZ",
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = true,
    },
    {
        .specifiers = "w",
        .whitelist = NULL,
        .blacklist = "\n\r\v\f\t ",
        .only_one = '\0',
        .match_all = true,
    },
    {
        .specifiers = "s",
        .whitelist = NULL,
        .blacklist = NULL,
        .only_one = '\0',
        .match_all = true,
    },
};

static caught_internal_matcher get_matcher(char specifier)
{
    size_t i;
    for (i = 0; i < sizeof(MATCHERS) / sizeof(MATCHERS[0]); i++)
    {
        size_t j;
        char *specifiers = MATCHERS[i].specifiers;
        for (j = 0; specifiers[j] != '\0'; ++j)
        {
            if (specifier == specifiers[j])
            {
                return MATCHERS[i];
            }
        }
    }

    caught_output_errorf("Invalid format specifier: $%c", specifier);
    return (caught_internal_matcher){};
}

static caught_internal_matcher_fstr_segment *break_fstr_segments(char *fstr)
{
    size_t length = strlen(fstr);

    size_t parts = 1;

    size_t i;
    for (i = 0; i < length; ++i)
    {
        if (fstr[i] == '$')
        {
            parts += 2; // Technically wrong for $$, but still a good enough heuristic
        }
    }

    caught_internal_matcher_fstr_segment *result = malloc(sizeof(caught_internal_matcher_fstr_segment) * (parts + 1));
    size_t result_size = 0;
    caught_internal_matcher_fstr_segment segment;

    char *buffer = malloc(sizeof(char) * (length + 1));
    buffer[0] = '\0';
    int buffer_i = 0;
    for (i = 0; i <= length; ++i)
    {
        // if end of string OR found delimeter
        bool is_$ = (i < length) && (fstr[i] == '$');
        bool next_$ = (i < length - 1) && (fstr[i + 1] == '$');
        bool is_end_of_specifier = (strlen(buffer) >= 2) && (segment.is_specifier);
        bool split = (is_$ && !next_$) || (is_end_of_specifier);

        if ((i == length || split) && strlen(buffer) >= 1)
        {
            segment.segment = strdup(buffer);
            if (segment.is_specifier)
            {
                segment.matcher = get_matcher(buffer[1]);
            }
            result[result_size++] = segment;
            segment = (caught_internal_matcher_fstr_segment){};
            buffer[0] = '\0';
            buffer_i = 0;
        }

        if (i == length)
        {
            break;
        }

        if (buffer_i == 0)
        {
            segment.is_specifier = is_$ && !next_$;
        }

        buffer[buffer_i] = fstr[i];
        buffer[buffer_i + 1] = '\0';
        buffer_i++;
        if (is_$ && next_$)
        {
            i++;
        }
    }

    free(buffer);

    result[result_size] = (caught_internal_matcher_fstr_segment){
        .segment = NULL,
    };

    return result;
}

static void break_fstr_segments_cleanup(caught_internal_matcher_fstr_segment *segments)
{
    size_t i;
    for (i = 0; segments[i].segment != NULL; ++i)
    {
        free(segments[i].segment);
    }
    free(segments);
}

static bool match(char *str, caught_internal_matcher_fstr_segment *segments)
{
    caught_internal_matcher_fstr_segment segment = *segments;
    char *segment_str = segment.segment;
    if (segment_str == NULL)
    {
        // we reached the last segment & end of str, we did it!
        return *str == '\0';
    }
    caught_internal_matcher matcher = segment.matcher;

    caught_internal_matcher_fstr_segment next_segment = *(segments + 1);
    char *next_segment_str = next_segment.segment;
    bool last_segment = next_segment_str == NULL;

    // Literal matches
    if (!segment.is_specifier)
    {
        if (strncmp(str, segment_str, strlen(segment_str)) != 0)
        {
            return false;
        }
        return match(str + strlen(segment_str), segments + 1);
    }

    // Specifier matches
    bool found_match = false;

    size_t match_size = 0;
    bool only_one_flag = false;
    while (match_size < strlen(str))
    {
        char c = str[match_size];

        // Apply whitelist
        if (matcher.whitelist)
        {
            if (strchr(matcher.whitelist, c) == NULL)
            {
                break;
            }
        }

        // Apply blacklist
        if (matcher.blacklist)
        {
            if (strchr(matcher.blacklist, c) != NULL)
            {
                break;
            }
        }

        // Only one condition - if more than one we fail
        if (matcher.only_one != '\0' && c == matcher.only_one)
        {
            if (only_one_flag)
            {
                break;
            }
            only_one_flag = true;
        }

        match_size += 1;

        // Look ahead - if next non-specifier doesn't match - we can't do this
        if (!last_segment && !next_segment.is_specifier)
        {
            if (strncmp(str + match_size, next_segment_str, strlen(next_segment_str)) != 0)
            {
                continue;
            }
        }

        // Match size worked
        found_match = match(str + match_size, segments + 1);

        if (found_match)
        {
            break;
        }

        if (match_size == 1 && !matcher.match_all)
        {
            break;
        }
    }

    return found_match;
}

bool caught_internal_match(char *str, char *fstr)
{
    assert(str);
    assert(fstr);

    caught_internal_matcher_fstr_segment *segments = break_fstr_segments(fstr);
    size_t segments_len = 0;
    while (segments[segments_len].segment != NULL)
    {
        segments_len++;
    }

    bool result = match(str, segments);
    break_fstr_segments_cleanup(segments);
    return result;
}

/* =============================< ./src/match.c >============================= */


/* =============================> ./src/mocks.c <============================= */

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#ifndef __USE_POSIX
#define __USE_POSIX
#endif
#include <stdio.h>
#include <string.h>


void MOCK_STDIN(char *input)
{
    if (caught_internal_state.original_stdin != -1)
    {
        caught_output_errorf("Cannot mock already mocked stdin");
    }
    caught_internal_state.original_stdin = dup(STDIN_FILENO);

    if (caught_internal_state.original_stdin == -1)
    {
        caught_output_perrorf("Failed to dup stdin before mocking");
    }

    int stdin_pipe[2];
    if (pipe(stdin_pipe) == -1)
    {
        caught_output_perrorf("Failed to create pipe for mocked stdin");
    }
    fflush(NULL);

    if (dup2(stdin_pipe[0], STDIN_FILENO) == -1)
    {
        caught_output_perrorf("Failed to mock stdin with pipe");
    }
    if (write(stdin_pipe[1], input, strlen(input)) == -1)
    {
        caught_output_perrorf("Failed to write to stdin mock");
    }
    fflush(NULL);
    close(stdin_pipe[0]);
    close(stdin_pipe[1]);
}

void RESTORE_STDIN()
{
    if (caught_internal_state.original_stdin == -1)
    {
        caught_output_errorf("Cannot restore not mocked stdin");
    }

    fflush(NULL);

    if (dup2(caught_internal_state.original_stdin, STDIN_FILENO) == -1)
    {
        caught_output_perrorf("Failed to restore stdin");
    }

    caught_internal_state.original_stdin = -1;
}

void MOCK_STDOUT()
{
    if (caught_internal_state.original_stdout != -1)
    {
        caught_output_errorf("Cannot mock already mocked stdout");
    }

    int pipe_status = pipe(caught_internal_state.mocked_stdout_pipe);

    if (pipe_status == -1)
    {
        caught_output_perrorf("Failed to create pipe for mocked stdout");
    }

    caught_internal_state.original_stdout = dup(STDOUT_FILENO);

    fflush(NULL);

    if (caught_internal_state.original_stdout == -1)
    {
        caught_output_perrorf("Failed to dup stdout before mocking stdout");
    }

    caught_internal_state.original_stdout_file = fdopen(caught_internal_state.original_stdout, "w");

    if (caught_internal_state.original_stdout_file == NULL)
    {
        caught_output_perrorf("Failed to open file descriptor to original stdout");
    }

    if (dup2(caught_internal_state.mocked_stdout_pipe[1], STDOUT_FILENO) == -1)
    {
        caught_output_perrorf("Failed to dup2 pipe to stdout");
    }
}

char *RESTORE_STDOUT()
{
    if (caught_internal_state.original_stdout == -1)
    {
        caught_output_errorf("Cannot restore not mocked stdout");
    }

    fflush(NULL);

    close(caught_internal_state.mocked_stdout_pipe[1]);

    if (dup2(caught_internal_state.original_stdout, STDOUT_FILENO) == -1)
    {
        caught_output_perrorf("Failed to restore stdout");
    }

    char *result = malloc(CAUGHT_MOCK_BUFFER_SIZE);
    int result_size = 0;
    int result_capacity = CAUGHT_MOCK_BUFFER_SIZE;

    char *buffer = malloc(CAUGHT_MOCK_BUFFER_SIZE);

    struct pollfd poll_fds = {
        .fd = caught_internal_state.mocked_stdout_pipe[0],
        .events = POLLIN,
    };
    int res = poll(&poll_fds, 1, 0);

    if (res < 0 || poll_fds.revents & (POLLERR | POLLNVAL))
    {
        caught_output_perrorf("Failed to poll mocked output pipe");
    }

    bool has_data_to_read = poll_fds.revents & POLLIN;

    while (has_data_to_read)
    {
        ssize_t size = read(caught_internal_state.mocked_stdout_pipe[0], buffer, CAUGHT_MOCK_BUFFER_SIZE);

        if (size == -1)
        {
            caught_output_perrorf("Failed to read pipe");
        }

        if (size == 0)
        {
            break;
        }

        memcpy(result + result_size, buffer, size);

        result_size += size;

        if (result_size + CAUGHT_MOCK_BUFFER_SIZE >= result_capacity)
        {
            result_capacity *= CAUGHT_MOCK_RESULT_GROW_RATE;
            buffer = realloc(buffer, result_capacity);
        }
    }

    free(buffer);
    buffer = NULL;

    result[result_size] = '\0';

    close(caught_internal_state.mocked_stdout_pipe[0]);

    caught_internal_state.mocked_stdout_pipe[0] = -1;
    caught_internal_state.mocked_stdout_pipe[1] = -1;
    caught_internal_state.original_stdout = -1;

    fclose(caught_internal_state.original_stdout_file);
    caught_internal_state.original_stdout_file = NULL;

    return result;
}

/* =============================< ./src/mocks.c >============================= */


/* =============================> ./src/output.c <============================ */

#include <stddef.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define CAUGHT_COLOR_SUCCESS "\x1b[32m"            // ANSI green
#define CAUGHT_COLOR_BACKGROUND_SUCCESS "\x1b[42m" // ANSI background green
#define CAUGHT_COLOR_FAIL "\x1b[31m"               // ANSI red
#define CAUGHT_COLOR_BACKGROUND_FAIL "\x1b[41m"    // ANSI background red
#define CAUGHT_COLOR_WARNING "\x1b[33m"            // ANSI yellow
#define CAUGHT_COLOR_INFO "\x1b[34m"               // ANSI blue
#define CAUGHT_OUTPUT_BOLD "\x1b[1m"               // ANSI bold
#define CAUGHT_OUTPUT_RESET "\x1b[0m"              // ANSI reset

#define CAUGHT_OUTPUT_HEADER_WIDTH 80
#define CAUGHT_OUTPUT_HEADER_BOTTOM "                    A lightweight & simple C testing library\n\n" \
                                    "                      Copyright(c) 2024 Timothy Gonzalez\n\n"     \
                                    "================================================================================\n"

int caught_color_enabled = 0;

void caught_internal_initialize_color_output()
{
    const char *TERM = getenv("TERM");
    const char *COLOR = getenv("COLOR");

    if (TERM != NULL && strstr(TERM, "color") != NULL)
    {
        caught_color_enabled = 1;
    }

    if (COLOR != NULL)
    {
        caught_color_enabled = !!strcmp(COLOR, "0");
    }
}

void caught_output_success()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_COLOR_SUCCESS);
}
void caught_output_background_success()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_COLOR_BACKGROUND_SUCCESS);
}
void caught_output_fail()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_COLOR_FAIL);
}
void caught_output_background_fail()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_COLOR_BACKGROUND_FAIL);
}
void caught_output_info()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_COLOR_INFO);
}
void caught_output_bold()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_OUTPUT_BOLD);
}
void caught_output_reset()
{
    if (!caught_color_enabled)
        return;
    caught_output_printf("%s", CAUGHT_OUTPUT_RESET);
}

void caught_output_internal_error(bool use_perror, char *fstr, va_list args)
{
    if (caught_color_enabled)
    {
        fprintf(stderr, "%s", CAUGHT_OUTPUT_BOLD CAUGHT_COLOR_BACKGROUND_FAIL CAUGHT_COLOR_FAIL);
    }
    fprintf(stderr, " %s ", "✖ ERROR");

    if (caught_color_enabled)
    {
        fprintf(stderr, "%s ", CAUGHT_OUTPUT_RESET CAUGHT_COLOR_FAIL CAUGHT_OUTPUT_BOLD);
    }

    vfprintf(stderr, fstr, args);

    if (use_perror && errno != 0)
    {
        fprintf(stderr, ": %s\n", strerror(errno));
        errno = 0; // Reset errno after printing it
    }

    if (caught_color_enabled)
    {
        fprintf(stderr, "%s", CAUGHT_OUTPUT_RESET);
    }

    exit(EXIT_FAILURE);
}

void caught_output_perrorf(char *fstr, ...)
{
    va_list args;
    va_start(args, fstr);
    caught_output_internal_error(1, fstr, args);
    va_end(args);
}

void caught_output_errorf(char *fstr, ...)
{
    va_list args;
    va_start(args, fstr);
    caught_output_internal_error(0, fstr, args);
    va_end(args);
}

void caught_output_printf(char *fstr, ...)
{
    va_list args;
    va_start(args, fstr);
    FILE *output = caught_internal_state.original_stdout_file ? caught_internal_state.original_stdout_file : stdout;
    vfprintf(output, fstr, args);
    va_end(args);
}

void caught_output_header()
{
    char *header_text;
    asprintf(&header_text, " Caught %s ", CAUGHT_VERSION_STRING);
    int equalsLeft = CAUGHT_OUTPUT_HEADER_WIDTH - strlen(header_text);
    int leftEquals = equalsLeft / 2;
    int rightEquals = equalsLeft - leftEquals;

    for (; leftEquals > 0; leftEquals--)
        putchar('=');

    caught_output_printf("%s", header_text);
    free(header_text);

    for (; rightEquals > 0; rightEquals--)
        putchar('=');

    caught_output_printf("\n\n%s", CAUGHT_OUTPUT_HEADER_BOTTOM);
    caught_output_bold();
    caught_output_info();
    caught_output_printf("\nLoaded %i tests\n", caught_internal_state.tests_num);
    caught_output_reset();
}

void caught_output_status_tag(int pass)
{
    caught_output_bold();
    pass ? caught_output_success() : caught_output_fail();
    pass ? caught_output_background_success() : caught_output_background_fail();
    caught_output_printf(caught_color_enabled ? " %s " : "%s", pass ? "PASS" : "FAIL");
    caught_output_reset();
    caught_output_printf(" ");
}

void caught_output_assertion_result(caught_internal_assertion_result assertion_result)
{
    if (assertion_result.lhs == NULL || assertion_result.rhs == NULL)
    {
        caught_output_errorf("%s cannot be null", assertion_result.lhs == NULL ? "lhs" : "rhs");
    }
    caught_output_printf("\n");

    caught_output_status_tag(assertion_result.pass);
    caught_output_printf("./%s:%i:\n", assertion_result.file, assertion_result.line);
    caught_output_info();
    caught_output_printf("    %s\n", assertion_result.expression);
    caught_output_reset();

    const char *expected_statement = "expected";
    const char *to_be_statement = caught_operator_to_to_be_statement(assertion_result.operator);
    int statement_padding = (strlen(expected_statement) > strlen(to_be_statement)) ? strlen(expected_statement) : strlen(to_be_statement);

    caught_output_printf("        %s:%*s ", expected_statement, statement_padding - (int)strlen(expected_statement), "");
    caught_output_success();
    caught_output_printf("%s", assertion_result.lhs);
    caught_output_reset();
    caught_output_printf("\n        %s:%*s ", to_be_statement, statement_padding - (int)strlen(to_be_statement), "");
    assertion_result.pass ? caught_output_success() : caught_output_fail();
    caught_output_printf("%s", assertion_result.rhs);
    caught_output_reset();
    caught_output_printf("\n");
}

void caught_output_test_summary(const char *test_name, int passed, int failed)
{
    int total = passed + failed;
    caught_output_status_tag(failed == 0);
    caught_output_printf("%s: %i passed", test_name, passed);
    if (failed)
    {
        caught_output_printf(", %i failed", failed);
    }
    caught_output_printf(", %i total\n", total);
}

void caught_output_summary(const char *prefix, int passed, int failed)
{
    int total = passed + failed;
    caught_output_printf("%s", prefix);
    caught_output_success();
    if (failed == 0)
    {
        caught_output_bold();
    }
    caught_output_printf("%i passed", passed);
    caught_output_reset();
    caught_output_printf("%s", ", ");
    if (failed > 0)
    {
        caught_output_bold();
        caught_output_fail();
        caught_output_printf("%i failed", failed);
        caught_output_reset();
        caught_output_printf("%s", ", ");
    }
    caught_output_printf("%i total\n", total);
}

void caught_output_overall_result(int pass)
{
    caught_output_bold();
    pass ? caught_output_success() : caught_output_fail();
    caught_output_printf(pass ? "All tests passed!\n" : "Some tests failed\n");
    caught_output_reset();
}

/* =============================< ./src/output.c >============================ */


/* =============================> ./src/state.c <============================= */

#include <stdlib.h>
#include <stdio.h>


struct caught_internal_t caught_internal_state = {
    .assertions = 0,
    .passed_assertions = 0,
    .tests = NULL,
    .tests_num = 0,
    .tests_capacity = 0,
    .original_stdin = -1,
    .mocked_stdout_pipe = {-1, -1},
    .original_stdout = -1,
    .original_stdout_file = NULL,
    .is_parent = 1,
};

void caught_internal_cleanup_state(bool exiting)
{
    if (caught_internal_state.tests)
    {
        free(caught_internal_state.tests);
        caught_internal_state.tests = NULL;
        caught_internal_state.tests_num = 0;
        caught_internal_state.tests_capacity = 0;
    }

    if (exiting && caught_internal_state.original_stdout != -1)
    {
        RESTORE_STDOUT();
    }
}

/* =============================< ./src/state.c >============================= */


/* =============================> ./src/tests.c <============================= */

#include <stdlib.h>

void caught_add_test(const char *name, void (*handler)())
{
    if (caught_internal_state.tests == NULL)
    {
        caught_internal_state.tests_capacity = CAUGHT_INTERNAL_TESTS_INITIAL_CAPACITY;
        caught_internal_state.tests = malloc(sizeof(caught_internal_test) * caught_internal_state.tests_capacity);
    }
    else if (caught_internal_state.tests_num == caught_internal_state.tests_capacity)
    {
        caught_internal_state.tests_capacity *= CAUGHT_INTERNAL_TESTS_CAPACITY_GROW_RATE;
        caught_internal_state.tests = realloc(caught_internal_state.tests, sizeof(caught_internal_test) * caught_internal_state.tests_capacity);
    }

    const caught_internal_test test = {
        .name = name,
        .handler = handler,
    };

    caught_internal_state.tests[caught_internal_state.tests_num++] = test;
}

/* =============================< ./src/tests.c >============================= */

