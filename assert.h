#include <stdbool.h>
#include "runner.h"

typedef void    (*function)(void);

/**
 * operators
 */
 typedef enum
 {
     operator_equal             = 0x00001,
     operator_above             = 0x00010,
     operator_below             = 0x00100,
     operator_above_or_equal    = operator_above | operator_equal,
     operator_below_or_equal    = operator_below | operator_equal
 }operator_t;

#define executor_declaration(suffix, type)        void executor_##suffix(string_t file, line_t ln, type actual, bool_t output, type expected, operator_t operator)

executor_declaration(bool, bool_t);
executor_declaration(char, char);
executor_declaration(short, short);
executor_declaration(int, int);
executor_declaration(long, long);
executor_declaration(float, float);
executor_declaration(double, double);
executor_declaration(string, string_t);

void executor_test      (string_t description, function func);
void executor_suite     (string_t description, function func);
void executor_pending   (string_t description);
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
#define expect_call(suffix, actual)     executor_##suffix(__FILE__, __LINE__, actual

#define expect_bool(actual)             expect_call(bool,   actual)
#define expect_char(actual)             expect_call(char,   actual)
#define expect_short(actual)            expect_call(short,  actual)
#define expect_int(actual)              expect_call(int,    actual)
#define expect_long(actual)             expect_call(long,   actual)
#define expect_float(actual)            expect_call(float,  actual)
#define expect_double(actual)           expect_call(double, actual)
#define expect_string(actual)           expect_call(string, actual)

#define not                             !
#define to                              ,
#define be                              true
#define equal(expected)                 , expected, operator_equal)
#define above(expected)                 , expected, operator_above)
#define above_or_equal(expected)        , expected, operator_above_or_equal)
#define below(expected)                 , expected, operator_below)
#define below_or_equal(expected)        , expected, operator_below_or_equal)

#define register_module(name)           executor_register(module_name(name))
#define run(name)                       executor_module(module_name(name))
#define run_all()                       executor_all()
