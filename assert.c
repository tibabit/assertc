#include "assert.h"

#include <stdio.h>

#define print_failure(fmt1, fmt2, actual, expected)          \
    printf("Assertion failed because expected: %"#fmt1", actual: %"#fmt2"\n", actual, expected)
#define print_success(fmt1, fmt2, actual, expected)          \
    printf("Assertion Successful because expected: %"#fmt1", actual: %"#fmt2"\n", actual, expected)

#define print_result(fmt1, fmt2, actual, expected, result)          \
    if (result)                                                     \
        print_success(fmt1, fmt2, actual, expected);                \
    else                                                            \
        print_failure(fmt1, fmt2, actual, expected);

#define bool_to_str(val)            (val == true ? "true" : "false")

void executor_bool(Bool actual, Bool output, Bool expected)
{
    bool result = (actual == expected) == output;
    print_result(s, s, bool_to_str(actual), bool_to_str(expected), result);
}
