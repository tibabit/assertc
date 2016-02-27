#include "assert.h"

#include <stdio.h>

// #define EXECUTE_ASSERTIONS

int main()
{
#ifdef EXECUTE_ASSERTIONS
    expect_bool(true) to be equal(true);
    expect_bool(false) to be equal(true);

    expect_char('c') to be equal('c');
    expect_char('c') to be equal('d');

    expect_short(5) to be equal(5);
    expect_short(5) to be equal(6);

    expect_int(10) to be equal(10);
    expect_int(10) to be equal(11);

    expect_long(100L) to be equal(100L);
    expect_long(100L) to be equal(101L);

    expect_float(10.02) to be equal(10.02);
    expect_float(10.02) to be equal(10.03);

    expect_double(10.25) to be equal(10.25);
    expect_double(10.25) to be equal(10.20);

    expect_string("Hello") to be equal("Hello");
    expect_string("Hello") to be equal("World");
#endif

    suite("Test suit 1")
    {
        test("Test 1")
        {
            expect_string("Hello") to be equal("World");
        } end
        suite("Test Suit 2 within test suit 1")
        {
            test("Test 2")
            {
                expect_string("Hello") to be equal("Hello");
            } end
        } end
        test("Test 3")
        {
            expect_string("Hello") to be equal("Hello");
        } end
    } end

    return 0;
}
