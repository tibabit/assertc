#include "assert.h"

int main()
{
    Bool p = true;
    expect_bool(p) to be equal(true);
    return 0;
}
