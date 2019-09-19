#include <covscript/shared/any.hpp>
#include <iostream>
#include <string>

int main()
{
    cs::runtime::any a, b(10), c(std::string("Hello"));
    // cs::runtime::any d(a), e(b), f(c);
}