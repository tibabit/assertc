#include <stdbool.h>

typedef bool Bool;
typedef char * String;

#define expect_declaration(suffix, type)        void executor_##suffix(type actual, Bool output, type expected)

expect_declaration(bool, Bool);


#define expect_call(suffix, actual)          executor_bool(actual,

#define expect_bool(actual)                 expect_call(bool, actual)

#define to
#define be                              true
#define equal(expected)                 , (expected))
