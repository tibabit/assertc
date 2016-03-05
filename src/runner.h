#ifndef RUNNER_H_
#define RUNNER_H_

#include <stdbool.h>
typedef bool    bool_t;
typedef char *  string_t;
typedef int     line_t;
typedef bool    boolean_t;

#define MAX_ASSERTIONS_PER_TEST     64
#define MAX_MODULES                 1024

/**
 * operators
 */
 typedef enum _operator
 {
     operator_equal             = 0x00001,
     operator_above             = 0x00010,
     operator_below             = 0x00100,
     operator_above_or_equal    = operator_above | operator_equal,
     operator_below_or_equal    = operator_below | operator_equal
 }operator_t;

typedef struct _assertion
{
    string_t    file;       /* name of the file where assertion failed */
    line_t      ln;         /* line_t number at which assertion failed */
    operator_t  operator;
    bool_t      output;
    string_t    actual;
    string_t    expected;
}assertion_t;

typedef struct _test
{
    string_t        description;                            /* description of the test as provided */
    boolean_t       is_failure;                             /* TRUE if test has failed, FALSE otherwise */
    boolean_t       is_pending;                             /* TRUE if test is pending for later implemention */
    assertion_t *   assertions[MAX_ASSERTIONS_PER_TEST];    /* assertions that belong to this test*/
    int             failed_assertion_count;                 /* number of failed assertions */
    int             time_taken;                             /* time taken in milliseconds to finish this test*/
    int             level;                                  /* test level */
}test_t;

typedef struct _test_runner
{
    test_t ** test_collection;
    int test_count;
    int suite_level;
    int total_failures;
    int total_success;
    int total_pending;
}test_runner_t;

typedef struct _test_reporter
{
    void (*print_description_suite)     (string_t description, int level, string_t format, ...);
    void (*print_description_success)   (test_t *test, string_t format, ...);
    void (*print_description_failure)   (test_t *test, int failure_number, string_t format, ...);
    void (*print_description_pending)   (test_t *test, string_t format, ...);
    void (*print_summary)               (test_runner_t * runner);
}test_reporter_t;

assertion_t *   assertion_create(string_t file,
    line_t line,
    operator_t operator,
    bool_t output,
    string_t actual,
    string_t expected);

void            assertion_destory(assertion_t * assertion);
test_t *        test_create(string_t description, int level);
void            test_add_failure(test_t * test, assertion_t * assertion);
test_t *        test_current(test_runner_t * runner);
void            test_runner_destroy(test_runner_t *runner);

#endif  // RUNNER_H_
