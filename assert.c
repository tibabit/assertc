#include "assert.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

FILE *dev_null = NULL;

#define print_description(description, type, format, ...)   \
    {                                                       \
        string spaces = get_level_spaces();                 \
        printf(type##_LINE_GAP_BEFORE "%s"                  \
        type##_BULLET_COLOR                                 \
        type##_BULLET type##_TEXT_COLOR                     \
        "%s" format RESET_COLOR                             \
        type##_LINE_GAP_AFTER "\n",                         \
        spaces,                                             \
        description,                                        \
        ##__VA_ARGS__);                                     \
        free(spaces);                                       \
    }

#define print_failure(failure_number, description, format, ...) \
    {                                                           \
        string spaces = get_level_spaces();                     \
        printf("%s" FAILURE_BULLET_COLOR                        \
        "%d) %s" format RESET_COLOR "\n",                       \
        spaces,                                                 \
        failure_number,                                         \
        description,                                            \
        ##__VA_ARGS__);                                         \
        free(spaces);                                           \
    }

#define print_result_counts(count, description, type)   \
    {                                                   \
        string spaces = get_level_spaces();             \
        printf("%s" type##_BULLET_COLOR                 \
        type##_BULLET "%3d %s" RESET_COLOR"\n",         \
        spaces,                                         \
        count,                                          \
        description);                                   \
        free(spaces);                                   \
    }

#define print_failure_description(failure_number, format, ...)  \
    {                                                           \
        string spaces = get_level_spaces();                     \
        printf("%s%d. " format "\n",                            \
        spaces,                                                 \
        failure_number,                                         \
        ##__VA_ARGS__);                                         \
        free(spaces);                                           \
    }

#define start_clock()               time_t start_time = clock()
#define stop_clock()                time_t end_time = clock()
#define get_elapsed_time()          (int)((double)(end_time - start_time) / CLOCKS_PER_SEC * 1000 + 0.5)

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
            int message_length = 0;                                         \
            string error;                                                   \
            string template = get_message_template(output, format);         \
            message_length = fprintf(dev_null, template, expected, actual); \
            error = malloc(message_length);                                 \
            sprintf(error, template, expected, actual);                     \
            Assertion * assertion = assertion_create(file, ln, error);      \
            Test * test = test_current();                                   \
            test_add_failure(test, assertion);                              \
            free(template);                                                 \
            free(error);                                                    \
        }                                                                   \
    }

#define SUMMARY                     "\x1b[7m"
#define RESET_COLOR                 "\x1b[0m"
#define SUCCESS_BULLET_COLOR        "\x1b[32m"
#define FAILURE_BULLET_COLOR        "\x1b[31m"
#define PENDING_BULLET_COLOR        "\x1b[36m"
#define SUITE_BULLET_COLOR          "\x1b[37m"

#define FAILURE_BG_COLOR            "\x1b[40m"
#define SUCCESS_BG_COLOR            "\x1b[40m"

#define SUCCESS_TEXT_COLOR          "\x1b[30m"
#define FAILURE_TEXT_COLOR          ""
#define PENDING_TEXT_COLOR          ""
#define SUITE_TEXT_COLOR            "\x1b[1m"

#define OK_TIME_COLOR               "\x1b[32m"      /* time taken is between 0 and idle / 2 */
#define MEDIUM_TIME_COLOR           "\x1b[33m"      /* time taken is between idle /2 and idle */
#define TOOMUCH_TIME_COLOR          "\x1b[31m"      /* time taken is greater than idle */

#define SUCCESS_BULLET              "✓ "
#define FAILURE_BULLET              "✗ "
#define PENDING_BULLET              "! "
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
#define MAX_MODULES                 1024

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
    int         time_taken;                             /* time taken in milliseconds to finish this test*/
}Test;

Assertion *     assertion_create(string file, line ln, string error);
void            assertion_destory(Assertion * assertion);
Test *          test_create(string description);
void            test_add_failure(Test * test, Assertion * assertion);
Test *          test_current();
void            print_test_result();
string          get_level_spaces();
string          get_message_template(boolean output, string format);

/* global variables to hold tests, and test results*/
Test **g_test_collection;
int g_test_count = 0;
int g_suite_level = 0;
int g_total_failures = 0;
int g_total_success = 0;
int g_total_pending = 0;

/* global variables to hold module list */
function g_module_collection[MAX_MODULES];
int g_total_modules = 0;

/* default settings */
int g_setting_idle_time = 75;       /* maximum time which is to be considered ok for a test to execute */

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
    print_description(description, SUITE, "");
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

void post_executor_test(int time_taken)
{
    Test * test = test_current();
    test->time_taken = time_taken;
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
    start_clock();
    func();
    stop_clock();
    post_executor_test(get_elapsed_time());
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
    print_description(test->description, PENDING, "");
}

void executor_pending(string description)
{
    pre_executor_pending(description);
    post_executor_pending(description);
}

/* MODULE */
void executor_register(function func)
{
    g_module_collection[g_total_modules++] = func;
}

void executor_module(function func)
{
    func();
}

void executor_all(void)
{
    int i = 0;

    for(i = 0; i < g_total_modules; i++)
    {
        g_module_collection[i]();
    }
}

Test * test_create(string description)
{
    int i = 0;
    Test * test = (Test *)calloc(1, sizeof(Test));
    test->description = strdup(description);
    test->is_failure = false;
    test->is_pending = false;
    test->time_taken = 0;
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
    string color = NULL;
    if (test->time_taken < g_setting_idle_time / 2)
    {
        color = OK_TIME_COLOR;
    }
    else if(test->time_taken >= g_setting_idle_time / 2 && test->time_taken < g_setting_idle_time)
    {
        color = MEDIUM_TIME_COLOR;
    }
    else if (test->time_taken >= g_setting_idle_time)
    {
        color = TOOMUCH_TIME_COLOR;
    }
    if (!test->is_failure)
    {
        if (test->time_taken < g_setting_idle_time / 2)
        {
            print_description(test->description, SUCCESS, "");
        }
        else
        {
            print_description(test->description, SUCCESS, " %s(%dms)", color, test->time_taken);
        }
    }
    else
    {
        if (test->time_taken < g_setting_idle_time / 2)
        {
            print_failure(g_total_failures, test->description, "");
        }
        else
        {
            print_failure(g_total_failures, test->description, " %s(%dms)", color, test->time_taken);
        }
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

string get_message_template(boolean output, string format)
{
    string template = malloc(128);
    memset(template, 0, 128);
    sprintf(template, "Assertion Error: %sxpected <%s> but was <%s>", output ? "E" : "Not e", format, format);

    return template;
}

__attribute__((constructor)) void init_test()
{
    dev_null = fopen("/dev/null", "w");
}

__attribute__((destructor)) void after_test()
{
    int i = 0, j = 0;
    int failure_number = 0;
    boolean overall_result = true;

    string spaces = get_level_spaces();
    printf("\n\n%s" SUMMARY " SUMMARY: " RESET_COLOR "\n\n", spaces);

    for(i = 0; i< g_test_count; i++)
    {
        Test * test = g_test_collection[i];
        if (test->is_failure)
        {
            overall_result = false;
            print_failure(++failure_number, test->description, "");
            g_suite_level++;
            for(j = 0; j < test->failed_assertion_count; j++)
            {
                Assertion * assertion = test->assertions[j];
                print_failure_description(j + 1, "%s, File: %s, Line: %d", assertion->error, assertion->file, assertion->ln);
            }
            g_suite_level--;
        }
        test_destroy(test);
    }
    free(g_test_collection);

    printf("%s", overall_result == false ? "\n\n" : "");
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
        printf("\n%s" FAILURE_BG_COLOR FAILURE_BULLET_COLOR " FAILED " RESET_COLOR "\n", spaces);
    }
    else
    {
        printf("\n%s" SUCCESS_BG_COLOR SUCCESS_BULLET_COLOR " SUCCESSFUL " RESET_COLOR "\n", spaces);
    }

    free(spaces);

    fclose(dev_null);
}
