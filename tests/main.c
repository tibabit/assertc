#include "assert.h"

extern module(above);
extern module(below);
extern module(byte);
extern module(number);
extern module(string);
extern module(array);

int main()
{
    run(byte);
    run(number);
    run(string);
    run(above);
    run(below);
    run(array);
    return 0;
}
