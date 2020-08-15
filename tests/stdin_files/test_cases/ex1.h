#ifndef EX1_H_
#define EX1_H_

#include "gtest/gtest.h"
#include <stdio.h>
#include <fstream>

#define RECORD_TEST(a, b, description, expected, thunk) \
	TEST(a, b) { \
		::testing::Test::RecordProperty("description", description); \
		::testing::Test::RecordProperty("expected", expected); \
		auto file_name = "unexpected_output_" #b ".txt" ; \
		std::ofstream output(file_name); \
		output << #a << std::endl; \
		output << #b << std::endl; \
		output << description << std::endl; \
		thunk; \
		std::remove(file_name); \
}

extern "C" int get_int();

#endif  // EX1_H_
