#include "../../ezpp.hpp"

#include <iostream>

#ifdef _MSC_VER
	#include <windows.h>
#else
	#define Sleep(ms) usleep(ms * 1000)
#endif

using namespace std;

class test
{
public:
	EZPP_CLS_REGISTER();
	test(void) {EZPP_CLS_INIT();}
};

void test_(void)
{
	{
		test all;
		test all1;
		test all2;
		Sleep(2000);
	}
	test all3;
}

class test_do
{
public:
	EZPP_CLS_REGISTER_DO();
	test_do(void) {EZPP_CLS_INIT_DO();}
};

void test_do_(void)
{
	{
		test_do do0;
		test_do do2;
		test_do do3;
		Sleep(2000);
	}
	test_do do3;
}

class test_ex
{
public:
	EZPP_CLS_REGISTER_EX();
	test_ex(void) {EZPP_CLS_INIT_EX("EZPP_CLS_INIT_EX");}
};

void test_ex_(void)
{
	{
		test_ex ex;
		test_ex ex1;
		test_ex ex2;
		Sleep(2000);
	}
	test_ex ex3;
}

class test_ex_do
{
public:
	EZPP_CLS_REGISTER_EX_DO();
	test_ex_do(void) {EZPP_CLS_INIT_EX_DO("EZPP_CLS_INIT_EX_DO");}
};

void test_ex_do_(void)
{
	{
		test_ex_do ex_do;
		test_ex_do ex_do1;
		test_ex_do ex_do2;
		Sleep(2000);
	}
	test_ex_do ex_do3;
}

int main(int argc,  char** argv)
{
	try {
		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);
		test_();
 		test_do_();
 		test_ex_();
 		test_ex_do_();
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}