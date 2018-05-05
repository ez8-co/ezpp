#include "../../ezpp.hpp"

#include <iostream>

#ifdef _MSC_VER
	#include <windows.h>
#else
	#define Sleep(ms) usleep(ms * 1000)
#endif

using namespace std;

void test(void)
{
	EZPP();
}

void test_1(void)
{
	{
		EZPP();
		Sleep(1000);
	}
	EZPP();
	Sleep(500);
}

void test_2(void)
{
	{
		EZPP();
		Sleep(1000);
	}
	EZPP();
}

void test_do(void)
{
	EZPP_DO();
	Sleep(2000);
}

int main(int argc,  char** argv)
{
	try {
		printf("default status: %s\n\n", EZPP_ENABLED() ? "enable" : "disable");

		printf("normal test\n");
		test_do();
		printf("%soutput\n\n", EZPP_ENABLED() ? "" : "no ");

		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);
		printf("after force enable\n");
		test_do();
		printf("output\n\n");

		EZPP_REMOVE_OPTION(EZPP_OPT_FORCE_ENABLE);
		printf("after restore\n");
		test_do();
		printf("no output\n\n");

		EZPP_ADD_OPTION(EZPP_OPT_FORCE_DISABLE);
		printf("after force disable\n");
		test_do();
		printf("no output\n\n");

		EZPP_REMOVE_OPTION(EZPP_OPT_FORCE_DISABLE);
		printf("after restore\n");
		test_do();
		printf("output\n\n");

		EZPP_ADD_OPTION(EZPP_OPT_FORCE_ENABLE);
		printf("print test\n");
		test_1();
		test_2();
		test_2();
		EZPP_ADD_OPTION(EZPP_OPT_SORT_BY_NAME);
		EZPP_PRINT();

		printf("\nsort by call cnt and cost time\n");
		EZPP_ADD_OPTION(EZPP_OPT_SORT_BY_CALL | EZPP_OPT_SORT_BY_COST);
		EZPP_PRINT();

		EZPP_REMOVE_OPTION(EZPP_OPT_SORT);
		EZPP_CLEAR();
		test();

		printf("\nsave file test\n");

		EZPP_SAVE("ppoptiontest.log");
#ifdef _WIN32
		system("type ppoptiontest.log && del ppoptiontest.log");
#else
		system("cat ppoptiontest.log && rm ppoptiontest.log");
#endif

		printf("\nsave in dtor\n");
		EZPP_ADD_OPTION(EZPP_OPT_SAVE_IN_DTOR);
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}