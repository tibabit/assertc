# assert
assertion and testing framework for C

- easy to use
- simple syntax

#### need an example?

```C
#include <stdio.h>
#include "assert.h"

module(byte)
{
    suite("Checking if byte assertions are working fine")
    {
        suite("bool")
        {
            test("Should pass if true == true")
            {
                expect_bool(true) to be equal(true);
            } end
            test("Should pass if true != false")
            {
                expect_bool(false) to not be equal(true);
            } end
        } end
        suite("char")
        {
            test("Should pass if 'c' == 'c'")
            {
                expect_char('c') to be equal('c');
            } end
            test("Should pass if 'c' != 'd'")
            {
                expect_char('c') to not be equal('d');
            } end
        }end
    } end
}
module(number)
{
    suite("Checking if number assertions are working fine")
    {
        suite("short")
        {
            test("Should pass if 5 == 5")
            {
                expect_short(5) to be equal(5);
            } end
            test("Should pass if 5 != 6")
            {
                expect_short(5) to not be equal(6);
            } end
        } end

        suite("int")
        {
            test("Should pass if 10 == 10")
            {
                expect_int(10) to be equal(10);
            } end
            test("Should pass if 10 != 11")
            {
                expect_int(10) to not be equal(11);
            } end
        } end

        suite("long")
        {
            test("Should pass if 100L == 100L")
            {
                expect_long(100L) to be equal(100L);
            } end
            test("Should pass if 100L != 101L")
            {
                expect_long(100L) to not be equal(101L);
            } end
        } end

        suite("float")
        {
            test("Should pass if 10.02 == 10.02")
            {
                expect_float(10.02) to be equal(10.02);
            } end
            test("Should pass if 10.02 != 10.03")
            {
                expect_float(10.02) to not be equal(10.03);
            } end
        } end

        suite("double")
        {
            test("Should pass if 10.25 == 10.25")
            {
                expect_double(10.25) to be equal(10.25);
            } end
            test("Should pass if 10.25 != 10.20")
            {
                expect_double(10.25) to not be equal(10.20);
            } end
        } end
    } end
}

module(string)
{
    suite("Checking if string assertions are working fine")
    {
        suite("string (char *)")
        {
            test("Should pass if \"Hello\" == \"Hello\"")
            {
                expect_string("Hello") to be equal("Hello");
            } end
            test("Should pass if \"Hello\" != \"World\"")
            {
                expect_string("Hello") to not be equal("World");
            } end
            pending("Test case to compare pointers");
        } end
    } end
}

int main()
{
    register_module(number);
    register_module(byte);
    register_module(string);
    run_all();
    return 0;
}
```

#### output

```
Checking if number assertions are working fine

    short
        ✓ Should pass if 5 == 5
        ✓ Should pass if 5 != 6

    int
        ✓ Should pass if 10 == 10
        ✓ Should pass if 10 != 11

    long
        ✓ Should pass if 100L == 100L
        ✓ Should pass if 100L != 101L

    float
        ✓ Should pass if 10.02 == 10.02
        ✓ Should pass if 10.02 != 10.03

    double
        ✓ Should pass if 10.25 == 10.25
        ✓ Should pass if 10.25 != 10.20

Checking if byte assertions are working fine

    bool
        ✓ Should pass if true == true
        ✓ Should pass if true != false

    char
        ✓ Should pass if 'c' == 'c'
        ✓ Should pass if 'c' != 'd'

Checking if string assertions are working fine

    string (char *)
        ✓ Should pass if "Hello" == "Hello"
        ✓ Should pass if "Hello" != "World"
        ! Test case to compare pointers


 SUMMARY:

✓  16 Successful
!   1 Pending


 SUCCESSFUL
```

just started working on it

feel free to suggest, fork, add new features
