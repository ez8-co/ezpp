#include "../../ezpp.hpp"

#include <iostream>
#include <thread>
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
	EZPP_CODE_CLIP_BEGIN(x);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	EZPP_CODE_CLIP_END(x);
	TEST(
		EZPP_CODE_CLIP_BEGIN(x);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EZPP_CODE_CLIP_END(x);
	);
}

void test_do(void)
{
	EZPP_CODE_CLIP_BEGIN_DO(x);
	std::this_thread::sleep_for(std::chrono::seconds(2));
	EZPP_CODE_CLIP_END_DO(x);
}

void test_ex(void)
{
	EZPP_CODE_CLIP_BEGIN_EX(x, "EZPP_CODE_CLIP_BEGIN_EX");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	EZPP_CODE_CLIP_END_EX(x);
	TEST(
		EZPP_CODE_CLIP_BEGIN_EX(x, "EZPP_CODE_CLIP_BEGIN_EX");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		EZPP_CODE_CLIP_END_EX(x);
	);
}

void test_ex_do(void)
{
	EZPP_CODE_CLIP_BEGIN_EX_DO(x, "EZPP_CODE_CLIP_BEGIN_EX_DO");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	EZPP_CODE_CLIP_END_EX_DO(x);
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