#include "assert.h"

#include <stdio.h>

module(array)
{
    suite("Array")
    {
        test("Array equality check")
        {
            int arr1[] = {2, 3, 4, 3};
            int arr2[] = {1, 2, 3, 4};
            expect_array_int(arr1, 4) to be above(arr2);
        } end
    } end
}

int main()
{
    run(array);
    return 0;
}
