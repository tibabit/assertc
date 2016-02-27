# assert
assertion and testing framework for C

- easy to use
- simple syntax

#### need an example?

```
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

```

#### output

```
Test suit 1
       ✗ Test 1

       Test Suit 2 within test suit 1
           ✓ Test 2
       ✓ Test 3


   2) Successful
   1) Failed
```

just started working on it

feel free to suggest, fork, add new features
