#include "runner.h"

#include <stdlib.h>   /* for malloc/calloc/realloc */
#include <string.h>   /* for strdup */

test_t * test_create(string_t description, int level)
{
    int i = 0;
    test_t * test = (test_t *)calloc(1, sizeof(test_t));
    test->description = strdup(description);
    test->is_failure = false;
    test->is_pending = false;
    test->time_taken = 0;
    test->failed_assertion_count = 0;
    test->level = level;

    for (i = 0; i < MAX_ASSERTIONS_PER_TEST; test->assertions[i++] = NULL);

    return test;
}

void test_destroy(test_t * test)
{
    int i = 0;
    free(test->description);

    for(i = 0; i < test->failed_assertion_count; i++)
    {
        assertion_destory(test->assertions[i]);
    }
    free(test);
}

test_t * test_current(test_runner_t * runner)
{
    return runner->test_collection[runner->test_count - 1];
}

void test_add_failure(test_t * test, assertion_t * assertion)
{
    test->assertions[test->failed_assertion_count++] = assertion;
    test->is_failure = true;
}


assertion_t * assertion_create(string_t file,
    line_t line,
    operator_t operator,
    bool_t output,
    string_t actual,
    string_t expected)
{
    assertion_t * assertion = (assertion_t *)calloc(1, sizeof(assertion_t));
    assertion->actual   = strdup(actual);
    assertion->expected = strdup(expected);
    assertion->file     = strdup(file);
    assertion->ln       = line;
    assertion->operator = operator;
    assertion->output   = output;
}

void assertion_destory(assertion_t * assertion)
{
    free(assertion->file);
    free(assertion->actual);
    free(assertion->expected);
    free(assertion);
}

void test_runner_destroy(test_runner_t *runner)
{
    int i = 0;
    for(i = 0; i < runner->test_count; i++)
    {
        test_destroy(runner->test_collection[i]);
    }
    free(runner->test_collection);
    free(runner);
}
