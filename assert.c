#include "assert.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define print_failure(format, actual, expected)          \
    printf("Assertion failed because expected: <"format">, actual: <"format">\n", actual, expected)

#define print_description(description, spaces, bullet)              \
    printf("%s%s%s%s%s%s%s%s\n",                              \
    bullet##_LINE_GAP_BEFORE,                                       \
    spaces,                                                         \
    bullet##_COLOR,                                                 \
    bullet##_BULLET,                                                \
    bullet##_TEXT_COLOR,                                            \
    description,                                                    \
    RESET_COLOR,                                                    \
    bullet##_LINE_GAP_AFTER)

#define print_result_counts(count, description, spaces, bullet)     \
    printf("%s%s%d) %s\n",                                          \
    spaces,                                                         \
    bullet##_COLOR,                                                 \
    count,                                                          \
    description)

#define bool_to_str(val)            (val == true ? "true" : "false")

#define executor_definition(suffix, type, comparator, format)               \
    void executor_##suffix(type actual, boolean output, type expected)      \
    {                                                                       \
        boolean is_failure = output ? !(comparator) : (comparator);         \
        if (is_failure)                                                     \
        {                                                                   \
            Assertion * assertion = assertion_create("Failed");             \
            Test * test = test_current();                                   \
            test->assertions[test->failed_assertion_count++] = assertion;   \
            test->is_failure = true;                                        \
            g_total_success--;                                              \
            g_total_failures++;                                             \
        }                                                                   \
    }

#define RESET_COLOR                 "\x1b[39m"
#define SUCCESS_COLOR               "\x1b[32m"
#define FAILURE_COLOR               "\x1b[31m"
#define SUITE_COLOR                 "\x1b[37m"

#define SUCCESS_TEXT_COLOR          "\x1b[30m"
#define FAILURE_TEXT_COLOR          ""
#define SUITE_TEXT_COLOR            ""

#define SUCCESS_BULLET              "✓ "
#define FAILURE_BULLET              "✗ "
#define SUITE_BULLET                ""

#define SUITE_LINE_GAP_BEFORE       "\n"
#define SUITE_LINE_GAP_AFTER        ""
#define SUCCESS_LINE_GAP_BEFORE     ""
#define SUCCESS_LINE_GAP_AFTER      ""
#define FAILURE_LINE_GAP_BEFORE     ""
#define FAILURE_LINE_GAP_AFTER      ""

#define MAX_ASSERTIONS_PER_TEST     64

typedef struct _Assertion
{
    string error;   /* error message to be displayed to user when this fails*/
}Assertion;

typedef struct _Test
{
    string      description;                            /* description of the test as provided */
    boolean     is_failure;                             /* TRUE if test has failed, FALSE otherwise */
    Assertion * assertions[MAX_ASSERTIONS_PER_TEST];    /* assertions that belong to this test*/
    int         failed_assertion_count;                 /* number of failed assertions */
}Test;

Assertion *     assertion_create(string error);
void            assertion_destory(Assertion * assertion);
Test *          test_create(string description);
Test *          test_current();
void            print_test_result();
string          get_level_spaces();

/* global variables to hold tests, and test results*/
Test **g_test_collection;
int g_test_count = 0;
int g_suite_level = 0;
int g_total_failures = 0;
int g_total_success = 0;
int g_total_pending = 0;

/* BEGIN executor definition functions */
executor_definition(boolp,  string, strcmp(actual, expected) == 0,      "%s");
executor_definition(char,   char,   actual == expected,                 "%c");
executor_definition(short,  short,  actual == expected,                 "%d");
executor_definition(int,    int,    actual == expected,                 "%d");
executor_definition(long,   long,   actual == expected,                 "%ld");
executor_definition(float,  float,  actual == expected,                 "%f");
executor_definition(double, double, actual == expected,                 "%lf");
executor_definition(string, string, strcmp(actual, expected) == 0,    "%s");

void executor_bool(bool actual, boolean output, bool expected)
{
    executor_boolp(bool_to_str(actual), output, bool_to_str(expected));
}
/* END executor definition functions */

/* SUITE */
void pre_executor_suite(string description)
{
    string spaces = get_level_spaces();
    g_suite_level++;
    print_description(description, spaces, SUITE);

    free(spaces);
}
void post_executor_suite(string description)
{
    g_suite_level--;
}

void executor_suite(string description, function func)
{
    pre_executor_suite(description);
    func();
    post_executor_suite(description);
}

/* TEST */
void pre_executor_test(string description)
{
    g_test_collection = (Test**)realloc(g_test_collection, sizeof(Test) * (++g_test_count));
    g_test_collection[g_test_count - 1] = test_create(description);
}

void post_executor_test(string description)
{
    print_test_result();
}

void executor_test(string description, function func)
{
    pre_executor_test(description);
    func();
    post_executor_test(description);
}

Test * test_create(string description)
{
    int i = 0;
    Test * test = (Test *)calloc(1, sizeof(Test));
    test->description = strdup(description);
    test->is_failure = false;
    test->failed_assertion_count = 0;
    g_total_success++; // assume that this test will succeed, later we will decrement this if a test failes

    for (i = 0; i < MAX_ASSERTIONS_PER_TEST; test->assertions[i++] = NULL);

    return test;
}

void test_destroy(Test * test)
{
    free(test->description);
    free(test);
}

Test * test_current()
{
    return g_test_collection[g_test_count - 1];
}

void print_test_result()
{
    Test *test = test_current();
    string spaces = get_level_spaces();
    if (!test->is_failure)
    {
        print_description(test->description, spaces, SUCCESS);
    }
    else
    {
        print_description(test->description, spaces, FAILURE);
    }

    free(spaces);
}

Assertion * assertion_create(string error)
{
    Assertion * assertion = (Assertion *)calloc(1, sizeof(Assertion));
    assertion->error = strdup(error);
}

void assertion_destory(Assertion * assertion)
{
    free(assertion->error);
    free(assertion);
}

string get_level_spaces()
{
    int num_spaces = 4/*increas 4 spaces at each level*/ * g_suite_level +
    4/*minimum spaces*/;
    string spaces = (string)malloc(num_spaces + 1);
    memset(spaces, ' ', num_spaces);
    spaces[num_spaces - 1] = 0; // make it null terminated string

    return spaces;
}

__attribute__((destructor)) void after_test()
{
    printf("\n\n");
    string spaces = get_level_spaces();
    print_result_counts(g_total_success, "Successful", spaces, SUCCESS);

    if (g_total_failures > 0)
    {
        print_result_counts(g_total_failures, "Failed", spaces, FAILURE);
    }
    printf("\n");

    free(spaces);
}
