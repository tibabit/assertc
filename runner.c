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


assertion_t * assertion_create(string_t file, line_t ln, string_t error)
{
    assertion_t * assertion = (assertion_t *)calloc(1, sizeof(assertion_t));
    assertion->error    = strdup(error);
    assertion->file     = strdup(file);
    assertion->ln       = ln;
}

void assertion_destory(assertion_t * assertion)
{
    free(assertion->file);
    free(assertion->error);
    free(assertion);
}
