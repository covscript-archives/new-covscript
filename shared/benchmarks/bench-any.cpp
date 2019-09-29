#define COVSCRIPT_SDK_DEBUG
#define COVSDK_LOGCR_ONLY

#include <covscript/shared/utility.hpp>
#include <covscript/shared/any.hpp>
#include <antlr4-runtime.h>
#include <iostream>
#include <string>
#include <any>

int test_epoch = 10000000;

int main()
{
	std::cout << "Size of std::any        : " << sizeof(std::any) << std::endl;
	std::cout << "Size of cov::any        : " << sizeof(cs::shared::any) << std::endl;
	std::cout << "Size of antlrcpp::Any   : " << sizeof(antlrcpp::Any) << std::endl;

	std::cout << "std::any copying        : " << cs::shared::timer::measure([]() {
		std::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			std::any b(a);
	}) << std::endl;
	std::cout << "cov::any copying        : " << cs::shared::timer::measure([]() {
		cs::shared::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			cs::shared::any b(a);
	}) << std::endl;
    std::cout << "antlrcpp::Any copying   : " << cs::shared::timer::measure([]() {
        antlrcpp::Any a(std::string("Hello"));
        for (int i = 0; i < test_epoch; ++i)
            antlrcpp::Any b(a);
    }) << std::endl;

	std::cout << "std::any instancing     : " << cs::shared::timer::measure([]() {
		std::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			std::any_cast<std::string &>(a).push_back('c');
			std::any_cast<std::string &>(a).pop_back();
		}
	}) << std::endl;
	std::cout << "cov::any instancing     : " << cs::shared::timer::measure([]() {
		cs::shared::any b(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			b.get<std::string>().push_back('c');
			b.get<std::string>().pop_back();
		}
	}) << std::endl;
    std::cout << "antlrcpp::Any instancing: " << cs::shared::timer::measure([]() {
        antlrcpp::Any b(std::string("Hello"));
        for (int i = 0; i < test_epoch; ++i) {
            b.as<std::string>().push_back('c');
            b.as<std::string>().pop_back();
        }
    }) << std::endl;

	return 0;
}
