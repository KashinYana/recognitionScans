#include "util.hpp"
#include <cassert>
#include <iostream>
#include <cstdlib>

void test_pair(int first, int second)
{
	std::pair <int,int> result = util::StrToPair(util::PairToStr(first, second));
	if (result != std::make_pair(first, second)) {
		std::cerr << "Test with pair " << first << ", " << second << " failed" << std::endl;
		std::cerr << first << ", " << second << " -> " << util::PairToStr(first, second) << std::endl;
		std::cerr << util::PairToStr(first, second) << " -> (" << result.first << ", " << result.second << ")" << std::endl;
		exit(1);
	}
	std::cerr << "Test pair (" << first << ", " << second << ") OK" << std::endl;
}

void test_int(int value)
{
	if (util::StrToInt(util::IntToStr(value)) != value) {
		std::cerr << "Test with int " << value << " failed" << std::endl;
		std::cerr << value << " -> " << util::IntToStr(value) << std::endl;
		exit(1);
	}
	std::cerr << "Test int " << value << " OK" << std::endl;
}

int main()
{
	test_int(0);
	test_int(7);
	test_int(-4);
	test_int(34);
	test_int(-45);
	test_int(46323489);
	test_int(-34563422);
	test_pair(0, 0);
	test_pair(1, 1);
	test_pair(-1, -1);
	test_pair(3456, 23545);
}
