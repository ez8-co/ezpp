#include "../../ezpp.hpp"

#include <iostream>

#ifdef _MSC_VER
	#include <windows.h>
#else
	#define Sleep(ms) usleep(ms * 1000)
#endif

using namespace std;

#define TEST_CNT 10
#define TEST(expression)		\
	do \
	{ \
		for(size_t i = 0; i < TEST_CNT; ++i) { \
			expression; \
		} \
	} while(false)\

void test(void)
{
	EZPP_BEGIN(x);
	Sleep(2000);
	EZPP_END(x);
	TEST(
		EZPP_BEGIN(x);
		Sleep(200);
		EZPP_END(x);
	);
}

void test_do(void)
{
	EZPP_BEGIN_DO(x);
	Sleep(2000);
	EZPP_END_DO(x);
}

void test_ex(void)
{
	EZPP_BEGIN_EX(x, "EZPP_BEGIN_EX");
	Sleep(2000);
	EZPP_END_EX(x);
	TEST(
		EZPP_BEGIN_EX(x, "EZPP_BEGIN_EX");
		Sleep(200);
		EZPP_END_EX(x);
	);
}

void test_ex_do(void)
{
	EZPP_BEGIN_EX_DO(x, "EZPP_BEGIN_EX_DO");
	Sleep(2000);
	EZPP_END_EX_DO(x);
}

int main(int argc,  char** argv)
{
	try {
		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);
		test();
		test_do();
		test_ex();
		test_ex_do();
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}