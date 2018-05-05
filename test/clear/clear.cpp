#include "../../ezpp.hpp"

#include <iostream>
#include <thread>
using namespace std;

void test(void)
{
	{
		EZPP();
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	for(int i = 0; i < 100; i++) {
		EZPP();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	for(int i = 0; i < 100; i++) {
		EZPP_IN_LOOP();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void test_do(void)
{
	EZPP_DO();
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void test_ex(void)
{
	{
		EZPP_EX("EZPP_EX");
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
	for(int i = 0; i < 100; i++) {
		EZPP_EX("EZPP_EX");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	for(int i = 0; i < 100; i++) {
		EZPP_EX_IN_LOOP("EZPP_EX_IN_LOOP");
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void test_ex_do(void)
{
	EZPP_EX_DO("EZPP_EX_DO");
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void test_recursion(int entry)
{
	EZPP_EX("EZPP_EX");
	if(entry == 1) {
		return;
	}
	test_recursion(entry - 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void test_recursion_do(int entry)
{
	EZPP_EX_DO("EZPP_EX_DO");
	if(entry == 1) {
		return;
	}
	test_recursion_do(entry - 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

void test_function_mt(void)
{
	EZPP_EX("EZPP_EX");
	std::this_thread::sleep_for(std::chrono::milliseconds(800));
}

bool run = true;

void test_clear(void)
{
	while(run) {
		std::this_thread::sleep_for(std::chrono::seconds(2));
		EZPP_PRINT();
		EZPP_CLEAR();
	}
}

int main(int argc,  char** argv)
{
	try {
		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);

		std::thread t(test_clear);

		test();
		test_do();
		test_ex();
		test_ex_do();
		test_recursion(10);
		test_recursion_do(10);

		std::thread t1(test_function_mt);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::thread t2(test_function_mt);

		std::this_thread::sleep_for(std::chrono::seconds(2));

		t2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		t1.join();

		run = false;
		t.join();
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}