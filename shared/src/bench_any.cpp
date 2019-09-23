#define COVSCRIPT_SDK_DEBUG
#define COVSDK_LOGCR_ONLY

#include <covscript/shared/utility.hpp>
#include <covscript/shared/any.hpp>
#include <iostream>
#include <string>
#include <any>

int test_epoch = 10000000;

int main()
{
	std::cout << "Size of std::any: " << sizeof(std::any) << std::endl;
	std::cout << "Size of cov::any: " << sizeof(cs::runtime::any) << std::endl;

	std::cout << "[Small Data] std::any copying: " << cs::utility::timer::measure([]() {
		std::any a(10);
		for (int i = 0; i < test_epoch; ++i)
			std::any b(a);
	}) << std::endl;
	std::cout << "[Small Data] cov::any copying: " << cs::utility::timer::measure([]() {
		cs::runtime::any a(10);
		for (int i = 0; i < test_epoch; ++i)
			cs::runtime::any b(a);
	}) << std::endl;

	std::cout << "[Small Data] std::any instancing: " << cs::utility::timer::measure([]() {
		std::any a(10);
		for (int i = 0; i < test_epoch; ++i) {
			++std::any_cast<int &>(a);
			--std::any_cast<int &>(a);
		}
	}) << std::endl;
	std::cout << "[Small Data] cov::any instancing: " << cs::utility::timer::measure([]() {
		cs::runtime::any b(10);
		for (int i = 0; i < test_epoch; ++i) {
			++b.get<int>();
			--b.get<int>();
		}
	}) << std::endl;

	std::cout << "[Big Data] std::any copying: " << cs::utility::timer::measure([]() {
		std::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			std::any b(a);
	}) << std::endl;
	std::cout << "[Big Data] cov::any copying: " << cs::utility::timer::measure([]() {
		cs::runtime::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i)
			cs::runtime::any b(a);
	}) << std::endl;

	std::cout << "[Big Data] std::any instancing: " << cs::utility::timer::measure([]() {
		std::any a(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			std::any_cast<std::string &>(a).push_back('c');
			std::any_cast<std::string &>(a).pop_back();
		}
	}) << std::endl;
	std::cout << "[Big Data] cov::any instancing: " << cs::utility::timer::measure([]() {
		cs::runtime::any b(std::string("Hello"));
		for (int i = 0; i < test_epoch; ++i) {
			b.get<std::string>().push_back('c');
			b.get<std::string>().pop_back();
		}
	}) << std::endl;

	return 0;
}