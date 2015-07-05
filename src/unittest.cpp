#include "openzl.hpp"
#include <iostream>
#include <fstream>

#ifdef _MSC_VER
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

using namespace zl;
using namespace std;

void test_timer()
{
	Timer t;
	long long sum = 0;
	for (int i = 0; i < 99999999; ++i)
	{
		++sum;
	}
	printf("Elapsed time %d sec %d ms %d us %u ns\n", t.elapsed_sec(), t.elapsed_ms(), t.elapsed_us(), t.elapsed_ns());
	t.reset();
	for (int i = 0; i < 99999999; ++i)
	{
		++sum;
	}
	printf("Elapsed time %s \n", t.to_string("[%sec sec] [%sec sec] [%ms ms] [%us us] [%ns ns]" ).c_str());
	printf("Elapsed %f sec\n", t.elapsed_sec_double());
}

void test_date()
{
	auto date = Date::local_time();
	for (auto i = 0; i < 99; ++i)
	{
		cout << date.to_string() << endl;
		date = Date::local_time();
		//sleep(1);
	}

}


void test_filehandler()
{
	{
		FileReader fh("../../cache/log_test_no.txt");

		FileEditor fh2("../../cache/\xe4\xb8\xad\xe6\x96\x87.txt", true);
		fh2 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good, support UTF-8" << os::endl();
		fh2 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good2, support UTF-8!" << os::endl();
		fh2.flush();

		FileEditor fh3 = std::move(fh2);

		//FileEditor fh4("../../cache/\xe4\xb8\xad\xe6\x96\x87.txt", std::ios::out);
		//fh4.flush();
		cout << fh3.is_open() << endl;
		cout << fh2.is_open() << endl;
		//cout << fh4.is_open() << endl;
		fh3 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good3, support UTF-8" << os::endl();
		fh2 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good should not be shown, support UTF-8" << os::endl();
		//fh4 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good from fh4, support UTF-8" << os::endl();
	}
	FileEditor fh2("../../cache/\xe4\xb8\xad\xe6\x96\x87.txt", false);
	fh2 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good, support UTF-8" << os::endl();
	fh2 << "\xe4\xb8\xad\xe6\x96\x87.txt" << " good2, support UTF-8!" << os::endl();
	fh2.flush();

}

void test_directory()
{
	cout << os::get_current_working_directory() << endl;
	cout << os::get_absolute_path("") << endl;
	FileEditor fh("temp.txt", std::ios::out);
	fh << os::get_current_working_directory() << os::endl();
	fh << os::get_absolute_path("") << os::endl();
}

void test_formatter()
{
	std::string str = "somwlojdlks {} here some {}, {}, {}";
	fmt::format_string(str, 1, 2.2, 3.3f);
	cout << str << endl;
}

void test_logger()
{
	log::Logger logger("default");
	auto sptr = std::make_shared<log::SimpleFileSink>("test.log");
	logger.sinkPtrs_.push_back(sptr);
	logger.sinkValves_.push_back(true);
	logger.info("test info {} {}", 1, 2.2);
	logger.info() << "call method 2  " << 1;
	logger.warn("method3") << " followed by this" << os::endl() << " " << os::endl();

	for (auto i = 0; i < 99999; ++i)
	{
		logger.info("Sequence increment {}", i);
	}
}



int main()
{
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	test_timer();
	test_date();
	test_logger();
	test_filehandler();
	test_directory();
	test_formatter();

	system("pause");

	return 0;
}

