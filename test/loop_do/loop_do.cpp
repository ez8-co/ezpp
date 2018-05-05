#include "../../ezpp.hpp"

#include <iostream>

#ifdef _MSC_VER
	#include <windows.h>
#else
	#define Sleep(ms) usleep(ms * 1000)
#endif

using namespace std;

void test_do(void)
{
	EZPP_ILDO_DECL(a);
	EZPP_ILDO_DECL(b);
	for(int i = 0; i < 100; i++) {
		EZPP_ILDO_BEGIN(b);
		Sleep(20);
		EZPP_ILDO_END(b);

		EZPP_ILDO(a);
		Sleep(20);
	}
	EZPP_ILDO_END(a);
	EZPP_ILDO_END(b);

	EZPP_ILDO_DECL_IL(c);
	EZPP_ILDO_DECL_IL(d);
	for(int i = 0; i < 100; i++) {
		EZPP_ILDO_BEGIN(d);
		Sleep(20);
		EZPP_ILDO_END(d);

		EZPP_ILDO(c);
		Sleep(20);
	}
	EZPP_ILDO_END(c);
	EZPP_ILDO_END(d);
}

void test_ex_do(void)
{
	EZPP_ILDO_EX_DECL(a, "EZPP_ILDO_EX_DECL");
	EZPP_ILDO_EX_DECL(b, "EZPP_ILDO_EX_DECL");
	for(int i = 0; i < 100; i++) {
		EZPP_ILDO_EX_BEGIN(b);
		Sleep(20);
		EZPP_ILDO_EX_END(b);

		EZPP_ILDO_EX(a);
		Sleep(20);
	}
	EZPP_ILDO_EX_END(a);
	EZPP_ILDO_EX_END(b);

	EZPP_ILDO_EX_DECL_IL(c, "EZPP_ILDO_EX_DECL_IL");
	EZPP_ILDO_EX_DECL_IL(d, "EZPP_ILDO_EX_DECL_IL");
	for(int i = 0; i < 100; i++) {
		EZPP_ILDO_EX_BEGIN(d);
		Sleep(20);
		EZPP_ILDO_EX_END(d);

		EZPP_ILDO_EX(c);
		Sleep(20);
	}
	EZPP_ILDO_EX_END(c);
	EZPP_ILDO_EX_END(d);
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