#include "assert.h"

module(array)
{
    suite("Array")
    {
        test("Array equality check")
        {
            int arr1[] = {1, 2, 3, 4, 5};
            int arr2[] = {1, 2, 3, 4, 5};
            expect_array_int(arr1, 5) to be equal(arr2);
        } end
        test("Array above or equal check")
        {
            int arr1[] = {2, 3, 4, 5, 6};
            int arr2[] = {1, 3, 3, 5, 5};
            expect_array_int(arr1, 5) to be above_or_equal(arr2);
        } end
        /* » */
        test("Array int » below or equal check")
        {
            int arr1[] = {2, 3, 4, 5, 6};
            int arr2[] = {1, 3, 3, 5, 5};
            expect_array_int(arr2, 5) to be below_or_equal(arr1);
        } end
        test("Array equality check with bool")
        {
            bool arr1[] = {false, false, true};
            bool arr2[] = {false, false, true};
            expect_array_bool(arr1, 3) to be equal(arr2);
        } end
        test("Array equality check with char")
        {
            char arr1[] = {'a', 'b', 'c'};
            char arr2[] = {'a', 'b', 'c'};
            expect_array_char(arr1, 3) to be equal(arr2);
        } end
        test("Array equality check with string")
        {
            string_t arr1[] = {"Hello", "World!", "dear"};
            string_t arr2[] = {"Hello", "World!", "dear"};
            expect_array_string(arr1, 3) to be equal(arr2);
        } end
    } end
}
