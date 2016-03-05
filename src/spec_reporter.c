#include "spec_reporter.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define SUMMARY                     "\x1b[4m"
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

#define print_description(type)                                 \
    va_list args;                                               \
    va_start(args, format);                                     \
    string_t spaces = get_level_spaces(level);                  \
                                                                \
    printf(type##_LINE_GAP_BEFORE                               \
        "%s" type##_BULLET_COLOR type##_BULLET                  \
        type##_TEXT_COLOR "%s",                                 \
        spaces,                                                 \
        description);                                           \
                                                                \
    vprintf(format, args);                                       \
                                                                \
    printf(RESET_COLOR type##_LINE_GAP_AFTER "\n");             \
                                                                \
    free(spaces);                                               \
    va_end(args);                                               \

#define print_result_counts(count, level, description, type)   \
    {                                                   \
        string_t spaces = get_level_spaces(level);             \
        printf("%s" type##_BULLET_COLOR                 \
        type##_BULLET "%3d %s" RESET_COLOR"\n",         \
        spaces,                                         \
        count,                                          \
        description);                                   \
        free(spaces);                                   \
    }

#define print_failure_description(failure_number, level, format, ...)\
    {                                                           \
        string_t spaces = get_level_spaces(level);              \
        printf("%s%d. " format "\n",                            \
        spaces,                                                 \
        failure_number,                                         \
        ##__VA_ARGS__);                                         \
        free(spaces);                                           \
    }

void spec_print_description_suite       (string_t description, int level, string_t format, ...);
void spec_print_description_success     (test_t * test, string_t format, ...);
void spec_print_description_pending     (test_t * test, string_t format, ...);
void spec_print_description_failure     (test_t *test, int failure_number, string_t format, ...);
void spec_print_summary                 (test_runner_t * runner);

string_t get_level_spaces(int level);
string_t get_message_template(boolean_t output, string_t format, operator_t operator);

test_reporter_t * spec_reporter_init()
{
    test_reporter_t * reporter = calloc(1, sizeof(test_reporter_t));

    reporter->print_description_suite       = spec_print_description_suite;
    reporter->print_description_success     = spec_print_description_success;
    reporter->print_description_failure     = spec_print_description_failure;
    reporter->print_description_pending     = spec_print_description_pending;
    reporter->print_summary                 = spec_print_summary;

    return reporter;
}

void spec_reporter_destroy(test_reporter_t * reporter)
{
    free(reporter);
}

void spec_print_description_suite(string_t description, int level, string_t format, ...)
{
    print_description(SUITE);
}
void spec_print_description_success(test_t * test, string_t format, ...)
{
    int level = test->level;
    string_t description = test->description;
    print_description(SUCCESS);
}

void spec_print_description_pending(test_t * test, string_t format, ...)
{
    int level = test->level;
    string_t description = test->description;
    print_description(PENDING);
}

void spec_print_description_failure(test_t *test, int failure_number, string_t format, ...)
{
    int level = test->level;

    va_list args;
    va_start(args, format);

    string_t spaces = get_level_spaces(level);

    printf("%s" FAILURE_BULLET_COLOR "%d) %s",
        spaces,
        failure_number,
        test->description);

    vprintf(format, args);

    printf(RESET_COLOR "\n");
    free(spaces);

    va_end(args);
}

void spec_print_summary(test_runner_t * runner)
{
    int i = 0, j = 0;
    int failure_number = 0;
    bool_t overall_result = true;

    string_t spaces = get_level_spaces(runner->suite_level);

    printf("\n\n%s" SUMMARY "SUMMARY:" RESET_COLOR "\n\n", spaces);

    string_t template = malloc(1024);
    string_t error = malloc(1024);

    for(i = 0; i< runner->test_count; i++)
    {
        test_t * test = runner->test_collection[i];
        test->level = runner->suite_level; /* show all results at same level */
        if (test->is_failure)
        {
            overall_result = false;
            spec_print_description_failure(test, ++failure_number, "");
            runner->suite_level++;
            for(j = 0; j < test->failed_assertion_count; j++)
            {
                assertion_t * assertion = test->assertions[j];
                sprintf(template, "Assertion Error: %sxpected %s <%s> but was <%s>",
                assertion->output ? "E" : "Not e",
                (assertion->operator == operator_equal ? "" :
                (assertion->operator == operator_above ? "above" :
                (assertion->operator == operator_above_or_equal ? "above or equal" :
                (assertion->operator == operator_below ? "below" : "below or equal")))),
                "%s", "%s");

                sprintf(error, template, assertion->expected, assertion->actual);

                print_failure_description(j + 1, runner->suite_level, "%s, File: %s, Line: %d", error, assertion->file, assertion->ln);
            }
            runner->suite_level--;
        }
    }

    free(template);
    free(error);

    printf("%s", overall_result == false ? "\n\n" : "");
    print_result_counts(runner->total_success, runner->suite_level, "Successful", SUCCESS);

    if (runner->total_failures > 0)
    {
        print_result_counts(runner->total_failures, runner->suite_level, "Failed", FAILURE);
    }
    if (runner->total_pending > 0)
    {
        print_result_counts(runner->total_pending, runner->suite_level, "Pending", PENDING);
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
}

string_t get_level_spaces(int level)
{
    int num_spaces = 4/*increas 4 spaces at each level*/ * level +
    4/*minimum spaces*/;
    string_t spaces = (string_t)malloc(num_spaces + 1);
    memset(spaces, ' ', num_spaces);
    spaces[num_spaces - 1] = 0; // make it null terminated string_t

    return spaces;
}
