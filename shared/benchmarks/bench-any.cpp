#define COVSCRIPT_SDK_DEBUG
#define COVSDK_LOGCR_ONLY

#include <covscript/shared/utility.hpp>
#include <covscript/shared/any.hpp>
#include <antlr4-runtime.h>
#include <iostream>
#include <string>
#include <any>

int test_epoch = 10000000;

struct test_large_data {
	std::string str;
	unsigned char alignment[32];
	test_large_data(const char* s) : str(s) {}
};

int main()
{
	std::cout << "Size of std::any        : " << sizeof(std::any) << std::endl;
	std::cout << "Size of cov::any        : " << sizeof(cs::shared::any) << std::endl;
	std::cout << "Size of antlrcpp::Any   : " << sizeof(antlrcpp::Any) << std::endl;

    std::cout << std::endl;

	std::cout << "[Small Data] std::any copying: " << cs::shared::timer::measure([]() {
		std::any a(10);
		for (int i = 0; i < test_epoch; ++i)
			std::any b(a);
	}) << std::endl;
	std::cout << "[Small Data] cov::any copying: " << cs::shared::timer::measure([]() {
		cs::shared::any a(10);
		for (int i = 0; i < test_epoch; ++i)
			cs::shared::any b(a);
	}) << std::endl;
    std::cout << "[Small Data] ant::any copying: " << cs::shared::timer::measure([]() {
        antlrcpp::Any a(10);
        for (int i = 0; i < test_epoch; ++i)
            antlrcpp::Any b(a);
    }) << std::endl;

    std::cout << std::endl;

	std::cout << "[Small Data] std::any instancing: " << cs::shared::timer::measure([]() {
		std::any a(10);
		for (int i = 0; i < test_epoch; ++i) {
			++std::any_cast<int &>(a);
			--std::any_cast<int &>(a);
		}
	}) << std::endl;
	std::cout << "[Small Data] cov::any instancing: " << cs::shared::timer::measure([]() {
		cs::shared::any b(10);
		for (int i = 0; i < test_epoch; ++i) {
			++b.get<int>();
			--b.get<int>();
		}
	}) << std::endl;
    std::cout << "[Small Data] ant::any instancing: " << cs::shared::timer::measure([]() {
        antlrcpp::Any b(10);
        for (int i = 0; i < test_epoch; ++i) {
            ++b.as<int>();
            --b.as<int>();
        }
    }) << std::endl;

    std::cout << std::endl;

	std::cout << "[Large Data] std::any copying: " << cs::shared::timer::measure([]() {
		std::any a(test_large_data("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			std::any b(a);
	}) << std::endl;
	std::cout << "[Large Data] cov::any copying: " << cs::shared::timer::measure([]() {
		cs::shared::any a(test_large_data("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			cs::shared::any b(a);
	}) << std::endl;
    std::cout << "[Large Data] ant::any copying: " << cs::shared::timer::measure([]() {
        antlrcpp::Any a(test_large_data("Hello"));
        for (int i = 0; i < test_epoch; ++i)
            antlrcpp::Any b(a);
    }) << std::endl;

    std::cout << std::endl;

	std::cout << "[Large Data] std::any instancing: " << cs::shared::timer::measure([]() {
		std::any a(test_large_data("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			std::any_cast<test_large_data &>(a).str.push_back('c');
			std::any_cast<test_large_data &>(a).str.pop_back();
		}
	}) << std::endl;
	std::cout << "[Large Data] cov::any instancing: " << cs::shared::timer::measure([]() {
		cs::shared::any b(test_large_data("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			b.get<test_large_data>().str.push_back('c');
			b.get<test_large_data>().str.pop_back();
		}
	}) << std::endl;
    std::cout << "[Large Data] ant::any instancing: " << cs::shared::timer::measure([]() {
        antlrcpp::Any b(test_large_data("Hello"));
        for (int i = 0; i < test_epoch; ++i) {
            b.as<test_large_data>().str.push_back('c');
            b.as<test_large_data>().str.pop_back();
        }
    }) << std::endl;

	return 0;
}
