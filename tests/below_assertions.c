#include "assert.h"

#include <stdio.h>

module(below)
{
    suite("Checking if below assertions or working fine")
    {
        test("Should pass if c > a")
        {
            expect_char('c') to not be below('a');
        } end
        test("Should pass if 10 > 5")
        {
            expect_short(10) to not be below(5);
        } end
        test("Should pass if 10000000 > 5000000")
        {
            expect_int(10000000) to not be below(5000000);
        } end
        test("Should pass if 10000000L > 5000000L")
        {
            expect_long(10000000L) to not be below(5000000L);
        } end
        test("Should pass if 100.54 > 99.4")
        {
            expect_float(100.54) to not be below(99.4);
        } end
        test("Should pass if 1e-200 > 1e-201")
        {
            expect_double(1e-200) to not be below(1e-201);
        } end
    } end
    suite("Checking if below or equal assertions or working fine")
    {
        test("Should pass if c >= a")
        {
            expect_char('c') to not be below_or_equal('a');
        } end
        test("Should pass if 10 >= 5")
        {
            expect_short(10) to not be below_or_equal(5);
        } end
        test("Should pass if 10000000 >= 5000000")
        {
            expect_int(10000000) to not be below_or_equal(5000000);
        } end
        test("Should pass if 10000000L >= 5000000L")
        {
            expect_long(10000000L) to not be below_or_equal(5000000L);
        } end
        test("Should pass if 100.54 >= 99.4")
        {
            expect_float(100.54) to not be below_or_equal(99.4);
        } end
        test("Should pass if 1e-200 >= 1e-201")
        {
            expect_double(1e-200) to not be below_or_equal(1e-201);
        } end

        test("Should pass if c >= c")
        {
            expect_char('c') to be below_or_equal('c');
        } end
        test("Should pass if 10 >= 10")
        {
            expect_short(10) to be below_or_equal(10);
        } end
        test("Should pass if 10000000 >= 10000000")
        {
            expect_int(10000000) to be below_or_equal(10000000);
        } end
        test("Should pass if 5000000L >= 5000000L")
        {
            expect_long(5000000L) to be below_or_equal(5000000L);
        } end
        test("Should pass if 100.54 >= 100.54")
        {
            expect_float(100.54) to be below_or_equal(100.54);
        } end
        test("Should pass if 1e-200 >= 1e-200")
        {
            expect_double(1e-200) to be below_or_equal(1e-200);
        } end
    } end
}
