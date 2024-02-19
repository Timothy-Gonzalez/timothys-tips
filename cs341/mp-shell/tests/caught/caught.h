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




/* =============================> ./src/caught.h <============================ */

// The central entrypoint of caught
// You should include this file from your tests
// #include "caught.h"

#define CAUGHT_VERSION_MAJOR 0
#define CAUGHT_VERSION_MINOR 3
#define CAUGHT_VERSION_PATCH 6
#define CAUGHT_VERSION_STRING "v0.3.6"

#ifndef CAUGHT_LIB
#define CAUGHT_LIB


/* ========================> ./src/assertion-macros.h <======================= */

#ifndef CAUGHT_ASSERTIONS
#define CAUGHT_ASSERTIONS


/* ========================> ./src/assertion-result.h <======================= */

#include <stdbool.h>

#ifndef CAUGHT_ASSERTION_RESULT
#define CAUGHT_ASSERTION_RESULT

enum caught_operator
{
    CAUGHT_OP_EQUAL,
    CAUGHT_OP_NOT_EQUAL,
    CAUGHT_OP_LESS_THAN,
    CAUGHT_OP_GREATER_THAN,
    CAUGHT_OP_LESS_THAN_EQ,
    CAUGHT_OP_GREATER_THAN_EQ,
    CAUGHT_OP_IN,
    CAUGHT_OP_NOT_IN,
    CAUGHT_OP_MATCH,
    CAUGHT_OP_NOT_MATCH,
};

static char *CAUGHT_OPERATOR_STRS[] = {
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "in",
    "not in",
    "match",
    "not match",
};

static char *CAUGHT_OPERATOR_TO_BES[] = {
    "to be",
    "to not be",
    "to be less than",
    "to be greater than",
    "to be <= to",
    "to be >= to",
    "to be in",
    "to not be in",
    "to match",
    "to not match",
};

enum caught_operator
caught_str_to_operator(char *str);
const char *caught_operator_to_str(enum caught_operator operator);
const char *caught_operator_to_to_be_statement(enum caught_operator operator);

typedef struct caught_internal_assertion_result
{
    const char *file;
    const int line;
    const char *expression;
    char *lhs;
    char *rhs;
    enum caught_operator operator;
    bool pass;
} caught_internal_assertion_result;

bool caught_internal_handle_assertion_result(caught_internal_assertion_result assertion_result);

#endif

/* ========================< ./src/assertion-result.h >======================= */


/* ===========================> ./src/formatters.h <========================== */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>
#include <stdbool.h>

/* ==============================> ./src/fork.h <============================= */

#ifndef CAUGHT_FORK
#define CAUGHT_FORK

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


/* =============================> ./src/output.h <============================ */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifndef CAUGHT_OUTPUT
#define CAUGHT_OUTPUT

void caught_internal_initialize_color_output();

void caught_output_success();
void caught_output_background_success();
void caught_output_fail();
void caught_output_background_fail();
void caught_output_warning();
void caught_output_info();
void caught_output_bold();
void caught_output_reset();

void caught_output_internal_error(bool use_perror, char *fstr, va_list args);
void caught_output_perrorf(char *fstr, ...);
void caught_output_errorf(char *fstr, ...);
void caught_output_printf(char *fstr, ...);

void caught_output_header();
void caught_output_status_tag(int pass);
void caught_output_assertion_result(caught_internal_assertion_result assertion_result);
void caught_output_test_summary(const char *test_name, int passed, int failed);
void caught_output_generic_summary(const char *prefix, int passed, int failed);
void caught_output_summary(const char *prefix, int passed, int failed);
void caught_output_overall_result(int pass);

extern int caught_color_enabled;

#endif

/* =============================< ./src/output.h >============================ */


typedef struct caught_internal_process_status
{
    int type;               // 0 for exit status, 1 for signal status
    int status;             // The exit status or signal, depending on above
    const char *status_str; // The string signal, if there is one
} caught_internal_process_status;

static const char *exit_status_names[] = {"EXIT_SUCCESS", "EXIT_FAILURE"};

static const char *signal_names[] = {
    "SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS",
    "SIGFPE", "SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
    "SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP", "SIGTTIN",
    "SIGTTOU", "SIGURG", "SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
    "SIGIO", "SIGPWR", "SIGSYS"};

caught_internal_process_status create_caught_internal_process_status(int type, int status);

#define CAUGHT_INTERNAL_FORK(child_execute_block)                                              \
    caught_internal_process_status caught_internal_fork_child_status = {};                     \
    fflush(NULL);                                                                              \
    pid_t caught_internal_pid = fork();                                                        \
    if (caught_internal_pid == -1)                                                             \
    {                                                                                          \
        caught_output_perrorf("Failed to fork\n");                                             \
    }                                                                                          \
    if (caught_internal_pid == 0)                                                              \
    {                                                                                          \
        caught_internal_cleanup_state(false);                                                  \
        child_execute_block                                                                    \
                                                                                               \
            caught_output_perrorf("Fork segment must call exit to prevent fork bombs\n");      \
    }                                                                                          \
    else                                                                                       \
    {                                                                                          \
        int caught_internal_status = 0;                                                        \
        waitpid(caught_internal_pid, &caught_internal_status, 0);                              \
        if (WIFEXITED(caught_internal_status))                                                 \
        {                                                                                      \
            caught_internal_fork_child_status =                                                \
                create_caught_internal_process_status(0, WEXITSTATUS(caught_internal_status)); \
        }                                                                                      \
        else if (WIFSIGNALED(caught_internal_status))                                          \
        {                                                                                      \
            caught_internal_fork_child_status =                                                \
                create_caught_internal_process_status(1, WTERMSIG(caught_internal_status));    \
        }                                                                                      \
    }

#endif

/* ==============================< ./src/fork.h >============================= */


#ifndef CAUGHT_FORMATTERS
#define CAUGHT_FORMATTERS

#define CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE 16
#define CAUGHT_INTERNAL_FORMATTER_ARRAY_GROWTH_RATE 2

// Formatters take a value of a certain type
// they must return a dynamically allocated string that represents that value

char *caught_internal_formatter_ptr(void *value);
char *caught_internal_formatter_ptr_ptr(void **value);
char *caught_internal_formatter_ptr_array(void **value, ssize_t length);

char *caught_internal_formatter_bool(bool value);
char *caught_internal_formatter_bool_ptr(bool *value);
char *caught_internal_formatter_bool_array(bool *value, ssize_t length);

char *caught_internal_formatter_int(int value);
char *caught_internal_formatter_int_ptr(int *value);
char *caught_internal_formatter_int_array(int *value, ssize_t length);

char *caught_internal_formatter_char(char value);
char *caught_internal_formatter_char_ptr(char *value);
char *caught_internal_formatter_char_array(char *value, ssize_t length);

char *caught_internal_formatter_str(char *value);
char *caught_internal_formatter_str_ptr(char **value);
char *caught_internal_formatter_str_array(char **value, ssize_t length);

char *caught_internal_formatter_exit_status(caught_internal_process_status value);

#define CAUGHT_INTERNAL_FORMATTER_FORMAT(fstr, ...) \
    char *result;                                   \
    asprintf(&result, fstr, __VA_ARGS__);           \
    return result;
#define CAUGHT_INTERNAL_FORMATTER_NULL_GUARD(value) \
    if (value == NULL)                              \
    {                                               \
        return strdup("NULL");                      \
    }
#define CAUGHT_INTERNAL_FORMATTER_ARRAY(array_exp, array_length, formatter) \
    if (array_length <= 0)                                                  \
    {                                                                       \
        caught_output_errorf("Invalid length of array: %lu", array_length); \
    }                                                                       \
    char *result = malloc(CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE);     \
    size_t result_size = 0;                                                 \
    size_t result_capacity = CAUGHT_INTERNAL_FORMATTER_ARRAY_INITAL_SIZE;   \
    result[0] = '{';                                                        \
    result[1] = ' ';                                                        \
    result[2] = '\0';                                                       \
    result_size += 2;                                                       \
    int i;                                                                  \
    for (i = 0; i < array_length; ++i)                                      \
    {                                                                       \
        char *formatted = formatter(array_exp[i]);                          \
        if (i != 0)                                                         \
        {                                                                   \
            char *old_formatted = formatted;                                \
            asprintf(&formatted, ", %s", old_formatted);                    \
            free(old_formatted);                                            \
        }                                                                   \
        size_t size = strlen(formatted);                                    \
        while (result_size + size + 3 > result_capacity)                    \
        {                                                                   \
            result_capacity *= CAUGHT_INTERNAL_FORMATTER_ARRAY_GROWTH_RATE; \
            result = realloc(result, result_capacity);                      \
        }                                                                   \
        memcpy(result + result_size, formatted, size);                      \
        result_size += size;                                                \
        free(formatted);                                                    \
    }                                                                       \
    result[result_size] = ' ';                                              \
    result[result_size + 1] = '}';                                          \
    result[result_size + 2] = '\0';                                         \
    return result;

#define CAUGHT_INTERNAL_FORMATTER_ARRAY_ALLOW_NULL_TERMINATOR(array_exp, array_length, formatter, terminator) \
    if (array_length < 0)                                                                                     \
    {                                                                                                         \
        array_length = -1;                                                                                    \
        do                                                                                                    \
        {                                                                                                     \
            array_length += 1;                                                                                \
        } while (array_exp[array_length] != terminator);                                                      \
        array_length += 1; /* Include terminator */                                                           \
    }                                                                                                         \
    CAUGHT_INTERNAL_FORMATTER_ARRAY(array_exp, array_length, formatter)

#endif

/* ===========================< ./src/formatters.h >========================== */


/* ===========================> ./src/evaluators.h <========================== */

#include <stdbool.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

#ifndef CAUGHT_EVALUATORS
#define CAUGHT_EVALUATORS

bool caught_internal_evaluator_ptr(void *lhs, enum caught_operator operator, void * rhs);
bool caught_internal_evaluator_ptr_ptr(void **lhs, enum caught_operator operator, void ** rhs);
bool caught_internal_evaluator_ptr_array(void **lhs, enum caught_operator operator, void ** rhs, ssize_t length);

bool caught_internal_evaluator_bool(bool lhs, enum caught_operator operator, bool rhs);
bool caught_internal_evaluator_bool_ptr(bool *lhs, enum caught_operator operator, bool * rhs);
bool caught_internal_evaluator_bool_array(bool *lhs, enum caught_operator operator, bool * rhs, ssize_t length);

bool caught_internal_evaluator_int(int lhs, enum caught_operator operator, int rhs);
bool caught_internal_evaluator_int_ptr(int *lhs, enum caught_operator operator, int * rhs);
bool caught_internal_evaluator_int_array(int *lhs, enum caught_operator operator, int * rhs, ssize_t length);

bool caught_internal_evaluator_char(char lhs, enum caught_operator operator, char rhs);
bool caught_internal_evaluator_char_ptr(char *lhs, enum caught_operator operator, char * rhs);
bool caught_internal_evaluator_char_array(char *lhs, enum caught_operator operator, char * rhs, ssize_t length);

bool caught_internal_evaluator_str_match(char *lhs, char *fstr);

bool caught_internal_evaluator_str(char *lhs, enum caught_operator operator, char * rhs);
bool caught_internal_evaluator_str_ptr(char **lhs, enum caught_operator operator, char ** rhs);
bool caught_internal_evaluator_str_array(char **lhs, enum caught_operator operator, char ** rhs, ssize_t length);

bool caught_internal_evaluator_exit_status(caught_internal_process_status lhs, enum caught_operator operator, caught_internal_process_status rhs);

// Uses default operators (==, <=, >=, ...) to compare lhs to rhs
#define CAUGHT_GENERATE_GENERIC_EVALUATOR(lhs, operator, rhs)                          \
    switch (operator)                                                                  \
    {                                                                                  \
    case CAUGHT_OP_EQUAL:                                                              \
        return lhs == rhs;                                                             \
    case CAUGHT_OP_NOT_EQUAL:                                                          \
        return lhs != rhs;                                                             \
    case CAUGHT_OP_LESS_THAN:                                                          \
        return lhs < rhs;                                                              \
    case CAUGHT_OP_GREATER_THAN:                                                       \
        return lhs > rhs;                                                              \
    case CAUGHT_OP_LESS_THAN_EQ:                                                       \
        return lhs <= rhs;                                                             \
    case CAUGHT_OP_GREATER_THAN_EQ:                                                    \
        return lhs >= rhs;                                                             \
    default:                                                                           \
        caught_output_errorf("Invalid operator %s", caught_operator_to_str(operator)); \
        return false;                                                                  \
    }

// If lhs or rhs in null, return the expected result for that operator
#define CAUGHT_GENERATE_GENERIC_EVALUATOR_NULL_GUARD(lhs, operator, rhs)                                                                     \
    if (lhs == NULL || rhs == NULL)                                                                                                          \
    {                                                                                                                                        \
        bool equal = lhs == rhs;                                                                                                             \
        bool equal_operator = (operator== CAUGHT_OP_EQUAL) || (operator== CAUGHT_OP_GREATER_THAN_EQ) || (operator== CAUGHT_OP_LESS_THAN_EQ); \
        return (equal && equal_operator) || (!equal && !equal_operator);                                                                     \
    }

// Uses evaluator on each element of array
#define CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs_exp, op_exp, rhs_exp, length, evaluator)                                \
    if (length <= 0)                                                                                                \
    {                                                                                                               \
        caught_output_errorf("Invalid length of array: %lu", length);                                               \
    }                                                                                                               \
    bool pass_all = true;                                                                                           \
    bool pass_any = false;                                                                                          \
    bool in_op = (op_exp == CAUGHT_OP_IN || op_exp == CAUGHT_OP_NOT_IN);                                            \
    bool pass_any_op = (op_exp == CAUGHT_OP_NOT_EQUAL || op_exp == CAUGHT_OP_IN);                                   \
    int i;                                                                                                          \
    for (i = 0; i < length; ++i)                                                                                    \
    {                                                                                                               \
        bool pass;                                                                                                  \
        if (!in_op)                                                                                                 \
        {                                                                                                           \
            pass = evaluator(lhs_exp[i], op_exp, rhs_exp[i]);                                                       \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            pass = evaluator(*lhs_exp, op_exp == CAUGHT_OP_IN ? CAUGHT_OP_EQUAL : CAUGHT_OP_NOT_EQUAL, rhs_exp[i]); \
        }                                                                                                           \
                                                                                                                    \
        if (pass)                                                                                                   \
        {                                                                                                           \
            pass_any = true;                                                                                        \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
            pass_all = false;                                                                                       \
        }                                                                                                           \
    }                                                                                                               \
    return (!pass_any_op && pass_all) || (pass_any_op && pass_any);

#define CAUGHT_GENERATE_EVALUATOR_ARRAY_ALLOW_NULL_TERMINATOR(lhs, op, rhs, length, evaluator, terminator) \
    if (length < 0)                                                                                        \
    {                                                                                                      \
        length = -1;                                                                                       \
        do                                                                                                 \
        {                                                                                                  \
            length += 1;                                                                                   \
        } while (rhs[length] != terminator);                                                               \
        length += 1; /* Include terminator */                                                              \
    }                                                                                                      \
    CAUGHT_GENERATE_EVALUATOR_ARRAY(lhs, op, rhs, length, evaluator)

#endif

/* ===========================< ./src/evaluators.h >========================== */


/* =============================> ./src/state.h <============================= */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>


/* =============================> ./src/tests.h <============================= */

#ifndef CAUGHT_TESTS
#define CAUGHT_TESTS

typedef struct caught_internal_test
{
    const char *name;
    void (*handler)(void);
} caught_internal_test;

void caught_add_test(const char *name, void (*handler)());

#define CAUGHT_INTERNAL_STRING(a) #a
#define CAUGHT_INTERNAL_CONCAT(a, b) a##b
#define CAUGHT_INTERNAL_EXPAND_AND_CONCAT(a, b) CAUGHT_INTERNAL_CONCAT(a, b)
#define CAUGHT_INTERNAL_TEST_UNIQUE_NAME(base) CAUGHT_INTERNAL_EXPAND_AND_CONCAT(base, __LINE__)

#define TEST(name)                                                                            \
    static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_)();                               \
    __attribute__((constructor)) static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(__test_case_)() \
    {                                                                                         \
        caught_add_test(name, CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_));                  \
    }                                                                                         \
    static void CAUGHT_INTERNAL_TEST_UNIQUE_NAME(test_line_)()

#endif

/* =============================< ./src/tests.h >============================= */


#ifndef CAUGHT_STATE
#define CAUGHT_STATE

#define CAUGHT_INTERNAL_TESTS_INITIAL_CAPACITY 8;
#define CAUGHT_INTERNAL_TESTS_CAPACITY_GROW_RATE 2;

struct caught_internal_t
{
    int assertions, passed_assertions;
    caught_internal_test *tests;
    int tests_num;
    int tests_capacity;
    int original_stdin;
    int mocked_stdout_pipe[2];
    int original_stdout;
    FILE *original_stdout_file;
    bool is_parent;
};

extern struct caught_internal_t caught_internal_state;

void caught_internal_cleanup_state(bool exiting);

#endif

/* =============================< ./src/state.h >============================= */


// This is used by every expect define to handle taking lhs, op, rhs, & send them into their handlers.
// These handlers then determine how to display (format) the passed data, and whether the assertion passed (comparators).
// Finally, these results are combined into a assertion result and sent to the result handler, which outputs and keeps track
// of assertions accordingly.
//
// Note: do while is required to have non-conflicting scope if multiple assertions are used
#define CAUGHT_INTERNAL_EXPECT_HANDLE(expression_str, type_lhs, type_rhs, lhs_exp, operator_exp, rhs_exp, evaluated, formated_lhs, formated_rhs) \
    do                                                                                                                                           \
    {                                                                                                                                            \
        type_lhs caught_internal_lhs = (lhs_exp);                                                                                                \
        type_rhs caught_internal_rhs = (rhs_exp);                                                                                                \
        enum caught_operator caught_internal_op = caught_str_to_operator(#operator_exp);                                                         \
        caught_internal_assertion_result caught_internal_assertion_result = {                                                                    \
            .file = __FILE__,                                                                                                                    \
            .line = __LINE__,                                                                                                                    \
            .expression = expression_str,                                                                                                        \
            .lhs = formated_lhs,                                                                                                                 \
            .rhs = formated_rhs,                                                                                                                 \
            .operator= caught_internal_op,                                                                                                       \
            .pass = evaluated,                                                                                                                   \
        };                                                                                                                                       \
        if (caught_internal_handle_assertion_result(caught_internal_assertion_result))                                                           \
        {                                                                                                                                        \
            return;                                                                                                                              \
        }                                                                                                                                        \
    } while (0);

// Convience mapping to make expression str
#define CAUGHT_INTERNAL_EXPECT_EXPRESSION_STR(func_postfix, lhs_str, op_str, rhs_str) \
    "EXPECT_" #func_postfix "( " lhs_str " " op_str " " rhs_str " )"

// Convience mapping for when type & formatter are both the same
#define CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(func_postfix, type, lhs, op, rhs, evaluator, formatter)      \
    CAUGHT_INTERNAL_EXPECT_HANDLE(CAUGHT_INTERNAL_EXPECT_EXPRESSION_STR(func_postfix, #lhs, #op, #rhs),    \
                                  type, type, lhs, op, rhs,                                                \
                                  evaluator(caught_internal_lhs, caught_internal_op, caught_internal_rhs), \
                                  formatter(caught_internal_lhs), formatter(caught_internal_rhs))

// Convience mapping for passing length as an argument, and when type & formatter are both the same
#define CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(func_postfix, type, lhs, op, rhs, length_exp, evaluator, formatter)                              \
    do                                                                                                                                       \
    {                                                                                                                                        \
        size_t caught_internal_array_length = length_exp;                                                                                    \
        CAUGHT_INTERNAL_EXPECT_HANDLE(CAUGHT_INTERNAL_EXPECT_EXPRESSION_STR(func_postfix, #lhs, #op, #rhs),                                  \
                                      type, type, lhs, op, rhs,                                                                              \
                                      evaluator(caught_internal_lhs, caught_internal_op, caught_internal_rhs, caught_internal_array_length), \
                                      formatter(caught_internal_lhs, caught_internal_array_length),                                          \
                                      formatter(caught_internal_rhs, caught_internal_array_length))                                          \
    } while (0);

// Convience mapping for passing length as argument, lhs is the element, rhs is the array
#define CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(func_postfix, type_lhs, type_rhs, lhs, op, rhs, length_exp, evaluator, formatter_lhs, formatter_rhs) \
    do                                                                                                                                                   \
    {                                                                                                                                                    \
        size_t caught_internal_array_length = length_exp;                                                                                                \
        CAUGHT_INTERNAL_EXPECT_HANDLE(CAUGHT_INTERNAL_EXPECT_EXPRESSION_STR(func_postfix, #lhs, #op, #rhs),                                              \
                                      type_lhs, type_rhs, lhs, op, rhs,                                                                                  \
                                      evaluator(&caught_internal_lhs, caught_internal_op, caught_internal_rhs, caught_internal_array_length),            \
                                      formatter_lhs(caught_internal_lhs),                                                                                \
                                      formatter_rhs(caught_internal_rhs, caught_internal_array_length))                                                  \
    } while (0);

// Convience mapping for having the same types & formatters, but custom expression str
#define CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC_CUSTOM_EXPR(expression_str, type, lhs, op, rhs, evluator, formatter) \
    CAUGHT_INTERNAL_EXPECT_HANDLE(expression_str, type, type, lhs, op, rhs,                                        \
                                  evluator(caught_internal_lhs, caught_internal_op, caught_internal_rhs),          \
                                  formatter(caught_internal_lhs), formatter(caught_internal_rhs))

// Convience mapping for forking & expecting a certain status
#define CAUGHT_INTERNAL_EXPECT_HANDLE_TERMINATE(func_postfix, expected_status, expected_status_str, execute_block) \
    do                                                                                                             \
    {                                                                                                              \
        CAUGHT_INTERNAL_FORK(execute_block)                                                                        \
        CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC_CUSTOM_EXPR(                                                         \
            CAUGHT_INTERNAL_EXPECT_EXPRESSION_STR(func_postfix, "process_status", "==", expected_status_str),      \
            caught_internal_process_status, caught_internal_fork_child_status, ==, expected_status,                \
            caught_internal_evaluator_exit_status, caught_internal_formatter_exit_status)                          \
    } while (0);

#define EXPECT(expr)                                   \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC_CUSTOM_EXPR( \
        "EXPECT( " #expr " )", bool, expr, ==, true,   \
        caught_internal_evaluator_bool, caught_internal_formatter_bool)
#define EXPECT_NOT(expr)                               \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC_CUSTOM_EXPR( \
        "EXPECT( " #expr " )", bool, expr, ==, false,  \
        caught_internal_evaluator_bool, caught_internal_formatter_bool)

#define EXPECT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(PTR, void *, lhs, op, rhs, caught_internal_evaluator_ptr, caught_internal_formatter_ptr)
#define EXPECT_PTR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(PTR_PTR, void **, lhs, op, rhs, caught_internal_evaluator_ptr_ptr, caught_internal_formatter_ptr_ptr)
#define EXPECT_PTR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(PTR_ARRAY, void **, lhs, op, rhs, length, caught_internal_evaluator_ptr_array, caught_internal_formatter_ptr_array)
#define EXPECT_PTR_ARRAY_ELEMENT(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(PTR_ARRAY_ELEMENT, void *, void **, lhs, op, rhs, length, caught_internal_evaluator_ptr_array, caught_internal_formatter_ptr, caught_internal_formatter_ptr_array)

#define EXPECT_BOOL(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(BOOL, bool, lhs, op, rhs, caught_internal_evaluator_bool, caught_internal_formatter_bool)
#define EXPECT_BOOL_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(BOOL_PTR, bool *, lhs, op, rhs, caught_internal_evaluator_bool_ptr, caught_internal_formatter_bool_ptr)
#define EXPECT_BOOL_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(BOOL_ARRAY, bool *, lhs, op, rhs, length, caught_internal_evaluator_bool_array, caught_internal_formatter_bool_array)
#define EXPECT_BOOL_ARRAY_ELEMENT(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(BOOL_ARRAY_ELEMENT, bool, bool *, lhs, op, rhs, length, caught_internal_evaluator_bool_array, caught_internal_formatter_bool, caught_internal_formatter_bool_array)

#define EXPECT_INT(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(INT, int, lhs, op, rhs, caught_internal_evaluator_int, caught_internal_formatter_int)
#define EXPECT_INT_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(INT_PTR, int *, lhs, op, rhs, caught_internal_evaluator_int_ptr, caught_internal_formatter_int_ptr)
#define EXPECT_INT_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(INT_ARRAY, int *, lhs, op, rhs, length, caught_internal_evaluator_int_array, caught_internal_formatter_int_array)
#define EXPECT_INT_ARRAY_ELEMENT(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(INT_ARRAY_ELEMENT, int, int *, lhs, op, rhs, length, caught_internal_evaluator_int_array, caught_internal_formatter_int, caught_internal_formatter_int_array)

#define EXPECT_CHAR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(CHAR, char, lhs, op, rhs, caught_internal_evaluator_char, caught_internal_formatter_char)
#define EXPECT_CHAR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(CHAR_PTR, char *, lhs, op, rhs, caught_internal_evaluator_char_ptr, caught_internal_formatter_char_ptr)
#define EXPECT_CHAR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(CHAR_ARRAY, char *, lhs, op, rhs, length, caught_internal_evaluator_char_array, caught_internal_formatter_char_array)
#define EXPECT_CHAR_ARRAY_ELEMENT(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(CHAR_ARRAY_ELEMENT, char, char *, lhs, op, rhs, length, caught_internal_evaluator_char_array, caught_internal_formatter_char, caught_internal_formatter_char_array)

#define EXPECT_STR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(STR, char *, lhs, op, rhs, caught_internal_evaluator_str, caught_internal_formatter_str)
#define EXPECT_STR_PTR(lhs, op, rhs) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_GENERIC(STR_PTR, char **, lhs, op, rhs, caught_internal_evaluator_str_ptr, caught_internal_formatter_str_ptr)
#define EXPECT_STR_ARRAY(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY(STR_ARRAY, char **, lhs, op, rhs, length, caught_internal_evaluator_str_array, caught_internal_formatter_str_array)
#define EXPECT_STR_ARRAY_ELEMENT(lhs, op, rhs, length) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_ARRAY_ELEMENT(STR_ARRAY_ELEMENT, char *, char **, lhs, op, rhs, length, caught_internal_evaluator_str_array, caught_internal_formatter_str, caught_internal_formatter_str_array)

#define EXPECT_EXIT(expected_status, execute_block) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_TERMINATE(EXIT, create_caught_internal_process_status(0, expected_status), #expected_status, execute_block)

#define EXPECT_SIGNAL(expected_signal, execute_block) \
    CAUGHT_INTERNAL_EXPECT_HANDLE_TERMINATE(SIGNAL, create_caught_internal_process_status(1, expected_signal), #expected_signal, execute_block)

#endif

/* ========================< ./src/assertion-macros.h >======================= */


/* =============================> ./src/mocks.h <============================= */

#ifndef CAUGHT_MOCKS
#define CAUGHT_MOCKS

static const int CAUGHT_MOCK_BUFFER_SIZE = 1024;
static const int CAUGHT_MOCK_RESULT_GROW_RATE = 2;

void MOCK_STDIN(char *input);
void RESTORE_STDIN();

void MOCK_STDOUT();
char *RESTORE_STDOUT();

#endif

/* =============================< ./src/mocks.h >============================= */


#endif

/* =============================< ./src/caught.h >============================ */


/* =============================> ./src/config.h <============================ */

#ifndef CAUGHT_CONFIG
#define CAUGHT_CONFIG

// #define EXPLICIT_TEST_PASS      // Enabled output for passed tests as well as failed ones
// #define EXPLICIT_ASSERTION_PASS // Enable output for passed assertions as well as failed ones

#endif

/* =============================< ./src/config.h >============================ */


/* =============================> ./src/match.h <============================= */

#include <stdbool.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <string.h>

#ifndef CAUGHT_MATCH
#define CAUGHT_MATCH

struct caught_internal_matcher
{
    char *specifiers; // Specifiers for this matcher
    char *whitelist;  // Whitelist characters
    char *blacklist;  // Blacklist characters
    char only_one;    // Character that can only be seen once, '\0' if not set
    bool match_all;   // Only match one if false
};

typedef struct caught_internal_matcher caught_internal_matcher;

struct caught_internal_matcher_fstr_segment
{
    bool is_specifier;
    caught_internal_matcher matcher;
    char *segment;
};
typedef struct caught_internal_matcher_fstr_segment caught_internal_matcher_fstr_segment;

/*
 * Returns if lhs matches rhs
 *
 * Match specifiers are specified with $
 *
 * $i, $d = match a integer (0-9)
 * $c = match a single character
 * $f = match a float (0-9, a single . allowed)
 * $a = match alpha-chars (a-z and A-Z)
 * $w = match a word (matches anything non-whitespace)
 * $s = match a string (at least 1 character)
 * $$ = a literal $
 *
 * Anything else = literal match
 *
 * Supports backtracing so "abc" will match "$s$s" but "a" won't
 */
bool caught_internal_match(char *str, char *fstr);

#endif

/* =============================< ./src/match.h >============================= */

