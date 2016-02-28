#include <stdbool.h>

typedef bool boolean;
typedef char * string;
typedef void (*function)(void);
typedef int line;

#define expect_declaration(suffix, type)        void executor_##suffix(string file, line ln, type actual, boolean output, type expected)

expect_declaration(bool, boolean);
expect_declaration(char, char);
expect_declaration(short, short);
expect_declaration(int, int);
expect_declaration(long, long);
expect_declaration(float, float);
expect_declaration(double, double);
expect_declaration(string, string);

void executor_test      (string description, function func);
void executor_suite     (string description, function func);
void executor_pending   (string description);
void executor_register  (function func);
void executor_module    (function func);
void executor_all       (void);

#define module_name(name)               assert_module_##name
#define module(name)                    void module_name(name)(void)
#define suite(desc)                     executor_suite(desc, ({ void func(void)
#define test(desc)                      executor_test(desc, ({ void func(void)
#define pending(desc)                   executor_pending(desc)
#define end                             func;}));

/**
 * syntax
*/
#define expect_call(suffix, actual)     executor_##suffix(__FILE__, __LINE__, actual,

#define expect_bool(actual)             expect_call(bool,   actual)
#define expect_char(actual)             expect_call(char,   actual)
#define expect_short(actual)            expect_call(short,  actual)
#define expect_int(actual)              expect_call(int,    actual)
#define expect_long(actual)             expect_call(long,   actual)
#define expect_float(actual)            expect_call(float,  actual)
#define expect_double(actual)           expect_call(double, actual)
#define expect_string(actual)           expect_call(string, actual)

#define not                     !
#define to
#define be                      true
#define equal(expected)         , (expected))

#define register_module(name)           executor_register(module_name(name))
#define run(name)                       executor_module(module_name(name))
#define run_all()                       executor_all()
