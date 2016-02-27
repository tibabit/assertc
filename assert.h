#include <stdbool.h>

typedef bool boolean;
typedef char * string;
typedef void (*function)(void);

#define expect_declaration(suffix, type)        void executor_##suffix(type actual, boolean output, type expected)

expect_declaration(bool, boolean);
expect_declaration(char, char);
expect_declaration(short, short);
expect_declaration(int, int);
expect_declaration(long, long);
expect_declaration(float, float);
expect_declaration(double, double);
expect_declaration(string, string);

void executor_test  (string description, function func);
void executor_suite (string description, function func);

#define expect_call(suffix, actual)     executor_##suffix(actual,

#define expect_bool(actual)             expect_call(bool, actual)
#define expect_char(actual)             expect_call(char, actual)
#define expect_short(actual)            expect_call(short, actual)
#define expect_int(actual)              expect_call(int, actual)
#define expect_long(actual)             expect_call(long, actual)
#define expect_float(actual)            expect_call(float, actual)
#define expect_double(actual)           expect_call(double, actual)
#define expect_string(actual)           expect_call(string, actual)

#define module(name)                    __attribute__((constructor)) assert_module_##name()
#define suite(desc)                     executor_suite(desc, ({ void func()
#define test(desc)                      executor_test(desc, ({ void func()
#define end                             func;}));

/**
 * syntactic sugars
*/
#define not                     !
#define to
#define be                      true
#define equal(expected)         , (expected))
