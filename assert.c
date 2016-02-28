#include "assert.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define print_description(description, type)        \
    {                                               \
        string spaces = get_level_spaces();         \
        printf("%s%s%s%s%s%s%s%s\n",                \
        type##_LINE_GAP_BEFORE,                     \
        spaces,                                     \
        type##_BULLET_COLOR,                        \
        type##_BULLET,                              \
        type##_TEXT_COLOR,                          \
        description,                                \
        RESET_COLOR,                                \
        type##_LINE_GAP_AFTER);                     \
        free(spaces);                               \
    }

#define print_failure(failure_number, description)  \
    {                                               \
        string spaces = get_level_spaces();         \
        printf("%s%s%d) %s%s\n",                    \
        spaces,                                     \
        FAILURE_BULLET_COLOR,                       \
        failure_number,                             \
        description,                                \
        RESET_COLOR);                               \
        free(spaces);                               \
    }

#define print_result_counts(count, description, type)   \
    {                                                   \
        string spaces = get_level_spaces();             \
        printf("%s%s%s%3d %s%s\n",                      \
        spaces,                                         \
        type##_BULLET_COLOR,                            \
        type##_BULLET,                                  \
        count,                                          \
        description,                                    \
        RESET_COLOR);                                   \
        free(spaces);                                   \
    }

#define print_failure_description(failure_number, description)  \
    {                                                           \
        string spaces = get_level_spaces();                     \
        printf("%s%d. %s\n",                                    \
        spaces,                                                 \
        failure_number,                                         \
        description);                                           \
        free(spaces);                                           \
    }

#define bool_to_str(val)            (val == true ? "true" : "false")

#define executor_definition(suffix, type, comparator, format)               \
    void executor_##suffix(string file,                                     \
        line ln,                                                            \
        type actual,                                                        \
        boolean output,                                                     \
        type expected)                                                      \
    {                                                                       \
        boolean is_failure = output ? !(comparator) : (comparator);         \
        if (is_failure)                                                     \
        {                                                                   \
            Assertion * assertion = assertion_create(file, ln, "Failed");   \
            Test * test = test_current();                                   \
            test_add_failure(test, assertion);                              \
        }                                                                   \
    }

#define UNDERLINE                   "\x1b[4m"
#define RESET_COLOR                 "\x1b[0m"
#define SUCCESS_BULLET_COLOR        "\x1b[32m"
#define FAILURE_BULLET_COLOR        "\x1b[31m"
#define PENDING_BULLET_COLOR        "\x1b[36m"
#define SUITE_BULLET_COLOR          "\x1b[37m"

#define SUCCESS_TEXT_COLOR          "\x1b[30m"
#define FAILURE_TEXT_COLOR          ""
#define PENDING_TEXT_COLOR          ""
#define SUITE_TEXT_COLOR            ""

#define SUCCESS_BULLET              "✓ "
#define FAILURE_BULLET              "✗ "
#define PENDING_BULLET              "• "
#define SUITE_BULLET                ""

#define SUITE_LINE_GAP_BEFORE       "\n"
#define SUITE_LINE_GAP_AFTER        ""
#define SUCCESS_LINE_GAP_BEFORE     ""
#define SUCCESS_LINE_GAP_AFTER      ""
#define FAILURE_LINE_GAP_BEFORE     ""
#define FAILURE_LINE_GAP_AFTER      ""
#define PENDING_LINE_GAP_BEFORE     ""
#define PENDING_LINE_GAP_AFTER      ""

#define MAX_ASSERTIONS_PER_TEST     64

typedef struct _Assertion
{
    string      file;   /* name of the file where assertion failed */
    line        ln;     /* line number at which assertion failed */
    string      error;  /* error message to be displayed to user when this fails*/
}Assertion;

typedef struct _Test
{
    string      description;                            /* description of the test as provided */
    boolean     is_failure;                             /* TRUE if test has failed, FALSE otherwise */
    boolean     is_pending;                             /* TRUE if test is pending for later implemention */
    Assertion * assertions[MAX_ASSERTIONS_PER_TEST];    /* assertions that belong to this test*/
    int         failed_assertion_count;                 /* number of failed assertions */
}Test;

Assertion *     assertion_create(string file, line ln, string error);
void            assertion_destory(Assertion * assertion);
Test *          test_create(string description);
void            test_add_failure(Test * test, Assertion * assertion);
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
executor_definition(string, string, strcmp(actual, expected) == 0,      "%s");

void executor_bool(string file, line ln, bool actual, boolean output, bool expected)
{
    executor_boolp(file, ln, bool_to_str(actual), output, bool_to_str(expected));
}
/* END executor definition functions */

/* SUITE */
void pre_executor_suite(string description)
{
    print_description(description, SUITE);
    g_suite_level++;
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
    Test * test = test_current();
    if (test->is_failure)
    {
        g_total_success--;
        g_total_failures++;
    }
    print_test_result();
}

void executor_test(string description, function func)
{
    pre_executor_test(description);
    func();
    post_executor_test(description);
}

/* PENDING */
void pre_executor_pending(string description)
{
    g_test_collection = (Test**)realloc(g_test_collection, sizeof(Test) * (++g_test_count));
    Test * test = test_create(description);
    test->is_pending = true;
    g_test_collection[g_test_count - 1] = test;
    g_total_success--;
    g_total_pending++;
}

void post_executor_pending(string description)
{
    Test * test = test_current();
    print_description(test->description, PENDING);
}

void executor_pending(string description)
{
    pre_executor_pending(description);
    post_executor_pending(description);
}

Test * test_create(string description)
{
    int i = 0;
    Test * test = (Test *)calloc(1, sizeof(Test));
    test->description = strdup(description);
    test->is_failure = false;
    test->is_pending = false;
    test->failed_assertion_count = 0;
    g_total_success++; // assume that this test will succeed, later we will decrement this if a test failes

    for (i = 0; i < MAX_ASSERTIONS_PER_TEST; test->assertions[i++] = NULL);

    return test;
}

void test_destroy(Test * test)
{
    int i = 0;
    free(test->description);

    for(i = 0; i < test->failed_assertion_count; i++)
    {
        assertion_destory(test->assertions[i]);
    }
    free(test);
}

Test * test_current()
{
    return g_test_collection[g_test_count - 1];
}

void test_add_failure(Test * test, Assertion * assertion)
{
    test->assertions[test->failed_assertion_count++] = assertion;
    test->is_failure = true;
}

void print_test_result()
{
    Test *test = test_current();
    if (!test->is_failure)
    {
        print_description(test->description, SUCCESS);
    }
    else
    {
        print_failure(g_total_failures, test->description);
    }
}

Assertion * assertion_create(string file, line ln, string error)
{
    Assertion * assertion = (Assertion *)calloc(1, sizeof(Assertion));
    assertion->error    = strdup(error);
    assertion->file     = strdup(file);
    assertion->ln       = ln;
}

void assertion_destory(Assertion * assertion)
{
    free(assertion->file);
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
    int i = 0, j = 0;
    int failure_number = 0;
    boolean overall_result = true;

    string spaces = get_level_spaces();
    printf("\n\n%s" UNDERLINE "SUMMARY" RESET_COLOR "\n\n", spaces);

    for(i = 0; i< g_test_count; i++)
    {
        Test * test = g_test_collection[i];
        if (test->is_failure)
        {
            overall_result = false;
            print_failure(++failure_number, test->description);
            g_suite_level++;
            for(j = 0; j < test->failed_assertion_count; j++)
            {
                Assertion * assertion = test->assertions[j];
                print_failure_description(j + 1, assertion->error);
            }
            g_suite_level--;
        }
        test_destroy(test);
    }
    free(g_test_collection);

    printf("\n\n");
    print_result_counts(g_total_success, "Successful", SUCCESS);

    if (g_total_failures > 0)
    {
        print_result_counts(g_total_failures, "Failed", FAILURE);
    }
    if (g_total_pending > 0)
    {
        print_result_counts(g_total_pending, "Pending", PENDING);
    }
    printf("\n");

    if (!overall_result)
    {
        printf("\n%s%sTEST FAILED.%s\n\n", spaces, FAILURE_BULLET_COLOR, RESET_COLOR);
    }
    else
    {
        printf("\n%s%sTEST SUCCESSFUL.%s\n\n", spaces, SUCCESS_BULLET_COLOR, RESET_COLOR);
    }

    free(spaces);
}
