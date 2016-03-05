#include "assert.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "runner.h"
#include "spec_reporter.h"

FILE *dev_null = NULL;

#define start_clock()               time_t start_time = clock()
#define stop_clock()                time_t end_time = clock()
#define get_elapsed_time()          (int)((double)(end_time - start_time) / CLOCKS_PER_SEC * 1000 + 0.5)

#define bool_to_str(val)            (val == true ? "true" : "false")


/* COMPARISON FUNCTIONS */
#define comparator(actual, expected, operator)              (actual operator expected)
#define comparator_bool(actual, expected, operator)         comparator(actual, expected, operator)
#define comparator_char(actual, expected, operator)         comparator(actual, expected, operator)
#define comparator_short(actual, expected, operator)        comparator(actual, expected, operator)
#define comparator_int(actual, expected, operator)          comparator(actual, expected, operator)
#define comparator_long(actual, expected, operator)         comparator(actual, expected, operator)
#define comparator_float(actual, expected, operator)        comparator(actual, expected, operator)
#define comparator_double(actual, expected, operator)       comparator(actual, expected, operator)
#define comparator_string(actual, expected, operator)       (strcmp(actual, expected) operator 0)

#define comparer(suffix, type)                                              \
    bool_t comparer_equal_##suffix(type actual, type expected)              \
    {                                                                       \
        return comparator_##suffix(actual, expected, ==);                   \
    }                                                                       \
    bool_t comparer_above_##suffix(type actual, type expected)              \
    {                                                                       \
        return comparator_##suffix(actual, expected, >);                    \
    }                                                                       \
    bool_t comparer_above_or_equal_##suffix(type actual, type expected)     \
    {                                                                       \
        return comparator_##suffix(actual, expected, >=);                   \
    }                                                                       \
    bool_t comparer_below_##suffix(type actual, type expected)              \
    {                                                                       \
        return comparator_##suffix(actual, expected, <);                    \
    }                                                                       \
    bool_t comparer_below_or_equal_##suffix(type actual, type expected)     \
    {                                                                       \
        return comparator_##suffix(actual, expected, <=);                   \
    }

#define executor_operator_definition(suffix, type)                                                          \
    bool_t executor_operator_##suffix(type actual, type expected, operator_t operator)                      \
    {                                                                                                       \
            return operator == operator_equal ? comparer_equal_##suffix(actual, expected)                   \
            : (operator == operator_above ? comparer_above_##suffix(actual, expected)                       \
            : (operator == operator_above_or_equal ? comparer_above_or_equal_##suffix(actual, expected)     \
            : (operator == operator_below ? comparer_below_##suffix(actual, expected)                       \
            : comparer_below_or_equal_##suffix(actual, expected))));                                        \
    }

comparer(bool, bool_t)
comparer(char, char)
comparer(short, short)
comparer(int, int)
comparer(long, long)
comparer(float, float)
comparer(double, double)
comparer(string, string_t)

executor_operator_definition(bool, bool_t)
executor_operator_definition(char, char)
executor_operator_definition(short, short)
executor_operator_definition(int, int)
executor_operator_definition(long, long)
executor_operator_definition(float, float)
executor_operator_definition(double, double)
executor_operator_definition(string, string_t)

/* test executor function for primitive types  */
#define executor_definition_primitive(suffix, type, format)                                 \
    void executor_##suffix(string_t file,                                                   \
        line_t ln,                                                                          \
        type actual,                                                                        \
        bool_t output,                                                                      \
        type expected,                                                                      \
        operator_t operator)                                                                \
    {                                                                                       \
        bool_t result = executor_operator_##suffix(actual, expected, operator);             \
        bool_t is_failure = output ? !(result) : (result);                                  \
        if (is_failure)                                                                     \
        {                                                                                   \
            int message_length = 0;                                                         \
            string_t template = malloc(10);                                                 \
            string_t actual_value;                                                          \
            string_t expected_value;                                                        \
                                                                                            \
            sprintf(template, "%s", format);                                                \
                                                                                            \
            message_length = fprintf(dev_null, template, actual);                           \
            actual_value = malloc(message_length + 1);                                      \
            sprintf(actual_value, template, actual);                                        \
                                                                                            \
            message_length = fprintf(dev_null, template, expected);                         \
            expected_value = malloc(message_length + 1);                                    \
            sprintf(expected_value, template, expected);                                    \
                                                                                            \
            assertion_t * assertion = assertion_create(file, ln, operator,                  \
                output, actual_value, expected_value);                                      \
            test_t * test = test_current(g_test_runner);                                    \
            test_add_failure(test, assertion);                                              \
                                                                                            \
            free(actual_value);                                                             \
            free(expected_value);                                                           \
            free(template);                                                                 \
        }                                                                                   \
    }

/* test executor function for array types  */
#define executor_definition_array(suffix, type, format)                                         \
    void executor_array_##suffix(string_t file,                                                 \
        line_t ln,                                                                              \
        type * actual,                                                                          \
        size_t actual_len,                                                                      \
        bool_t output,                                                                          \
        type * expected,                                                                        \
        operator_t operator)                                                                    \
    {                                                                                           \
        size_t i = 0;                                                                           \
        while(i < actual_len)                                                                   \
        {                                                                                       \
            bool_t result = executor_operator_##suffix(actual[i], expected[i], operator);       \
            bool_t is_failure = output ? !(result) : (result);                                  \
            if (is_failure)                                                                     \
            {                                                                                   \
                int message_length = 0;                                                         \
                string_t template = malloc(10);                                                 \
                string_t actual_value;                                                          \
                string_t expected_value;                                                        \
                                                                                                \
                sprintf(template, "%s at position %s", format, "%d");                           \
                                                                                                \
                message_length = fprintf(dev_null, template, actual[i], i);                     \
                actual_value = malloc(message_length + 1);                                      \
                sprintf(actual_value, template, actual[i], i);                                  \
                                                                                                \
                message_length = fprintf(dev_null, template, expected[i], i);                   \
                expected_value = malloc(message_length + 1);                                    \
                sprintf(expected_value, template, expected[i], i);                              \
                                                                                                \
                assertion_t * assertion = assertion_create(file, ln, operator,                  \
                    output, actual_value, expected_value);                                      \
                test_t * test = test_current(g_test_runner);                                    \
                test_add_failure(test, assertion);                                              \
                                                                                                \
                free(actual_value);                                                             \
                free(expected_value);                                                           \
                free(template);                                                                 \
                break; /* stop if a check failes*/                                              \
            }                                                                                   \
            i++;                                                                                \
        }                                                                                       \
    }


/* >>>>>>>>>>>>>>>> BEGIN: global variables  <<<<<<<<<<<<<<<< */

test_runner_t *g_test_runner;
test_reporter_t *g_test_reporter;
function g_module_collection[MAX_MODULES];
int g_total_modules = 0;

/* >>>>>>>>>>>>>>>> END: global variables  <<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>> BEGIN: helper function declarations  <<<<<<<<<<<<<<<< */

void print_test_result();

/* >>>>>>>>>>>>>>>> END: helper function declarations  <<<<<<<<<<<<<<<< */

 /* >>>>>>>>>>>>>>>> BEGIN: default settings <<<<<<<<<<<<<<<< */

int g_setting_idle_time = 75;       /* maximum time which is to be considered ok for a test to execute */

/* >>>>>>>>>>>>>>>> END: default settings <<<<<<<<<<<<<<<< */

/* >>>>>>>>>>>>>>>> BEGIN: executor definition functions <<<<<<<<<<<<<<<< */

executor_definition_primitive(bool,     bool_t,     "%d");
executor_definition_primitive(char,     char,       "%c");
executor_definition_primitive(short,    short,      "%d");
executor_definition_primitive(int,      int,        "%d");
executor_definition_primitive(long,     long,       "%ld");
executor_definition_primitive(float,    float,      "%f");
executor_definition_primitive(double,   double,     "%lf");
executor_definition_primitive(string,   string_t,   "%s");

executor_definition_array(bool,     bool_t,     "%d");
executor_definition_array(char,     char,       "%c");
executor_definition_array(short,    short,      "%d");
executor_definition_array(int,      int,        "%d");
executor_definition_array(long,     long,       "%ld");
executor_definition_array(float,    float,      "%f");
executor_definition_array(double,   double,     "%lf");
executor_definition_array(string,   string_t,   "%s");

/* >>>>>>>>>>>>>>>> END: executor definition functions <<<<<<<<<<<<<<<< */

/* SUITE */
void pre_executor_suite(string_t description)
{
    g_test_reporter->print_description_suite(description, g_test_runner->suite_level, "");
    g_test_runner->suite_level++;
}
void post_executor_suite(string_t description)
{
    g_test_runner->suite_level--;
}

void executor_suite(string_t description, function func)
{
    pre_executor_suite(description);
    func();
    post_executor_suite(description);
}

/* TEST */
void pre_executor_test(string_t description)
{
    g_test_runner->test_collection = (test_t**)realloc(g_test_runner->test_collection,
        sizeof(test_t) * (++g_test_runner->test_count));

    g_test_runner->test_collection[g_test_runner->test_count - 1] = test_create(description, g_test_runner->suite_level);
    g_test_runner->total_success++; // assume that this test will succeed, later we will decrement this if a test failes
}

void post_executor_test(int time_taken)
{
    test_t * test = test_current(g_test_runner);
    test->time_taken = time_taken;
    if (test->is_failure)
    {
        g_test_runner->total_success--;
        g_test_runner->total_failures++;
    }
    print_test_result();
}

void executor_test(string_t description, function func)
{
    pre_executor_test(description);
    start_clock();
    func();
    stop_clock();
    post_executor_test(get_elapsed_time());
}

/* PENDING */
void pre_executor_pending(string_t description)
{
    g_test_runner->test_collection = (test_t**)realloc(g_test_runner->test_collection,
        sizeof(test_t) * (++g_test_runner->test_count));

    test_t * test = test_create(description, g_test_runner->suite_level);
    test->is_pending = true;
    g_test_runner->test_collection[g_test_runner->test_count - 1] = test;
    g_test_runner->total_pending++;
}

void post_executor_pending(string_t description)
{
    test_t * test = test_current(g_test_runner);
    g_test_reporter->print_description_pending(test, "");
}

void executor_pending(string_t description)
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

void print_test_result()
{
    test_t *test = test_current(g_test_runner);
    string_t color = NULL;
    if (!test->is_failure)
    {
        g_test_reporter->print_description_success(test, "");
    }
    else
    {
        g_test_reporter->print_description_failure(test, g_test_runner->total_failures, "");
    }
}
__attribute__((constructor)) void init_test()
{
    dev_null = fopen("/dev/null", "w");
    g_test_runner = calloc(1, sizeof(test_runner_t));
    g_test_reporter = spec_reporter_init();
}

__attribute__((destructor)) void after_test()
{
    g_test_reporter->print_summary(g_test_runner);

    fclose(dev_null);
    test_runner_destroy(g_test_runner);
    spec_reporter_destroy(g_test_reporter);
}
