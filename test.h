#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include "kvstore.h"

class Test {
protected:
	static const std::string not_found_result;

	uint64_t nth_test_cases;
	uint64_t nth_passed_test_cases;
	uint64_t nth_phases;
	uint64_t nth_passed_phases;

#define EXPECT(exp_value, got_value) expect<decltype(got_value)>((exp_value), (got_value), __FILE__, __LINE__)
	template<typename T>
	void expect(const T &exp_value, const T &got_value,
		    const std::string &file, int line)
	{
		++nth_test_cases;
		if (exp_value == got_value) {
			++nth_passed_test_cases;
			return;
		}
		if (verbose) {
			std::cerr << "TEST Error @" << file << ":" << line;
			std::cerr << ", expected value " << exp_value;
			std::cerr << ", got value " << got_value << std::endl;
		}
	}

	void phase(void)
	{
		// Report
		std::cout << "  Phase " << (nth_phases+1) << ": ";
		std::cout << nth_passed_test_cases << "/" << nth_test_cases << " ";

		// Count
		++nth_phases;
		if (nth_test_cases == nth_passed_test_cases) {
			++nth_passed_phases;
			std::cout << "[PASS]" << std::endl;
		} else
			std::cout << "[FAIL]" << std::endl;

		std::cout.flush();

		// Reset
		nth_test_cases = 0;
		nth_passed_test_cases = 0;
	}

	void report(void)
	{
		std::cout << nth_passed_phases << "/" << nth_phases << " passed.";
		std::cout << std::endl;
		std::cout.flush();

		nth_phases = 0;
		nth_passed_phases = 0;
	}

	class KVStore store;
	bool verbose;

public:
	Test(const std::string &dir, bool v=true): store(dir), verbose(v)
	{
		nth_test_cases = 0;
		nth_passed_test_cases = 0;
		nth_phases = 0;
		nth_passed_phases = 0;
	}

	virtual void start_test(void *args = NULL)
	{
		std::cout << "No test is implemented." << std::endl;
	}

};
const std::string Test::not_found_result = "";
