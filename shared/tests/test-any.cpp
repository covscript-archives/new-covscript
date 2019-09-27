#define COVSCRIPT_SDK_DEBUG

#include <covscript/shared/any.hpp>
#include <iostream>
#include <string>

int main()
{
	std::cout << "Size of any: " << sizeof(cs::shared::any) << std::endl;
	std::cout << "Size of size_t: " << sizeof(std::size_t) << std::endl;
	std::cout << "Size of string: " << sizeof(std::string) << std::endl;
	std::cout << "Size of vector: " << sizeof(std::vector<cs::shared::any>) << std::endl;
	cs::shared::any a, b(10), c(std::string("Hello"));
	cs::shared::any d(a), e(b), f(c);
	a = 3.14;
	d = a;
	std::cout << a.get<double>() <<std::endl;
	std::cout << b.get<int>() << std::endl;
	std::cout << c.get<std::string>() << std::endl;
	std::cout << d.get<double>() << std::endl;
	std::cout << e.get<int>() << std::endl;
	std::cout << f.get<std::string>() << std::endl;
	return 0;
}
