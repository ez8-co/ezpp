#include "../../ezpp.hpp"

#include <iostream>
#include <thread>
using namespace std;

void test_do(void)
{
	EZPP_IL_DO_DECL(a);
	EZPP_IL_DO_DECL(b);
	for(int i = 0; i < 100; i++) {
		EZPP_IL_DO_CODE_CLIP_BEGIN(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		EZPP_IL_DO_CODE_CLIP_END(b);

		EZPP_IL_DO(a);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	EZPP_IL_DO_END(a);
	EZPP_IL_DO_END(b);

	EZPP_IL_DO_DECL_IL(c);
	EZPP_IL_DO_DECL_IL(d);
	for(int i = 0; i < 100; i++) {
		EZPP_IL_DO_CODE_CLIP_BEGIN(d);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		EZPP_IL_DO_CODE_CLIP_END(d);

		EZPP_IL_DO(c);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	EZPP_IL_DO_END(c);
	EZPP_IL_DO_END(d);
}

void test_ex_do(void)
{
	EZPP_IL_DO_EX_DECL(a, "EZPP_IL_DO_EX_DECL");
	EZPP_IL_DO_EX_DECL(b, "EZPP_IL_DO_EX_DECL");
	for(int i = 0; i < 100; i++) {
		EZPP_IL_DO_EX_CODE_CLIP_BEGIN(b);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		EZPP_IL_DO_EX_CODE_CLIP_END(b);

		EZPP_IL_DO_EX(a);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	EZPP_IL_DO_EX_END(a);
	EZPP_IL_DO_EX_END(b);

	EZPP_IL_DO_EX_DECL_IL(c, "EZPP_IL_DO_EX_DECL_IL");
	EZPP_IL_DO_EX_DECL_IL(d, "EZPP_IL_DO_EX_DECL_IL");
	for(int i = 0; i < 100; i++) {
		EZPP_IL_DO_EX_CODE_CLIP_BEGIN(d);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		EZPP_IL_DO_EX_CODE_CLIP_END(d);

		EZPP_IL_DO_EX(c);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	EZPP_IL_DO_EX_END(c);
	EZPP_IL_DO_EX_END(d);
}

int main(int argc,  char** argv)
{
	try {
		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);
		test_do();
		test_ex_do();
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}