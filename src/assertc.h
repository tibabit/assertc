#include <stddef.h>
#include "runner.h"

typedef void    (*function)(void);

/* Begin Definitions */

/* Primitive types */
#define executor_declaration_primitive(suffix, type)    \
    void executor_##suffix(string_t file,               \
        line_t line,                                    \
        type actual,                                    \
        bool_t output,                                  \
        type expected,                                  \
        operator_t operator)

executor_declaration_primitive(bool, bool_t);
executor_declaration_primitive(char, char);
executor_declaration_primitive(short, short);
executor_declaration_primitive(int, int);
executor_declaration_primitive(long, long);
executor_declaration_primitive(float, float);
executor_declaration_primitive(double, double);
executor_declaration_primitive(string, string_t);

/* array */
#define executor_declaration_array(suffix, type)        \
    void executor_array_##suffix(string_t file,         \
        line_t line,                                    \
        type * actual,                                  \
        size_t actual_len,                              \
        bool_t output,                                  \
        type * expected,                                \
        operator_t operator)

executor_declaration_array(bool, bool_t);
executor_declaration_array(char, char);
executor_declaration_array(short, short);
executor_declaration_array(int, int);
executor_declaration_array(long, long);
executor_declaration_array(float, float);
executor_declaration_array(double, double);
executor_declaration_array(string, string_t);

/* End Definitions */

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

/* Begin syntax Definitions */

/* Primitive types */
/* =============== */
#define expect_call_primitive(suffix, actual)     executor_##suffix(__FILE__, __LINE__, actual

#define expect_bool(actual)             expect_call_primitive(bool,   actual)
#define expect_char(actual)             expect_call_primitive(char,   actual)
#define expect_short(actual)            expect_call_primitive(short,  actual)
#define expect_int(actual)              expect_call_primitive(int,    actual)
#define expect_long(actual)             expect_call_primitive(long,   actual)
#define expect_float(actual)            expect_call_primitive(float,  actual)
#define expect_double(actual)           expect_call_primitive(double, actual)
#define expect_string(actual)           expect_call_primitive(string, actual)

/* Array types */
/* =============== */
#define expect_call_array(suffix, actual, len)      executor_array_##suffix(__FILE__, __LINE__, actual, len

#define expect_array_bool(actual, len)              expect_call_array(bool, actual, len)
#define expect_array_char(actual, len)              expect_call_array(char, actual, len)
#define expect_array_short(actual, len)             expect_call_array(short, actual, len)
#define expect_array_int(actual, len)               expect_call_array(int, actual, len)
#define expect_array_long(actual, len)              expect_call_array(long, actual, len)
#define expect_array_float(actual, len)             expect_call_array(float, actual, len)
#define expect_array_double(actual, len)            expect_call_array(double, actual, len)
#define expect_array_string(actual, len)            expect_call_array(string, actual, len)

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

/* End syntax Definitions */
